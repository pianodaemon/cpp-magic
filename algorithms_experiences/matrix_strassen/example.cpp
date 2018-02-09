#include "SquareMatrix.hpp"
#include <cmath> 
 
#define MATRIX_MEASURE_SIZE 2
 
int main()
{
 
    SquareMatrix<int> a(MATRIX_MEASURE_SIZE);
    SquareMatrix<int> b(MATRIX_MEASURE_SIZE);
 
    a.setPositionValue(0,0, 1);
    a.setPositionValue(0,1, 2);
    a.setPositionValue(1,0, 3);
    a.setPositionValue(1,1, 4);
    a.dumpToStdout();
 
    std::cout << "=================================" << std::endl;    
 
    b.setPositionValue(0,0, 5);
    b.setPositionValue(0,1, 6);
    b.setPositionValue(1,0, 7);
    b.setPositionValue(1,1, 8);
    b.dumpToStdout();
 
    if ( a.getSideMeasure() == b.getSideMeasure() )
    {
        std::cout << "=================================" << std::endl; 
     
        SquareMatrix<int> c = a * b;
        c.dumpToStdout();
    }
     
 
    return 0;
}
