#include <iostream>
#include <ctime>
#include <string>
#include <algorithm>

#include "Matrix.hpp"
#include "FileSet.hpp"
#include "HopcroftKarp.hpp"

void testFileSet(std::string fileName)
{
    FileSet fileB(fileName);

    Matrix B = fileB.getMatrixFromFile();
    B.printMatrix();
}

void solve()
{
    Matrix A(5);
    A.randomizeAllMatrix();
    A.printMatrix(); 

    // Добавляет запреты вхождение из вершины в нее же саму.
    A.addRestrictsCyclePath();

    std::cout << std::endl;
    debugPrintMinRows(&A);
    debugPrintMinCols(&A);

    // Уменьшаем элементы построчно.
    for(size_t index = 0; index < A.getDim(); index++) {
        uint16_t minRowValue = A.getMinInRow(index);
        for(size_t jndex = 0; jndex < A.getDim(); jndex++) {
            uint16_t CellValue = A.getCell(index, jndex);
            if(CellValue > 0) {
                uint16_t newCellValue = CellValue - minRowValue; 
                A.setCell(index, jndex, newCellValue);
            }
        }
    }

    A.printMatrix(0);

    // Уменьшаем элементы постолбично.
    for(size_t index = 0; index < A.getDim(); index++) {
        uint16_t minColValue = A.getMinInCol(index);
        for(size_t jndex = 0; jndex < A.getDim(); jndex++) {
            uint16_t CellValue = A.getCell(jndex, index);
            if(CellValue > 0) {
                uint16_t newCellValue = CellValue - minColValue;
                A.setCell(jndex, index, newCellValue);
            }
        }
    }

    A.printMatrix(0);

    // Смотрим на максимальное паросочетание
    size_t verticies = A.getDim();

    HopcroftKarp hk(verticies, verticies);

    // Соберём граф
    for(size_t index = 0; index < A.getDim(); index++) {
        for(size_t jndex = 0; jndex < A.getDim(); jndex++) {
            if(A.getCell(index, jndex) == 0) {
                // Добавляем ребро в граф
                hk.addEdge(index, jndex);
            }
        }
    }

    int16_t matching = hk.maximumMatching();
    std::vector<std::vector<int16_t>> pairs = hk.getPairs();
    std::cout << "matching: " << matching << " Pairs:" << std::endl;
    for(size_t index = 0; index < pairs.size(); index++) {
        std::cout << " (" << pairs[index][0] << "," << pairs[index][1] << ")" << std::endl;
    }

    

}

int main() {   
    srand( time( 0 ) ); // автоматическая рандомизация

    solve();

    return 0;
}