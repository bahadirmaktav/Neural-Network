#ifndef NEURAL_NETWORK_HPP_
#define NEURAL_NETWORK_HPP_

#include <math.h>

#include "Matrix.hpp"

class NeuralNetwork {
public:
    NeuralNetwork(int iInputLayerLen, int iHiddenLayerLen, int iOutputLayerLen)
    : hiddenMatrix_(iHiddenLayerLen, 1)
    // , inputMatrix_(iInputLayerLen, 1)
    , outputMatrix_(iOutputLayerLen, 1)
    , weightMatrixIH_(iHiddenLayerLen, iInputLayerLen)
    , weightMatrixHO_(iOutputLayerLen, iHiddenLayerLen)
    , biasOutput_(iOutputLayerLen, 1)
    , biasHidden_(iHiddenLayerLen, 1) {
        weightMatrixIH_.RandomiseElements(-1 , 1);
        weightMatrixHO_.RandomiseElements(-1 , 1);
        biasOutput_.RandomiseElements(-1 , 1);
        biasHidden_.RandomiseElements(-1 , 1);
        sigmoidFn_ = [](double val) -> double {
            return 1 / (1 + exp(-1 * val));
        };
    }
    ~NeuralNetwork() {}
    Matrix & FeedForward(const Matrix & inputMatrix) {
        // INPUT TO HIDDEN PART
        hiddenMatrix_ = ((weightMatrixIH_ * inputMatrix) + biasHidden_).MapToNewMatrix(sigmoidFn_);
        // HIDDEN TO OUTPUT PART
        outputMatrix_ = ((weightMatrixHO_ * hiddenMatrix_) + biasOutput_).MapToNewMatrix(sigmoidFn_);
        // RETURN OUTPUT
        return outputMatrix_;
    }
    void Train(const Matrix & inputMatrix, const Matrix & labeledMatrix) {
        
    }

    // TODO : Testing purposes remove later.
    double Sigmoid(double val) {
        return 1 / (1 + exp(-1 * val));
    }

private:
    Matrix hiddenMatrix_;
    // Matrix inputMatrix_;
    Matrix outputMatrix_;
    Matrix weightMatrixIH_;  // Input to Hidden
    Matrix weightMatrixHO_;  // Hidden to Output
    Matrix biasOutput_;
    Matrix biasHidden_;
    std::function<double(double)> sigmoidFn_;
};

#endif //  NEURAL_NETWORK_HPP_