#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <iostream>
#include <functional>

class Matrix {
public:
    int rowNum;
    int colNum;
    double ** data;
public:
    Matrix(int iRowNum, int iColNum) {
        rowNum = iRowNum;
        colNum = iColNum;
        data = new double*[rowNum];
        for(int i = 0; i < rowNum; i++) {
            data[i] = new double[colNum]{0};
        }
    }
    ~Matrix() {
        for(int i = 0; i < rowNum; i++) {
            delete[] data[i];
        }
        delete[] data;
    }
    void PrintMatrix() {
        for(int i = 0; i < rowNum; i++) {
            for(int j = 0; j < colNum; j++) {
                std::cout << data[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    // Operator overloading functions returns new Matrix except '='.
    void operator = (const Matrix & iMatrix) {
        if(iMatrix.rowNum != rowNum || iMatrix.colNum != colNum) {
            throw std::runtime_error("Error : You cannot equalize matrices with different dimensions!");
        }
        for(int i = 0; i < rowNum; i++) {
            for(int j = 0; j < colNum; j++) {
                data[i][j] = iMatrix.data[i][j];
            }
        }
    }
    Matrix & operator + (const Matrix & iMatrix) {
        if(iMatrix.rowNum != rowNum || iMatrix.colNum != colNum) {
            throw std::runtime_error("Error : You cannot add matrices with different dimensions!");
        }
        Matrix * res = new Matrix(rowNum, colNum);
        for(int i = 0; i < rowNum; i++) {
            for(int j = 0; j < colNum; j++) {
                res->data[i][j] = data[i][j] + iMatrix.data[i][j];
            }
        }
        return *res;
    }
    Matrix & operator - (const Matrix & iMatrix) {
        if(iMatrix.rowNum != rowNum || iMatrix.colNum != colNum) {
            throw std::runtime_error("Error : You cannot subtract matrices with different dimensions!");
        }
        Matrix * res = new Matrix(rowNum, colNum);
        for(int i = 0; i < rowNum; i++) {
            for(int j = 0; j < colNum; j++) {
                res->data[i][j] = data[i][j] - iMatrix.data[i][j];
            }
        }
        return *res;
    }
    Matrix & operator * (const Matrix & iMatrix) {
        if(colNum != iMatrix.rowNum) {
            throw std::runtime_error("Error : First matrix column number must equal to second matrix row number to multiply these matrices!");
        }
        Matrix * res = new Matrix(rowNum, iMatrix.colNum);
        for(int i = 0; i < rowNum; i++) {
            for(int j = 0; j < iMatrix.colNum; j++) {
                for(int c = 0; c < iMatrix.rowNum; c++) {
                    res->data[i][j] += (data[i][c] * iMatrix.data[c][j]); 
                }
            }
        }
        return *res;
    }
    Matrix & operator * (const double & iConst) {
        Matrix * res = new Matrix(rowNum, colNum);
        for(int i = 0; i < rowNum; i++) {
            for(int j = 0; j < colNum; j++) {
                res->data[i][j] = data[i][j] * iConst;
            }
        }
        return *res;
    }

    // Below functions changes the current Matrix object.
    void Map(std::function<double(double)> handlerFn) {
        for(int i = 0; i < rowNum; i++) {
            for(int j = 0; j < colNum; j++) {
                data[i][j] = handlerFn(data[i][j]);
            }
        }
    }
};

#endif // MATRIX_HPP_