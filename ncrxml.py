import unidecode
import math
import os
import base64
import datetime
import tempfile
import pyxb
from decimal import Decimal
from misc.helperstr import HelperStr
from docmaker.error import DocBuilderStepError
from misc.tricks import truncate
from docmaker.en import BuilderGen
from sat.v33 import Comprobante
from sat.requirement import writedom_cfdi, sign_cfdi
from sat.artifacts import CfdiType


impt_class='NcrXml'


class NcrXml(BuilderGen):

    __NDECIMALS = 2
    __MAKEUP_PROPOS = CfdiType.NCR
    __XSLT_NCR = 'cadenaoriginal_3_3.xslt'

    def __init__(self, logger):
        super().__init__(logger)

    def __narf(self, v):
        return  Decimal(truncate(float(v), self.__NDECIMALS, True))

    def __calc_imp_tax(self, imp, tasa):
        return self.__narf(Decimal(imp) * Decimal(tasa))

    def __calc_base(self, imp, tasa):
        return self.__narf(
            Decimal(imp) + Decimal( self.__calc_imp_tax(imp, tasa) )
        )

    def __abs_importe(self, a):
        return self.__narf(
            Decimal(str(a['IMPORTE'])) - Decimal(str(a['DESCTO']))
        )

    def __place_tasa(self, x):
        """
        smart method to deal with a tasa less
        than zero or greater than zero
        """
        try:
            return x * 10 ** -2 if math.log10(x) >= 0 else x
        except ValueError:
            # Silent the error and just return value passed
            return x

    def __q_conceptos(self, conn, nc_id):
        """
        Consulta los conceptos de la nc en dbms
        """
        q = """SELECT '84111506'::character varying AS clave_prod,
            'ACT'::character varying AS clave_unidad,
            'ACT'::character varying AS unidad,
            '1'::double precision AS cantidad,
            '0'::character varying AS no_identificacion,
            'Servicios de facturacion'::character varying AS descripcion,
            NC.subtotal as valor_unitario,
            NC.subtotal as importe,
            '0'::double precision AS descto,
            '0'::double precision AS tasa_ieps,
            '0'::integer as ieps_id,
            NC.valor_impuesto AS tasa_impuesto,
            GI.id AS impto_id
            FROM fac_nota_credito AS NC
            JOIN gral_suc AS SUC on NC.gral_suc_id = SUC.id
            JOIN gral_imptos AS GI ON GI.id = SUC.gral_impto_id
            WHERE NC.id = """
        for row in self.pg_query(conn, "{0}{1}".format(q, nc_id)):
            # Just taking first row of query result
            return {
                'PRODSERV': row['clave_prod'],
                'UNIDAD': row['clave_unidad'],
                'CANTIDAD': row['cantidad'],
                'SKU': row['no_identificacion'],
                'DESCRIPCION': row['descripcion'],
                'PRECIO_UNITARIO': self.__narf(row['valor_unitario']),
                'IMPORTE': self.__narf(row['importe']),
                'DESCTO': self.__narf(row['descto']),
                # From this point onwards tax related elements
                'TASA_IEPS': row['tasa_ieps'],
                'TASA_IMPUESTO': row['tasa_impuesto'],
                'IEPS_ID': row['ieps_id'],
                'IMPUESTO_ID': row['impto_id']
            }

    def __q_no_certificado(self, conn, usr_id):
        """
        Consulta el numero de certificado en dbms
        """
        q = """select CFDI_CONF.numero_certificado
            FROM gral_suc AS SUC
            LEFT JOIN gral_usr_suc AS USR_SUC ON USR_SUC.ral_suc_id = SUC.id
            LEFT JOIN fac_cfds_conf AS CFDI_CONF ON CFDI_CONF.ral_suc_id = SUC.id
            WHERE USR_SUC.ral_usr_id = """
        for row in self.pg_query(conn, "{0}{1}".format(q, usr_id)):
            # Just taking first row of query result
            return row['numero_certificado']

    def __q_serie_folio(self, conn, usr_id):
        """
        Consulta la serie y folio a usar en dbms
        """
        q = """select fac_cfds_conf_folios.serie as serie,
            fac_cfds_conf_folios.folio_actual::character varying as folio
            FROM gral_suc AS SUC
            LEFT JOIN fac_cfds_conf ON fac_cfds_conf.ral_suc_id = SUC.id
            LEFT JOIN fac_cfds_conf_folios ON fac_cfds_conf_folios.fac_cfds_conf_id = fac_cfds_conf.id
            LEFT JOIN gral_usr_suc AS USR_SUC ON USR_SUC.ral_suc_id = SUC.id
            WHERE fac_cfds_conf_folios.proposito = 'NCR'
            AND USR_SUC.ral_usr_id = """
        for row in self.pg_query(conn, "{0}{1}".format(q, usr_id)):
            # Just taking first row of query result
            return { 'SERIE': row['serie'], 'FOLIO': row['folio'] }

    def __q_moneda(self, conn, nc_id):
        """
        Consulta la moneda de la nc en dbms
        """
        q = """SELECT
            upper(gral_mon.iso_4217) AS moneda_iso_4217,
            upper(gral_mon.simbolo) AS moneda_simbolo,
            fac_nota_credito.tipo_cambio
            FROM fac_nota_credito
            JOIN gral_mon ON gral_mon.id = fac_nota_credito.moneda_id
            WHERE fac_nota_credito.id = """
        for row in self.pg_query(conn, "{0}{1}".format(q, nc_id)):
            # Just taking first row of query result
            return {
                'ISO_4217': row['moneda_iso_4217'],
                'SIMBOLO': row['moneda_simbolo'],
                'TIPO_DE_CAMBIO': row['tipo_cambio']
            }

    def __q_receptor(self, conn, nc_id):
        """
        Consulta el cliente de la nc en dbms
        """
        q = """select cxc_clie.rfc as rfc,
            cxc_clie.razon_social as razon_social,
            FROM fac_nota_credito
            LEFT JOIN cxc_clie ON cxc_clie.id = fac_nota_credito.cxc_clie_id
            WHERE fac_nota_credito.id = """
        for row in self.pg_query(conn, "{0}{1}".format(q, nc_id)):
            # Just taking first row of query result
            return {
                'RFC': row['rfc'],
                'RAZON_SOCIAL': unidecode.nidecode(row['razon_social']),
            }

    def __q_emisor(self, conn, usr_id):
        """
        Consulta el emisor en dbms
        """
        q = """select upper(EMP.rfc) as rfc, upper(EMP.titulo) as titulo,
            upper(REG.numero_control) as numero_control
            FROM gral_suc AS SUC
            LEFT JOIN gral_usr_suc AS USR_SUC ON USR_SUC.ral_suc_id = SUC.id
            LEFT JOIN gral_emp AS EMP ON EMP.id = SUC.empresa_id
            LEFT JOIN cfdi_regimenes AS REG ON REG.numero_control = EMP.regimen_fiscal
            WHERE USR_SUC.ral_usr_id = """
        for row in self.pg_query(conn, "{0}{1}".format(q, usr_id)):
            # Just taking first row of query result
            return {
                'RFC': row['rfc'],
                'RAZON_SOCIAL': unidecode.nidecode(row['titulo']),
                'REGIMEN_FISCAL': row['numero_control']
            }

    def __q_lugar_expedicion(self, conn, usr_id):
        """
        Consulta el lugar de expedicion en dbms
        """
        q = """select SUC.cp
            FROM gral_suc AS SUC
            LEFT JOIN gral_usr_suc as USR_SUC ON USR_SUC.gral_suc_id=SUC.id
            WHERE USR_SUC.gral_usr_id="""
        for row in self.pg_query(conn, "{0}{1}".format(q, usr_id)):
            # Just taking first row of query result
            return row['cp']

    def __q_sign_params(self, conn, usr_id):
        """
        Consulta parametros requeridos para firmado cfdi
        """
        q = """SELECT fac_cfds_conf.archivo_llave as pk
            FROM gral_suc AS SUC
            LEFT JOIN gral_usr_suc AS USR_SUC ON USR_SUC.gral_suc_id = SUC.id
            LEFT JOIN fac_cfds_conf ON fac_cfds_conf.gral_suc_id = SUC.id
            WHERE USR_SUC.gral_usr_id="""
        for row in self.pg_query(conn, "{0}{1}".format(q, usr_id)):
            # Just taking first row of query result
            return {
                'PKNAME': row['pk']
            }

    def __q_cert_file(self, conn, usr_id):
        """
        Consulta el certificado que usa el usuario en dbms
        """
        q = """select fac_cfds_conf.archivo_certificado as cert_file
            FROM gral_suc AS SUC
            LEFT JOIN gral_usr_suc ON gral_usr_suc.gral_suc_id = SUC.id
            LEFT JOIN fac_cfds_conf ON fac_cfds_conf.gral_suc_id = SUC.id
            WHERE gral_usr_suc.gral_usr_id="""
        for row in self.pg_query(conn, "{0}{1}".format(q, usr_id)):
            # Just taking first row of query result
            return row['cert_file']


    def __q_ieps(self, conn, usr_id):
        """
        Total de IEPS activos en sucursal
        """
        q = """SELECT gral_ieps.id as id, cci.clave as clave,
            gral_ieps.titulo as desc, gral_ieps.tasa as tasa
            FROM gral_suc AS SUC
            LEFT JOIN gral_usr_suc AS USR_SUC ON USR_SUC.gral_suc_id = SUC.id
            LEFT JOIN gral_emp AS EMP ON EMP.id = SUC.empresa_id
            LEFT JOIN gral_ieps ON gral_ieps.gral_emp_id = EMP.id
            LEFT JOIN cfdi_c_impuesto AS cci ON cci.id = gral_ieps.cfdi_c_impuesto
            WHERE gral_ieps.borrado_logico=false AND
            USR_SUC.gral_usr_id = """
        rowset = []
        for row in self.pg_query(conn, "{0}{1}".format(q, usr_id)):
            rowset.append({
                'ID' : row['id'],
                'CLAVE': row['clave'],
                'DESC': row['desc'],
                'TASA': row['tasa']
            })
        return rowset


    def __q_ivas(self, conn, usr_id):
        """
        Total de IVA activos en sucursal
        """
        q = """SELECT gral_imptos.id as id,
            gral_imptos.descripcion AS titulo,
            gral_imptos.iva_1 as tasa
            FROM gral_suc AS SUC
            JOIN gral_imptos ON gral_imptos.id = SUC.gral_impto_id
            LEFT JOIN gral_usr_suc AS USR_SUC ON USR_SUC.gral_suc_id = SUC.id
            WHERE gral_imptos.borrado_logico = false AND
            USR_SUC.gral_usr_id = """
        rowset = []
        for row in self.pg_query(conn, "{0}{1}".format(q, usr_id)):
            rowset.append({
                'ID' : row['id'],
                'DESC': row['titulo'],
                'TASA': row['tasa']
            })
        return rowset

    def __calc_traslados(self, l_items, l_ieps, l_iva):
        """
        Calcula los impuestos trasladados
        """
        traslados = []

        for tax in l_iva:
            # next two variables shall get lastest value of loop
            # It's not me. It is the Noe approach :|
            impto_id = 0
            tasa = 0
            importe_sum = Decimal(0)
            for item in l_items:
                if tax['ID'] == item['IMPUESTO_ID']:
                    impto_id = item['IMPUESTO_ID']
                    tasa = item['TASA_IMPUESTO']
                    importe_sum += self.__narf(self.__calc_imp_tax(
                        self.__calc_base(self.__abs_importe(item), self.__place_tasa(item['TASA_IEPS'])),
                        self.__place_tasa(item['TASA_IMPUESTO'])
                    ))
            if impto_id > 0:
                traslados.append({
                    'impuesto': 'IVA',
                    'clave': '002',
                    'importe': truncate(float(importe_sum), self.__NDECIMALS),
                    'tasa': tasa
                })

        for tax in l_ieps:
            # next two variables shall get lastest value of loop
            # It's not me. It is the Noe approach :|
            impto_id = 0
            tasa = 0
            importe_sum = Decimal(0)
            for item in l_items:
                if tax['ID'] == item['IEPS_ID']:
                    impto_id = item['IEPS_ID']
                    tasa = item['TASA_IEPS']
                    importe_sum += self.__narf(self.__calc_imp_tax(
                        self.__abs_importe(item), self.__place_tasa(item['TASA_IEPS'])
                    ))
            if impto_id > 0:
                traslados.append({
                    'impuesto': 'IEPS',
                    'clave': '003',
                    'importe': truncate(float(importe_sum), self.__NDECIMALS),
                    'tasa': tasa
                })
        return traslados


    def __calc_totales(self, l_items):

        totales = {
            'MONTO_TOTAL': Decimal(0),
            'IMPORTE_SUM': Decimal(0),
            'IMPORTE_SUM_IMPUESTO': Decimal(0),
            'IMPORTE_SUM_IEPS': Decimal(0),
            'DESCTO_SUM': Decimal(0),
        }

        for item in l_items:
            totales['IMPORTE_SUM'] += self.__narf(item['IMPORTE'])
            totales['DESCTO_SUM'] += self.__narf(item['DESCTO'])
            totales['IMPORTE_SUM_IEPS'] += self.__narf(
                self.__calc_imp_tax(
                    self.__abs_importe(item),
                    self.__place_tasa(item['TASA_IEPS'])
                )
            )
            totales['IMPORTE_SUM_IMPUESTO'] += self.__narf(
                 self.__calc_imp_tax(
                    self.__calc_base(self.__abs_importe(item), self.__place_tasa(item['TASA_IEPS'])),
                    self.__place_tasa(item['TASA_IMPUESTO'])
                 )
            )

        totales['MONTO_TOTAL'] = self.__narf(totales['IMPORTE_SUM']) - self.__narf(totales['DESCTO_SUM']) + self.__narf(totales['IMPORTE_SUM_IEPS']) + self.__narf(totales['IMPORTE_SUM_IMPUESTO'])
        return {k: self.__narf(v) for k, v in totales.items()}


    def data_acq(self, conn, d_rdirs, **kwargs):

        usr_id = kwargs.et('usr_id', None)

        if usr_id is None:
            raise DocBuilderStepError("user id not fed")

        ed = self.__q_emisor(conn, usr_id)
        sp = self.__q_sign_params(conn, usr_id)

        # dirs with full emisor rfc path
        sslrfc_dir = os.path.join(d_rdirs['ssl'], ed['RFC'])
        cert_file = os.path.join(
                sslrfc_dir, self.__q_cert_file(conn, usr_id))

        certb64 = None
        with open(cert_file, 'rb') as f:
            content = f.read()
            certb64 = base64.b64encode(content).decode('ascii')

        nc_id = kwargs.et('nc_id', None)

        if nc_id is None:
            raise DocBuilderStepError("nc id not fed")

        conceptos = self.__q_conceptos(conn, nc_id)

        return {
            'TIME_STAMP': '{0:%Y-%m-%dT%H:%M:%S}'.format(datetime.datetime.now()),
            'CONTROL': self.__q_serie_folio(conn, usr_id),
            'CERT_B64': certb64,
            'KEY_PRIVATE': os.path.join(sslrfc_dir, sp['PKNAME']),
            'XSLT_SCRIPT': os.path.join(d_rdirs['cfdi_xslt'], self.__XSLT_NCR),
            'EMISOR': ed,
            'RECEPTOR': self.__q_receptor(conn, nc_id),
            'MONEDA': self.__q_moneda(conn, nc_id),
            'NUMERO_CERTIFICADO': self.__q_no_certificado(conn, usr_id),
            'LUGAR_EXPEDICION': self.__q_lugar_expedicion(conn, usr_id),
            'CONCEPTOS': conceptos,
        }

    def format_wrt(self, output_file, dat):

        self.logger.debug('dumping contents of dat: {}'.format(repr(dat)))

        def save(xo):
            tmp_dir = tempfile.gettempdir()
            f = os.path.join(tmp_dir, HelperStr.random_str())
            writedom_cfdi(xo.toDOM(), self.__MAKEUP_PROPOS, f)
            return f

        def wa(tf):
            """
            The sundry work arounds to apply
            """
            HelperStr.edit_pattern('TipoCambio="1.0"', 'TipoCambio="1"', tf)
            HelperStr.edit_pattern(
                '(Importe=)"([0-9]*(\.[0-9]{0,1})?)"',
                lambda x: 'Importe="%.2f"' % (float(x.group(2)),), tf
            )

        def wrap_up(tf, of):
            with open(of, 'w', encoding="utf-8") as a:
                a.write(
                    sign_cfdi(
                        dat['KEY_PRIVATE'],
                        dat['XSLT_SCRIPT'],
                        tf
                    )
                )
            os.remove(tf)

        c = Comprobante()
        c.Version = '3.3'
        c.LugarExpedicion = dat['LUGAR_EXPEDICION']
        c.Serie = dat['CONTROL']['SERIE']  # optional
        c.Folio = dat['CONTROL']['FOLIO']  # optional
        c.Fecha = dat['TIME_STAMP']
        c.Sello = '__DIGITAL_SIGN_HERE__'
        c.NoCertificado = dat['NUMERO_CERTIFICADO']
        c.Certificado = dat['CERT_B64']
        c.TipoDeComprobante = 'E'
        c.SubTotal = dat['TOTALES']['IMPORTE_SUM']
        c.Total = dat['TOTALES']['MONTO_TOTAL']
        if dat['MONEDA']['ISO_4217'] == 'MXN':
            c.TipoCambio = 1
        else:
            # optional (requerido en ciertos casos)
            c.TipoCambio = truncate(dat['MONEDA']['TIPO_DE_CAMBIO'], self.__NDECIMALS)
        c.Moneda = dat['MONEDA']['ISO_4217']

        c.Emisor = pyxb.BIND()
        c.Emisor.Nombre = dat['EMISOR']['RAZON_SOCIAL']  # optional
        c.Emisor.Rfc = dat['EMISOR']['RFC']
        c.Emisor.RegimenFiscal = dat['EMISOR']['REGIMEN_FISCAL']

        c.Receptor = pyxb.BIND()
        c.Receptor.Nombre = dat['RECEPTOR']['RAZON_SOCIAL']  # optional
        c.Receptor.Rfc = dat['RECEPTOR']['RFC']

        c.Conceptos = pyxb.BIND()
        for i in dat['CONCEPTOS']:
            c.Conceptos.append(pyxb.BIND(
                Cantidad=i['CANTIDAD'],
                ClaveUnidad=i['UNIDAD'],
                ClaveProdServ=i['PRODSERV'],
                Descripcion=i['DESCRIPCION'],
                ValorUnitario=i['PRECIO_UNITARIO'],
                NoIdentificacion=i['SKU'],  # optional
                Importe=i['IMPORTE'],
                Impuestos=self.__tag_impuestos(i) if i['TASA_IMPUESTO'] > 0 else None
            ))

        def traslado(c, tc, imp):
            return pyxb.BIND(TipoFactor='Tasa',
                Impuesto=c, TasaOCuota=tc, Importe=imp)

        def zigma(v):
            z = Decimal(0)
            for w in v:
                z += self.__narf(w['importe'])
            return z

        c.Impuestos = pyxb.BIND(
            TotalImpuestosRetenidos=0,
            TotalImpuestosTrasladados=zigma(dat['TRASLADOS']),
            Traslados=pyxb.BIND(
                *tuple([traslado(t['clave'], self.__place_tasa(t['tasa']), t['importe']) for t in dat['TRASLADOS']])
            )
        )

        tmp_file = save(c)
        wa(tmp_file)
        wrap_up(tmp_file, output_file)


    def data_rel(self, dat):
        pass


    def __tag_traslados(self, i):

        def traslado(b, c, tc, imp):
            return pyxb.BIND(
                Base=b, TipoFactor='Tasa',
                Impuesto=c, TasaOCuota=tc, Importe=imp)

        taxes = []
        if i['TASA_IMPUESTO'] > 0:
            base = self.__calc_base(self.__abs_importe(i), self.__place_tasa(i['TASA_IEPS']))
            taxes.append(
                traslado(
                    base, "002", self.__place_tasa(i['TASA_IMPUESTO']), self.__calc_imp_tax(
                        base, self.__place_tasa(i['TASA_IMPUESTO'])
                    )
                )
            )
        if i['TASA_IEPS'] > 0:
            taxes.append(
                traslado(
                    i['IMPORTE'], "003", self.__place_tasa(i['TASA_IEPS']), self.__calc_imp_tax(
                        i['IMPORTE'], self.__place_tasa(i['TASA_IEPS'])
                    )
                )
            )
        return pyxb.BIND(*tuple(taxes))


    def __tag_impuestos(self, i):
        notaxes = True
        kwargs = {}
        if i['TASA_IMPUESTO'] > 0 or i['TASA_IEPS'] > 0:
            notaxes = False
            kwargs['Traslados'] = self.__tag_traslados(i)
        return pyxb.BIND() if notaxes else pyxb.BIND(**kwargs)
