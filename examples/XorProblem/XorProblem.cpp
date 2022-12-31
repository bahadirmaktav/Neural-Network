#include <iostream>
#include "NeuralNetwork.hpp"

int main(int, char**) {
    try {
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