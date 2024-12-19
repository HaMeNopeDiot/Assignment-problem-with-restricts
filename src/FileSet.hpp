#ifndef _FILE_SET_HPP_
#define _FILE_SET_HPP_

#include <string>
#include <fstream>
#include <vector>

#include "Matrix.hpp"

class FileSet {
    std::string fileName;
    public:
    
    FileSet(std::string fileName) : fileName(fileName) {}
    
    std::string* getFileName() { return &fileName; }
    void         setFileName(std::string newFileName) { fileName = newFileName; }

    Matrix getMatrixFromFile() {
        std::ifstream file(fileName);
        if(!file) {
            std::cout << "Cannot open file!" << std::endl;
            return Matrix(0);
        } else {
            // Read dimension of matrix
            size_t dimension;
            file >> dimension;
            
            std::vector<std::vector<uint16_t>> data;
            for(size_t index = 0; index < dimension; index++) {
                std::vector<uint16_t> data_row;
                for(size_t jndex = 0; jndex < dimension; jndex++) {
                    uint16_t data_tmp;
                    file >> data_tmp;
                    data_row.push_back(data_tmp);
                }
                data.push_back(data_row);
            }

            return Matrix(dimension, dimension, &data);
            file.close();
        }
    }
};

#endif //  _FILE_SET_HPP_
