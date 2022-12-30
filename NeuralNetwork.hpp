#ifndef NEURAL_NETWORK_HPP_
#define NEURAL_NETWORK_HPP_

#include <math.h>

#include "Matrix.hpp"

class NeuralNetwork {
public:
    NeuralNetwork(int iInputLayerLen, int iHiddenLayerLen, int iOutputLayerLen)
    : learningRate_(0.1)
    // , inputMatrix_(iInputLayerLen, 1)
    , hiddenMatrix_(iHiddenLayerLen, 1)
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
        diffOfSigmoidFn_ = [&](double val) -> double {
            return sigmoidFn_(val) * (1 - sigmoidFn_(val));
        };
    }
    ~NeuralNetwork() {}
    Matrix & FeedForward(Matrix & inputMatrix) {
        // INPUT TO HIDDEN PART
        hiddenMatrix_ = ((weightMatrixIH_ * inputMatrix) + biasHidden_).MapToNewMatrix(sigmoidFn_);
        // HIDDEN TO OUTPUT PART
        outputMatrix_ = ((weightMatrixHO_ * hiddenMatrix_) + biasOutput_).MapToNewMatrix(sigmoidFn_);
        // RETURN OUTPUT
        return outputMatrix_;
    }
    void Train(Matrix & inputMatrix, Matrix & labeledMatrix) {
        // GET OUTPUT MATRIX AND NOT SIGMOIDED OUTPUT MATRIX
        Matrix zHidden = ((weightMatrixIH_ * inputMatrix) + biasHidden_); 
        hiddenMatrix_ = zHidden.MapToNewMatrix(sigmoidFn_);
        Matrix zOutput = ((weightMatrixHO_ * hiddenMatrix_) + biasOutput_);
        outputMatrix_ = zOutput.MapToNewMatrix(sigmoidFn_);
        // HIDDEN ERROR MATRIX
        Matrix errorMatrix = (outputMatrix_ - labeledMatrix);
        Matrix hiddenErrorMatrix = weightMatrixHO_.Transpose().MeanByColumns() * errorMatrix;
        // std::cout << "hiddenErrorMatrix : " << std::endl;
        // weightMatrixHO_.Transpose().MeanByColumns().PrintMatrix();
        // BACKPROPAGATION - OUTPUT TO HIDDEN
        Matrix msDeltaMatrixOH = errorMatrix.ElementWiseMultiply(zOutput.MapToNewMatrix(diffOfSigmoidFn_));
        Matrix deltaWeightMatrixOH = (msDeltaMatrixOH * hiddenMatrix_.Transpose()) * learningRate_ * -1;
        Matrix deltaBiasMatrixO = msDeltaMatrixOH * learningRate_ * -1;
        // BACKPROPAGATION - HIDDEN TO INPUT
        Matrix msDeltaMatrixHI = hiddenErrorMatrix.ElementWiseMultiply(zHidden.MapToNewMatrix(diffOfSigmoidFn_));
        Matrix deltaWeightMatrixHI = (msDeltaMatrixHI * inputMatrix.Transpose()) * learningRate_ * -1;
        Matrix deltaBiasMatrixH = msDeltaMatrixHI * learningRate_ * -1;
        // ADJUST WEIGHTS
        // std::cout << "deltaMatrixOH : " << std::endl;
        // deltaMatrixOH.PrintMatrix();
        // std::cout << "deltaMatrixHI : " << std::endl;
        // deltaMatrixHI.PrintMatrix();
        weightMatrixHO_ = weightMatrixHO_ + deltaWeightMatrixOH;
        weightMatrixIH_ = weightMatrixIH_ + deltaWeightMatrixHI;
        biasOutput_ = biasOutput_ + deltaBiasMatrixO;
        biasHidden_ = biasHidden_ + deltaBiasMatrixH;
    }

    // TODO : Testing purposes remove later.
    double Sigmoid(double val) {
        return 1 / (1 + exp(-1 * val));
    }

public:
    double learningRate_;
    // Matrix inputMatrix_;
    Matrix hiddenMatrix_;
    Matrix outputMatrix_;
    Matrix weightMatrixIH_;  // Input to Hidden
    Matrix weightMatrixHO_;  // Hidden to Output
    Matrix biasOutput_;
    Matrix biasHidden_;
    std::function<double(double)> sigmoidFn_;
    std::function<double(double)> diffOfSigmoidFn_;
};

#endif //  NEURAL_NETWORK_HPP_