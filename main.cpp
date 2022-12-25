#include <iostream>
#include "Matrix.hpp"

int main(int, char**) {
    try {
        Matrix m1(4, 6);
        Matrix m2(4, 6);
        Matrix m3(4, 6);
        m1.data[0][0] = 1; m1.data[0][1] = 2; m1.data[3][2] = 3; m1.data[0][3] = 4; m1.data[0][4] = 5;
        m2.data[0][0] = 5; m2.data[0][1] = 2; m2.data[3][2] = 9; m2.data[0][3] = 4; m2.data[0][4] = 5;
        m3 = m1 + m2;
        m3 = m1 - m2;
        m3.PrintMatrix();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }    
}