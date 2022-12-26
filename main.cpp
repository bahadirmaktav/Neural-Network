#include <iostream>
#include "Matrix.hpp"

int main(int, char**) {
    try {
        Matrix m1(2, 3);
        Matrix m2(3, 4);
        Matrix m3(2, 4);

        m1.data[0][0] = 8; m1.data[0][1] = -1; m1.data[0][2] = 4;
        m1.data[1][0] = -5; m1.data[1][1] = 3; m1.data[1][2] = 6;
        std::cout << "Matrix m1 : " << std::endl;
        m1.PrintMatrix();

        m2.data[0][0] = 7; m2.data[0][1] = -4; m2.data[0][2] = 8; m2.data[0][3] = 7;
        m2.data[1][0] = -1; m2.data[1][1] = 7; m2.data[1][2] = -3; m2.data[1][3] = 3;
        m2.data[2][0] = 3; m2.data[2][1] = 2; m2.data[2][2] = 4; m2.data[2][3] = 2;
        std::cout << "Matrix m2 : " << std::endl;
        m2.PrintMatrix();

        std::cout << "Matrix m3 = m1 * m2 : " << std::endl;
        m3 = m1 * m2;
        m3.PrintMatrix();

        Matrix m4(2, 4);
        m4 = m3 * 5;
        std::cout << "Matrix m4 = m3 * 5 : " << std::endl;
        m4.PrintMatrix();

        std::function<double(double)> halfFn = [](double input) -> double {
            return input / 2;
        };
        m4.Map(halfFn);
        std::cout << "Matrix m4 = all elements divided by 2 : " << std::endl;
        m4.PrintMatrix();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }    
}