#include <iostream>
#include "NeuralNetwork.hpp"

int main(int, char**) {
    try {
    /* Matrix functions tests :
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
    */
    /* Random number generator tests :
        NeuralNetwork nnSigmoidTest(2,3,2);
        std::cout << "Sigmoid(0.6) = " << nnSigmoidTest.Sigmoid(0.6) << std::endl;
        srand((unsigned int)time(NULL));
        for(int i = 0; i < 20; i++) {
            std::cout << "Random value : " << (rand() / (double)RAND_MAX) * (1 - (-1)) + (-1) << std::endl;
        }
    */
    /*
        NeuralNetwork nn(2,3,2);
        Matrix inputMatrix(2, 1);
        inputMatrix.data[0][0] = 0.52; inputMatrix.data[1][0] = 0.73;
        Matrix outputMatrix = nn.FeedForward(inputMatrix);
        std::cout << "Output Matrix : " << std::endl;
        outputMatrix.PrintMatrix();
    */
    /*
        Matrix m6(2, 3);
        m6.data[0][0] = 5; m6.data[0][1] = 4; m6.data[0][2] = -7;
        m6.data[1][0] = 2; m6.data[1][1] = -5; m6.data[1][2] = 9;
        Matrix m7 = m6.Transpose();
        std::cout << "m7 matrix : " << std::endl;
        m7.PrintMatrix();
        Matrix m8(3, 2);
        m8.data[0][0] = 5; m8.data[0][1] = 4;
        m8.data[1][0] = -7; m8.data[1][1] = 5;
        m8.data[2][0] = 5; m8.data[2][1] = -2;
        std::cout << "m8 matrix : " << std::endl;
        m8.PrintMatrix();
        std::cout << "m7 .* m8 = : " << std::endl;
        m8.ElementWiseMultiply(m7).PrintMatrix();
    */
    /*    
        NeuralNetwork nn(2,3,2);
        Matrix inputMatrix(2, 1);     Matrix labeledMatrix(2, 1);
        inputMatrix.data[0][0] = 1;   labeledMatrix.data[0][0] = 1;
        inputMatrix.data[1][0] = 0;   labeledMatrix.data[1][0] = 0;
        nn.Train(inputMatrix, labeledMatrix);
    */
        NeuralNetwork neuralNetwork(2, 2, 1);
        Matrix m00(2, 1); m00.data[0][0] = 0; m00.data[1][0] = 0;
        Matrix m01(2, 1); m01.data[0][0] = 0; m01.data[1][0] = 1;
        Matrix m10(2, 1); m10.data[0][0] = 1; m10.data[1][0] = 0;
        Matrix m11(2, 1); m11.data[0][0] = 1; m11.data[1][0] = 1;
        Matrix inputMatrixArray[4] = { m00, m01, m10, m11 };
        Matrix m00l(1, 1); m00l.data[0][0] = 0;
        Matrix m01l(1, 1); m01l.data[0][0] = 1;
        Matrix m10l(1, 1); m10l.data[0][0] = 1;
        Matrix m11l(1, 1); m11l.data[0][0] = 0;
        Matrix labelMatrixArray[4] = { m00l, m01l, m10l, m11l };
        std::cout << "Untrained Neural Network Infos : " << std::endl;
        std::cout << "bias hidden matrix : " << std::endl;
        neuralNetwork.biasHidden_.PrintMatrix();
        std::cout << "bias output matrix : " << std::endl;
        neuralNetwork.biasOutput_.PrintMatrix();
        std::cout << "weight hidden to output matrix : " << std::endl;
        neuralNetwork.weightMatrixHO_.PrintMatrix();
        std::cout << "weight input to hidden matrix : " << std::endl;
        neuralNetwork.weightMatrixIH_.PrintMatrix();
        std::cout << "\nEnter the number of train sample!\n";
        int trainSampleNum = 10;
        std::cin >> trainSampleNum;
        std::cout << "Training started!\n";
        srand((unsigned int)time(NULL));
        int counter = 1;
        for(int i = 0; i < trainSampleNum; i++) {
            int randomInd = (rand() / (double)RAND_MAX) * 4;
            randomInd %= 4;
            neuralNetwork.Train(inputMatrixArray[randomInd], labelMatrixArray[randomInd]);
            if(counter % 1000000 == 0) {
                std::cout << "Trained Sample Number : " << counter << std::endl;
            }
            counter++;
        }
        std::cout << "Training finished!\n\n";
        std::cout << "Trained Neural Network Infos : " << std::endl;
        std::cout << "bias hidden matrix : " << std::endl;
        neuralNetwork.biasHidden_.PrintMatrix();
        std::cout << "bias output matrix : " << std::endl;
        neuralNetwork.biasOutput_.PrintMatrix();
        std::cout << "weight hidden to output matrix : " << std::endl;
        neuralNetwork.weightMatrixHO_.PrintMatrix();
        std::cout << "weight input to hidden matrix : " << std::endl;
        neuralNetwork.weightMatrixIH_.PrintMatrix();

        Matrix inputMatrix(2, 1); 
        std::cout << "\nOutput guess of the trained neural network for (0,0) is : ";
        inputMatrix.data[0][0] = 0; inputMatrix.data[1][0] = 0;
        neuralNetwork.FeedForward(inputMatrix).PrintMatrix();

        std::cout << "\nOutput guess of the trained neural network for (0,1) is : ";
        inputMatrix.data[0][0] = 0; inputMatrix.data[1][0] = 1;
        neuralNetwork.FeedForward(inputMatrix).PrintMatrix();

        std::cout << "\nOutput guess of the trained neural network for (1,0) is : ";
        inputMatrix.data[0][0] = 1; inputMatrix.data[1][0] = 0;
        neuralNetwork.FeedForward(inputMatrix).PrintMatrix();

        std::cout << "\nOutput guess of the trained neural network for (1,1) is : ";
        inputMatrix.data[0][0] = 1; inputMatrix.data[1][0] = 1;
        neuralNetwork.FeedForward(inputMatrix).PrintMatrix();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }    
}