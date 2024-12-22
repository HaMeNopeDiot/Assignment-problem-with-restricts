#include <iostream>
#include <ctime>
#include <string>
#include <algorithm>
#include <set>

#include "Matrix.hpp"
#include "FileSet.hpp"
#include "HopcroftKarp.hpp"


#define SIZE_OF_MATIX    5
#define PROCENT_RESTRICT 75

void checkCol(
    Matrix A, 
    Matrix redZeroes,
    std::vector<size_t> *markedRows,
    std::vector<size_t> *markedCols,
    size_t currentRow
    ) {
        for(size_t currentCol = 0; currentCol < A.getCols(); currentCol++) {
            if(A.getCell(currentRow, currentCol) == 0) {
                // std::cout << "mark col: " << currentCol << std::endl;
                bool existCol = false;
                for(size_t index = 0; index < markedCols->size() && !existCol; index++) {
                    if((*markedCols)[index] == currentCol) {
                        existCol = true;
                    }
                }
                // Проверка что мы не дублируем
                if(!existCol) {
                    std::cout << "mark col: " << currentCol << std::endl;
                    // Отмечаем столбец
                    markedCols->push_back(currentCol);
                    // Ищем отмеченные нули
                    for(size_t currentRowZero = 0; currentRowZero < A.getRows(); currentRowZero++) {
                        // Просматриваем ячейку на то что она содержит особый нуль
                        if(redZeroes.getCell(currentRowZero, currentCol) == 1) {
                            // std::cout << "mark row: " << currentRowZero << std::endl;
                            // Проверим есть ли уже такая строка
                            bool existRow = false;
                            for(size_t index = 0; index < markedRows->size() && !existRow; index++) {
                                if((*markedRows)[index] == currentRowZero) {
                                    existRow = true;
                                }
                            }
                            // Проверяем всё ли окей
                            if(!existRow) {
                                // Отмечаем строку
                                std::cout << "mark row: " << currentRowZero << std::endl;
                                markedRows->push_back(currentRowZero);
                                checkCol(A, redZeroes, markedRows, markedCols, currentRowZero);
                            }
                        }
                        // std::cout<<"Status: " << currentRow << " " << currentCol << " " << currentRowZero << std::endl;
                    }
                }
            }
        }
    }

Matrix byFileSet(std::string fileName)
{
    FileSet fileB(fileName);

    Matrix B = fileB.getMatrixFromFile();
    B.printMatrix();
    return B;
}

Matrix byRandomize(size_t dim)
{
    Matrix A(dim);
    A.randomizeAllMatrix();
    
    // A.addRestrictsCyclePath();

    // Добавляем ограничения в матрицу
    A.addRandomRestricts(PROCENT_RESTRICT);
    
    return A;
}

// ERRORS

void error(){
    Matrix A = byFileSet("ifiles/er1.txt");
    //A.printMatrix();
    size_t verticies = A.getDim();

        HopcroftKarp hk(verticies);

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



void solve()
{
    bool solved = false;
    bool solveable = true;

    Matrix A = byRandomize(SIZE_OF_MATIX);

    // Matrix A = byFileSet("ifiles/er2.txt");

    Matrix SourceMatrix = A;


    // Посмотрим есть ли строки полностью из рестриктов
    bool allRestrictRow = false;
    for(size_t index = 0; index < A.getRows() && !allRestrictRow; index++) {
        size_t restrict_cnt = 0;
        for(size_t jndex = 0; jndex < A.getCols(); jndex++) {
            if(A.getCell(index, jndex) == (uint16_t)(-1)) {
                restrict_cnt++;
            }
        }
        if(restrict_cnt == A.getDim()) {
            allRestrictRow = true;
        }
    }

    bool allRestrictCol = false;
    if(!allRestrictRow) {
        for(size_t index = 0; index < A.getRows() && !allRestrictCol; index++) {
            size_t restrict_cnt = 0;
            for(size_t jndex = 0; jndex < A.getCols(); jndex++) {
                if(A.getCell(jndex, index) == (uint16_t)(-1)) {
                    restrict_cnt++;
                }
            }
            if(restrict_cnt == A.getDim()) {
                allRestrictCol = true;
            }
        }
    }

    solveable = ((!allRestrictCol) & (!allRestrictRow));
    std::cout << "Source Matrix" << std::endl;
    A.printMatrix(); 

    while(!solved && solveable) {
        
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

        HopcroftKarp hk(verticies);

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

        // Сортируем
        std::sort(pairs.begin(), pairs.end(), [](const std::vector<int16_t>& a, const std::vector<int16_t>& b) {
                return a[0] < b[0]; // Сравниваем первый элемент
            });

        // Смотрим на независимые нули
        std::set<int16_t> rowPos;
        std::set<int16_t> colPos;

        for(size_t index = 0; index < matching; index++) {
            rowPos.insert(pairs[index][0]);
            colPos.insert(pairs[index][1]);
        }

        size_t rowZeroes = rowPos.size();
        size_t colZeroes = colPos.size();

        std::cout << "RowPos: " << rowZeroes << " ColPos: " << colZeroes << " Match: " << matching << std::endl;

        // Собираем независимые нули
        std::vector<std::vector<int16_t>> choosedPairs;
        for(size_t index = 0; index < pairs.size(); index++) {
            bool flag = true;
            // проверяем на независимость
            for(size_t jndex = 0; jndex < choosedPairs.size() && flag; jndex++) {
                if((choosedPairs[jndex][0] == pairs[index][0]) || (choosedPairs[jndex][1] == pairs[index][1])) {
                    flag = false;
                }
            }
            if(flag) {
                choosedPairs.push_back(pairs[index]);
            }

        }
        // Смотрим на полученный результат
        for(size_t index = 0; index < choosedPairs.size(); index++) {
            std::cout << " (" << choosedPairs[index][0] << "," << choosedPairs[index][1] << ")" << std::endl;
        }

        // Решена ли задача?
        if(choosedPairs.size() != A.getDim()) {
            // Формируем матрицу местоположения выделенных нулей
            Matrix redZeroes(A.getDim());
            for(size_t index = 0; index < choosedPairs.size(); index++) {
                redZeroes.setCell(choosedPairs[index][0], choosedPairs[index][1], 1);
            }

            // Заводим векторы для хранения отмечаемых строк и столбцов
            std::vector<size_t> markedRows;
            std::vector<size_t> markedCols;

            // --- ПРОИЗВОДИМ ПОИСК ОТМЕЧАЕМЫХ СТРОК И СТОЛБЦОВ --- //
            // Ищем первую строку без выделенных нулей
            // std::cout << "debug: " << A.getDim() << std::endl;
            for(size_t currentRow = 0; currentRow < A.getDim(); currentRow++) {
                // ищем по строке
                bool exist = false;
                for(size_t index = 0; index < choosedPairs.size(); index++) {
                    if(choosedPairs[index][0] == currentRow) {
                        exist = true;
                    }
                }
                // проверяем на существование
                if((!exist)) {
                    checkCol(A, redZeroes, &markedRows, &markedCols, currentRow);
                }
                
            }

            // Выводим помеченные строки
            std::cout << "ROWS:";
            for(size_t index = 0; index < markedRows.size(); index++) {
                std::cout << markedRows[index] << " ";
            }
            std::cout << std::endl;

            // Выводим помеченные столбцы
            std::cout << "COLS:";
            for(size_t index = 0; index < markedCols.size(); index++) {
                std::cout << markedCols[index] << " ";
            }
            std::cout << std::endl;

            std::cout << "SIZES: C: " << markedCols.size() << " R: " << markedRows.size() << std::endl; 

            if(markedCols.size() == 0 && markedRows.size() == 0) {
                // Увы, не решаемо :c
                solveable = false;
            } else {
                // К счастью, решаемо
                // Заводим матрицу перекрестий
                Matrix cross(A.getDim());
                for(size_t index = 0; index < cross.getDim(); index++) {
                    
                    // Ищем строку в пометках
                    bool markedRow = false;
                    for(size_t jndex = 0; jndex < markedRows.size() && !markedRow; jndex++) {
                        if(markedRows[jndex] == index) {
                            markedRow = true;
                        }  
                    }
                    uint16_t valueR = markedRow? 0: 1;
                    for(size_t jndex = 0; jndex < cross.getDim(); jndex++) {
                        // есть ли помеченные столбцы
                        bool markedCol = false;
                        for(size_t kndex = 0; kndex < cross.getDim() && !markedCol; kndex++) {
                            if(markedCols[kndex] == jndex) {
                                markedCol = true;
                            }
                        }
                        uint16_t valueC = markedCol? 1: 0;
                        // Записываем сколько раз линии пересекли ячейку
                        cross.setCell(index, jndex, valueC + valueR);
                    }
                }
                // Посмотрим что вывелось
                cross.printMatrix();

                // Ищем минимальный элемент, который не пересекали матрицы
                uint16_t minVal = -1;
                for(size_t index = 0; index < A.getDim(); index++) {
                    for(size_t jndex = 0; jndex < A.getDim(); jndex++) {
                        uint16_t tmpVal = A.getCell(index, jndex);
                        if(minVal > tmpVal && cross.getCell(index, jndex) == 0) {
                            minVal = tmpVal;
                        }
                    }
                }
                std::cout << "MinVal: " << minVal << std::endl;

                // Используем минимальный элемент на матрице
                for(size_t index = 0; index < A.getDim(); index++) {
                    for(size_t jndex = 0; jndex < A.getDim(); jndex++) {
                        uint16_t curCell = A.getCell(index, jndex);
                        if(cross.getCell(index, jndex) == 0) {
                            A.setCell(index, jndex, curCell - minVal);
                        }
                        if(cross.getCell(index, jndex) == 2) {
                            A.setCell(index, jndex, curCell + minVal);
                        }
                    }
                }
                // Смотрим что получилось 
                A.printMatrix();
            }
        } else {
            solved = true;

            // Сформируем матрицу назначений
            std::vector<std::vector<int16_t>> answerPairs = choosedPairs;
            Matrix AnswerMatrix(A.getDim());
            for(size_t index = 0; index < A.getDim(); index++) {
                for(size_t jndex = 0; jndex < A.getDim(); jndex++) {
                    for(size_t kndex = 0; kndex < choosedPairs.size(); kndex++) {
                        if(choosedPairs[kndex][0] == index && choosedPairs[kndex][1] == jndex) {
                            AnswerMatrix.setCell(index, jndex, 1);
                        }
                    }
                    
                }   
            }
            // Напечатаем её
            AnswerMatrix.printMatrix();
            // Посчитаем ответ
            uint16_t answerValue = 0;
            for(size_t index = 0; index < A.getDim(); index++) {
                for(size_t jndex = 0; jndex < A.getDim(); jndex++) {
                    answerValue += AnswerMatrix.getCell(index, jndex) * SourceMatrix.getCell(index, jndex);
                }   
            }

            std::cout << "answer: " << answerValue << std::endl;
        }
    }
    if(!solveable) {
        // если это не решаемо
        std::cout << "Matrix not solveable" << std::endl;
    }
}

int main() {   
    srand( time( 0 ) ); // автоматическая рандомизация

    std::cout << "@@@ @@@ @@@ BUGAGA @@@ @@@ @@@" << std::endl;
    solve();
    std::cout << "@@@ @@@ @@@ GETAKA @@@ @@@ @@@" << std::endl;
    return 0;
}