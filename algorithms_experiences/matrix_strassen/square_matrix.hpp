#ifndef __SQUARE_MATRIX_HPP__
#define __SQUARE_MATRIX_HPP__
 
#include <vector>
#include <iostream>
 
template <typename T>
class SquareMatrix
{
    public:
 
        SquareMatrix(int sideMeasure) : m_matrix(sideMeasure)
        {
            for (unsigned int i = 0; i < this->m_matrix.size(); i++) this->m_matrix[i].resize(this->m_matrix.size(), 0);
        }
 
        virtual ~SquareMatrix(){};
 
         
        SquareMatrix<T> operator*( const SquareMatrix<T>& beta )
        {
            unsigned int tam = this->m_matrix.size();
 
            SquareMatrix<T> gama( tam );
             
            if ( tam == 1 )
            {
                gama.m_matrix[0][0] = this->m_matrix[0][0] * beta.m_matrix[0][0];
            }
            else
            { 
                // other cases are treated here:
                 const unsigned int halfTam = ( tam >> 1 );
 
                SquareMatrix<T> a11( halfTam );               
                SquareMatrix<T> a12( halfTam );
                SquareMatrix<T> a21( halfTam );
                SquareMatrix<T> a22( halfTam );
          
                SquareMatrix<T> b11( halfTam );
                SquareMatrix<T> b12( halfTam );
                SquareMatrix<T> b21( halfTam );
                SquareMatrix<T> b22( halfTam );
          
                 
                //dividing the matrices in 4 sub-matrices:
                for (unsigned int i = 0; i < halfTam; i++)
                {
                    for (unsigned int j = 0; j < halfTam; j++)
                    {
                        a11.m_matrix[i][j] = this->m_matrix[i][j];
                        a12.m_matrix[i][j] = this->m_matrix[i][j + halfTam];
                        a21.m_matrix[i][j] = this->m_matrix[i + halfTam][j];
                        a22.m_matrix[i][j] = this->m_matrix[i + halfTam][j + halfTam];
              
                        b11.m_matrix[i][j] = beta.m_matrix[i][j];
                        b12.m_matrix[i][j] = beta.m_matrix[i][j + halfTam];
                        b21.m_matrix[i][j] = beta.m_matrix[i + halfTam][j];
                        b22.m_matrix[i][j] = beta.m_matrix[i + halfTam][j + halfTam];
                    }
                }
 
                 
                // Calculating p1 to p7:
                SquareMatrix<T> aResult = a11 + a22;
                SquareMatrix<T> bResult = b11 + b22;
                SquareMatrix<T> p1 = aResult * bResult;
  
                aResult = a21 + a22;
                SquareMatrix<T> p2 = aResult * b11;
  
                bResult = b12 - b22;
                SquareMatrix<T> p3 = a11 * bResult;
  
                bResult = b21 - b11;
                SquareMatrix<T> p4 = a22 * bResult;
  
                aResult = a11 + a12;
                SquareMatrix<T> p5 = aResult * b22;
  
                aResult = a21 - a11;
                bResult = b11 + b12;
                SquareMatrix<T> p6 = aResult * bResult;
  
                aResult = a12 - a22;
                bResult = b21 + b22;
                SquareMatrix<T> p7 = aResult * bResult;
  
                 
                // calculating c12, c21, c11 e c22:
                SquareMatrix<T> c12 = p3 + p5;
                SquareMatrix<T> c21 = p2 + p4;
  
                aResult = p1 + p4;
                bResult = aResult + p7;
                SquareMatrix<T> c11 = bResult - p5;
  
                aResult = p1 + p3;
                bResult = aResult + p6;
                SquareMatrix<T> c22 = bResult - p2;
  
                // Grouping the results obtained in a single matrix:
                for (unsigned int i = 0; i < halfTam ; i++)
                {
                    for (unsigned int j = 0 ; j < halfTam ; j++)
                    {
                        gama.m_matrix[i][j] = c11.m_matrix[i][j];
                        gama.m_matrix[i][j + halfTam] = c12.m_matrix[i][j];
                        gama.m_matrix[i + halfTam][j] = c21.m_matrix[i][j];
                        gama.m_matrix[i + halfTam][j + halfTam] = c22.m_matrix[i][j];
                    }
                }
 
            }            
  
            return gama;
        }
 
 
        SquareMatrix<T> operator+(const SquareMatrix<T>& beta)
        {
            SquareMatrix<T> gama(this->m_matrix.size());
 
            this->additionFunction(this->m_matrix, beta.m_matrix, gama.m_matrix, true);
 
            return gama;
        }
 
 
        SquareMatrix<T> operator-(const SquareMatrix<T>& beta)
        {
            SquareMatrix<T> gama(this->m_matrix.size());
 
            this->additionFunction(this->m_matrix, beta.m_matrix, gama.m_matrix, false);
 
            return gama;
        }
 
 
        void setPositionValue(const int i, const int j, const T value)
        {
            this->m_matrix[i][j] = value;
        }
 
        const T getPositionValue(const int i, const int j)
        {
            return this->m_matrix[i][j];
        }
         
        unsigned int getSideMeasure()
        {
            return this->m_matrix.size();
        }
 
        void dumpToStdout()
        {
            for (unsigned int i = 0; i < this->m_matrix.size(); i++)
            {
                for (unsigned int j = 0; j < this->m_matrix.size(); j++)
                {
                    if (j != 0) { std::cout << "\t"; }
 
                    std::cout << this->m_matrix[i][j];
                }
 
                std::cout << std::endl;
            }
        }
 
    private:
 
        std::vector< std::vector< T > > m_matrix;
 
        SquareMatrix(){};
 
        void additionFunction(const std::vector< std::vector< T > >& first, 
            const std::vector< std::vector< T > >& second,
            std::vector< std::vector< T > >& third,
            bool mode)
        {
            for (unsigned int i = 0; i < first.size(); i++)
            {
                for (unsigned int j = 0; j < first.size(); j++)
                {
                    third[i][j] = (mode) ? (first[i][j] + second[i][j]) : (first[i][j] - second[i][j]);
                }
            }
        }
 
};
 
#endif  /* __SQUARE_MATRIX_HPP__ */
