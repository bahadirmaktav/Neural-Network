#include <iostream>
#include "MnistDataReader.hpp"
#include "NeuralNetwork.hpp"

void TrainDigitRecognitionModel();
double TestDigitRecognitionModel();
void TrainDigitRecognitionModelRandomise();

void TrainDigitRecognitionModel() {
    NeuralNetwork neuralNetwork(784, 20, 10);
    int setLen = 60000;
    auto loadedMnistData = MNIST.Read("train-images.idx3-ubyte", "train-labels.idx1-ubyte", setLen);

    std::cout << "Train process started." << std::endl;
    for(int i = 0; i < setLen; i++) {
        Matrix inputMatrix(1, 1);
        inputMatrix.ReloadWithArray(784, 1, loadedMnistData[i]->data, 784);
        Matrix labeledMatrix(1, 1);
        double labelArr[10]{0};
        labelArr[loadedMnistData[i]->label] = 1;
        labeledMatrix.ReloadWithArray(10, 1, labelArr, 10);

        // loadedMnistData[i]->PrintMnistData();

        neuralNetwork.Train(inputMatrix, labeledMatrix);
        if((i + 1) % 4000 == 0) {
            // std::cout << "Train process : %" << (double)i / setLen * 100 << std::endl;
            neuralNetwork.SaveModel("Digit_Recognition_Model_01.json");
            std::cout << "Neural Network model saved in 'Digit_Recognition_Model_01.json'." << std::endl;
            std::cout << "Train process : %" << (double)i / setLen * 100 << ", Error Rate : " << TestDigitRecognitionModel() << std::endl;
        }
    }
    std::cout << "Train process finished." << std::endl;
    neuralNetwork.SaveModel("Digit_Recognition_Model_01.json");
    std::cout << "Neural Network model saved in 'Digit_Recognition_Model_01.json'." << std::endl;
}

double TestDigitRecognitionModel() {
    NeuralNetwork neuralNetwork("Digit_Recognition_Model_01.json");
    // std::cout << "Neural Network model Digit_Recognition_Model_01.json loaded." << std::endl;
    int setLen = 10000;
    auto loadedMnistData = MNIST.Read("t10k-images.idx3-ubyte", "t10k-labels.idx1-ubyte", setLen);

    // std::cout << "Testing process started." << std::endl;
    int wrongGuessCounter = 0;
    for(int k = 0; k < setLen; k++) {
        Matrix inputMatrix(1, 1);
        inputMatrix.ReloadWithArray(784, 1, loadedMnistData[k]->data, 784);
        Matrix & outputMatrix = neuralNetwork.FeedForward(inputMatrix);
        int maxValInd = 0; double maxVal = outputMatrix.data[0][0];
        // std::cout << "Ouput Matrix :\n"; 
        // outputMatrix.PrintMatrix();
        for(int m = 0; m < 10; m++) {
            if(outputMatrix.data[m][0] > maxVal) {
                maxValInd = m;
                maxVal = outputMatrix.data[m][0];
            }
        }
        if(loadedMnistData[k]->label != maxValInd) {
            wrongGuessCounter++;
        }
        // if((k + 1) % 4000 == 0) {
        //     std::cout << "Testing process : %" << (double)k / setLen * 100 << ", Error Rate : %" << (double)wrongGuessCounter / setLen * 100 << std::endl; 
        // }
    }
    // std::cout << "Testing process finished." << std::endl;
    // std::cout << "Final Error Rate : %" << (double)wrongGuessCounter / setLen * 100 << std::endl;
    return (double)wrongGuessCounter / setLen * 100;
}

void TrainDigitRecognitionModelRandomise() {
    NeuralNetwork neuralNetwork(784, 64, 10);
    // neuralNetwork.LoadModel("Digit_Recognition_Model_01.json");
    // std::cout << "Neural Network model Digit_Recognition_Model_01.json loaded." << std::endl;
    int setLen = 60000; int trainLen = 300000;
    auto loadedMnistData = MNIST.Read("train-images.idx3-ubyte", "train-labels.idx1-ubyte", setLen);

    srand((unsigned int)time(NULL));
    std::cout << "Train process started." << std::endl;
    int i = 0;
    /* for(int i = 0; i < trainLen; i++) */ while(1) {
        int randomSample = (rand() / (double)RAND_MAX) * (setLen - 1);
        if(randomSample < 0 || randomSample >= setLen) {
            std::cout << "randomSample : " << randomSample << std::endl;
            throw std::runtime_error("randomSample error!");
        }
        Matrix inputMatrix(1, 1);
        inputMatrix.ReloadWithArray(784, 1, loadedMnistData[randomSample]->data, 784);
        Matrix labeledMatrix(1, 1);
        double labelArr[10]{0};
        labelArr[loadedMnistData[randomSample]->label] = 1;
        labeledMatrix.ReloadWithArray(10, 1, labelArr, 10);

        // loadedMnistData[randomSample]->PrintMnistData();

        neuralNetwork.Train(inputMatrix, labeledMatrix);
        if((i + 1) % 5000 == 0) {
            neuralNetwork.SaveModel("Digit_Recognition_Model_01.json");
            // std::cout << "Neural Network model saved in 'Digit_Recognition_Model_01.json'." << std::endl;    
            // std::cout << "Train process : %" << (double)i / trainLen * 100 << std::endl;
            std::cout << "Train process : Number of sample = " << i << ", Error Rate : " << TestDigitRecognitionModel() << std::endl;
        }
        i++;
    }
    std::cout << "Train process finished." << std::endl;
    neuralNetwork.SaveModel("Digit_Recognition_Model_01.json");
    std::cout << "Neural Network model saved in 'Digit_Recognition_Model_01.json'." << std::endl;    
}

void TrainTestModel() {
    NeuralNetwork neuralNetwork(784, 64, 10);
    // neuralNetwork.LoadModel("Digit_Recognition_Model_01.json");
    // std::cout << "Neural Network model Digit_Recognition_Model_01.json loaded." << std::endl;
    int setTrainLen = 60000; int setTestLen = 10000; int trainLen = 300000;
    auto loadedTrainMnistData = MNIST.Read("train-images.idx3-ubyte", "train-labels.idx1-ubyte", setTrainLen);

    srand((unsigned int)time(NULL));
    std::cout << "Train process started." << std::endl;
    for(int i = 0; i < trainLen; i++) {
        int randomSample = (rand() / (double)RAND_MAX) * (setTrainLen - 1);
        if(randomSample < 0 || randomSample >= setTrainLen) {
            std::cout << "randomSample : " << randomSample << std::endl;
            throw std::runtime_error("randomSample error!");
        }
        Matrix inputMatrix(1, 1);
        inputMatrix.ReloadWithArray(784, 1, loadedTrainMnistData[randomSample]->data, 784);
        Matrix labeledMatrix(1, 1);
        double labelArr[10]{0};
        labelArr[loadedTrainMnistData[randomSample]->label] = 1;
        labeledMatrix.ReloadWithArray(10, 1, labelArr, 10);
        neuralNetwork.Train(inputMatrix, labeledMatrix);
        if((i + 1) % 5000 == 0) {
            neuralNetwork.SaveModel("Digit_Recognition_Model_01.json");
            std::cout << "Neural Network model saved in 'Digit_Recognition_Model_01.json'." << std::endl;    
            std::cout << "Train process : %" << (double)i / trainLen * 100 << std::endl;
            TestDigitRecognitionModel();
        }
    }
    std::cout << "Train process finished." << std::endl;
    neuralNetwork.SaveModel("Digit_Recognition_Model_01.json");
    std::cout << "Neural Network model saved in 'Digit_Recognition_Model_01.json'." << std::endl;    
}

int main() {
    try {
        TrainDigitRecognitionModel();
        // TrainDigitRecognitionModelRandomise();
        // TestDigitRecognitionModel();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}