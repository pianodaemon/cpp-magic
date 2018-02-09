#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
 
#define RUNNING_DIR "/tmp"
#define LOCK_FILE   "cacherd.lock"
#define LOG_FILE    "cacherd.log"
 
void log_message(filename,message)
char *filename;
char *message;
{
FILE *logfile;
    logfile=fopen(filename,"a");
    if(!logfile) return;
    fprintf(logfile,"%s\n",message);
    fclose(logfile);
}
 
 
 
void signal_handler(sig)
int sig;
{
    switch(sig) {
    case SIGHUP:
        log_message(LOG_FILE,"Senal de hangup cachada");
        break;
    case SIGTERM:
        log_message(LOG_FILE,"Senal de Terminacion cachada");
        exit(0);
        break;
    }
}
 
void daemonize(){
    int i,lfp;
    char str[10];
     
    // Primero obtenemos el indentificador de Proceso   
    if( getppid() == 1 ) return; 
     
    // Ahora generamos un proceso hijo y obtenemos su PID
    // mediante la variable i   
    i=fork();
 
    // Comprobamos que el proceso hijo este lanzado de manera correcta
    // Verificando los posibles ERRORES de lanzamiento
    // En caso de existir un error de lanzamiento en el proceso hijo
    // procedemos a la finalizacion del proceso padre 
    if (i<0) { exit(1); } // fork error, el proceso hijo no pudo ser creado
    if (i>0) { exit(0); } /* Padre existe */
 
    // Proceso hijo (Demonio) continua  
    setsid(); // Crea una sesión y define el ID de grupo del proceso
 
    /* Cerrar todos los descriptores de archivo
    OJO: Esto evita que el hijo use los mismos decriptores de fichero que el padre*/
    for (i=getdtablesize();i>=0;--i) close(i); 
 
    i=open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standart I/O */
     
    umask(027); /* Establece los permisos que le seran dados  a los nuevos archivos a crear */
 
    chdir(RUNNING_DIR); /* Cambia el directorio de ejecucion */
 
    /* Abre el archivo de bloqueo en modo lectura/escritura, pero
    si el archivo no existiece, lo crea con el permiso 640 */
    lfp=open(LOCK_FILE,O_RDWR|O_CREAT,0640);
 
    if (lfp<0) { exit(1); } /* No puede ser abierto */
    if (lockf(lfp,F_TLOCK,0)<0) exit(0); /* No puede ser bloqueado el fichero para este proceso*/
 
    /* Primera instancia continua */
    sprintf(str,"%d\n",getpid());
 
    // Escribimos el PID al fichero de bloqueo
    write(lfp,str,strlen(str)); 
 
    signal(SIGCHLD,SIG_IGN); /* Ignora hijo */
    signal(SIGTSTP,SIG_IGN); /* Ignora Senales tty */
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGHUP,signal_handler); /* Cacha Senal hangup (kill -1 PID)*/
    signal(SIGTERM,signal_handler); /* Cacha Senal de Terminacion (kill -15 PID)*/
}
 
int main(){ daemonize(); while(1)  sleep(1);  }
