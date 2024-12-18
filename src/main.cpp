#include <iostream>
#include <ctime>
#include <string>

#include "Matrix.hpp"
#include "FileSet.hpp"

int main() {   
    srand( time( 0 ) ); // автоматическая рандомизация

    Matrix A(5);
    A.randomizeAllMatrix();
    A.printMatrix(); 

    std::cout << std::endl;
    debugPrintMinRows(&A);
    debugPrintMinCols(&A);

    std::string fileName = "ifiles/ex3.txt";
    FileSet fileB(fileName);

    Matrix B = fileB.getMatrixFromFile();
    B.printMatrix();

    return 0;
}