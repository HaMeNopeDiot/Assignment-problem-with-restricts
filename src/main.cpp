#include <iomanip>
#include <iostream>
#include <vector>
#include <ctime>

#define SPACE_FOR_NUMBERS 3

int16_t get_random_uint(int16_t start = 1, int16_t end = 10) {
    return (start + rand() % end);
}

class Matrix {
    // Data in matrix
    std::vector<std::vector<int16_t>> data;
    // Sizes of matrix
    size_t rows;
    size_t cols;

    public:
    Matrix(size_t size) : rows(size), cols(size)              { data.resize(size, std::vector<int16_t>(size, 0.0f)); }
    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) { data.resize(cols, std::vector<int16_t>(rows, 0.0f)); }

    size_t getRows() { return rows; }
    size_t getCols() { return cols; }

    int8_t setCell(int16_t val, size_t row, size_t col)
    {
        if(row < rows && col < cols) {
            data[row][col] = val;
            return 0;
        } else {
            std::cout << "No such cell!" << std::endl;
            return -1;
        }
    }

    int16_t getCell(size_t row, size_t col)
    {
        if(row < rows && col < cols) {
            return data[row][col];
        } else {
            std::cout << "No such cell!" << std::endl;
            return 0;
        }
    }

    bool isSquareMatrix() { return (cols == rows); }

    void randomizeAllMatrix(int16_t start = 1, int16_t end = 10)
    {
        for(size_t index = 0; index < rows; index++) {
            for(size_t jndex = 0; jndex < cols; jndex++) {
                data[index][jndex] = get_random_uint(start, end);
            }
        }
    }

    void printMatrix() {
        std::cout<< char(0xDA); // "┌";
        // Upper edge
        for(size_t index = 0; index < cols; index++) {
            for(size_t jndex = 0; jndex < SPACE_FOR_NUMBERS; jndex++) {
                std::cout << char(0xC4); // "―";
            }
            if(index == cols - 1) {
                std::cout << char(0xBF) << std::endl; // "┐"
            } else {
                std::cout << char(0xC2); // "┬";
            }
        }
        for(size_t index = 0; index < rows; index++) {
            for(size_t jndex = 0; jndex < cols; jndex++) {
                std::cout << char(0xB3) << std::setw(SPACE_FOR_NUMBERS) << data[index][jndex]; //  "│"
            }
            // start cut the cells
            if(index == rows - 1) {
                std::cout << char(0xB3) << std::endl << char(0xC0); 
            } else {
                std::cout << char(0xB3) << std::endl << char(0xC3); //"├";
            }
            /* Line between two rows */
            for(size_t jndex = 0; jndex < cols; jndex++) {
                for(size_t kndex = 0; kndex < SPACE_FOR_NUMBERS; kndex++) {
                    std::cout << char(0xC4); // "―";
                }
                // Is the end of matrix?
                if(index == rows - 1) {
                    if(jndex == cols - 1) {
                        std::cout << char(0xD9);
                    } else {
                        std::cout << char(0xC1); // "┴";
                    }
                } else {
                    if(jndex == cols - 1) {
                        std::cout << char(0xB4);
                    } else {
                        std::cout << char(0xC5); // "┼";
                    }
                }
            }
            std::cout << std::endl;
        }
    }

    int16_t getMinInRow(size_t row)
    {
        if(row < rows) {
            int16_t min_row = data[row][0];
            for(size_t index = 1; index < cols; index++) {
                int16_t cur_data = data[row][index];
                min_row = (min_row > cur_data)? cur_data: min_row;
            }
            return min_row;
        } else {
            std::cout << "No such row!" << std::endl;
            return 0;
        }
    }

    int16_t getMinInCol(size_t col)
    {
        if(col < cols) {
            int16_t min_col = data[0][col];
            for(size_t index = 1; index < rows; index++) {
                int16_t cur_data = data[index][col];
                min_col = (min_col > cur_data)? cur_data: min_col;
            }
            return min_col;
        } else {
            std::cout << "No such col!" << std::endl;
            return 0;
        }
    }   
};

void debugPrintMinRows(Matrix *Obj)
{
    std::cout<< "Min Rows: ";
    for(size_t index = 0; index < Obj->getRows(); index++) {
        std::cout << Obj->getMinInRow(index) << " ";
    }
    std::cout << std::endl;
}

void debugPrintMinCols(Matrix *Obj)
{
    std::cout<< "Min Cols: ";
    for(size_t index = 0; index < Obj->getCols(); index++) {
        std::cout << Obj->getMinInCol(index) << " ";
    }
    std::cout << std::endl;
}

int main() {   
    srand( time( 0 ) ); // автоматическая рандомизация

    Matrix A(5);
    A.randomizeAllMatrix();
    A.printMatrix(); 

    std::cout << std::endl;
    debugPrintMinRows(&A);
    debugPrintMinCols(&A);

    return 0;
}