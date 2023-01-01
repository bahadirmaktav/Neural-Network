#ifndef NEURAL_NETWORK_HPP_
#define NEURAL_NETWORK_HPP_

#include <math.h>

#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

#include "Matrix.hpp"

class NeuralNetwork {
public:
    NeuralNetwork(int iInputLayerLen, int iHiddenLayerLen, int iOutputLayerLen)
    : learningRate_(0.1)
    , inputMatrix_(iInputLayerLen, 1)
    , hiddenMatrix_(iHiddenLayerLen, 1)
    , outputMatrix_(iOutputLayerLen, 1)
    , weightMatrixIH_(iHiddenLayerLen, iInputLayerLen)
    , weightMatrixHO_(iOutputLayerLen, iHiddenLayerLen)
    , biasOutput_(iOutputLayerLen, 1)
    , biasHidden_(iHiddenLayerLen, 1) {
        weightMatrixIH_.RandomiseElements(-1, 1);
        weightMatrixHO_.RandomiseElements(-1, 1);
        // biasOutput_.RandomiseElements(0.1, 0.5);
        // biasHidden_.RandomiseElements(0.1, 0.5);
        biasOutput_.MapToCurrentMatrix([](double val) -> double {
            return 1;
        });
        biasHidden_.MapToCurrentMatrix([](double val) -> double {
            return 1;
        });
        sigmoidFn_ = [](double val) -> double {
            return 1 / (1 + exp(-1 * val));
        };
        diffOfSigmoidFn_ = [&](double val) -> double {
            return sigmoidFn_(val) * (1 - sigmoidFn_(val));
        };
    }
    NeuralNetwork(const std::string & iFilename) : NeuralNetwork(1, 1, 1) {
        LoadModel(iFilename);
    }
    ~NeuralNetwork() {}
    Matrix & FeedForward(Matrix & inputMatrix) {
        // INPUT VALIDITY CONTROL
        if(inputMatrix.rowNum != inputMatrix_.rowNum || inputMatrix.colNum != inputMatrix_.colNum) {
            throw std::runtime_error("Error : Input matrix dimensions does not match with the neural network input layer!");
        }
        // INPUT TO HIDDEN PART
        hiddenMatrix_ = ((weightMatrixIH_ * inputMatrix) + biasHidden_).MapToNewMatrix(sigmoidFn_);
        // HIDDEN TO OUTPUT PART
        outputMatrix_ = ((weightMatrixHO_ * hiddenMatrix_) + biasOutput_).MapToNewMatrix(sigmoidFn_);
        // RETURN OUTPUT
        return outputMatrix_;
    }
    void Train(Matrix & inputMatrix, Matrix & labeledMatrix) {
        // INPUT VALIDITY CONTROL
        if(inputMatrix.rowNum != inputMatrix_.rowNum || inputMatrix.colNum != inputMatrix_.colNum) {
            throw std::runtime_error("Error : Input matrix dimensions does not match with the neural network input layer!");
        }
        if(labeledMatrix.rowNum != outputMatrix_.rowNum || labeledMatrix.colNum != outputMatrix_.colNum) {
            throw std::runtime_error("Error : Labeled matrix dimensions does not match with the neural network output layer!");
        }
        // GET OUTPUT MATRIX AND NOT SIGMOIDED OUTPUT MATRIX
        Matrix zHidden = ((weightMatrixIH_ * inputMatrix) + biasHidden_); 
        hiddenMatrix_ = zHidden.MapToNewMatrix(sigmoidFn_);
        Matrix zOutput = ((weightMatrixHO_ * hiddenMatrix_) + biasOutput_);
        outputMatrix_ = zOutput.MapToNewMatrix(sigmoidFn_);
        // HIDDEN ERROR MATRIX
        Matrix errorMatrix = (outputMatrix_ - labeledMatrix);
        Matrix hiddenErrorMatrix = weightMatrixHO_.Transpose().MeanByColumns() * errorMatrix;
        // BACKPROPAGATION - OUTPUT TO HIDDEN
        Matrix msDeltaMatrixOH = errorMatrix.ElementWiseMultiply(zOutput.MapToNewMatrix(diffOfSigmoidFn_));
        Matrix deltaWeightMatrixOH = (msDeltaMatrixOH * hiddenMatrix_.Transpose()) * learningRate_ * -1;
        Matrix deltaBiasMatrixO = msDeltaMatrixOH * learningRate_ * -1;
        // BACKPROPAGATION - HIDDEN TO INPUT
        Matrix msDeltaMatrixHI = hiddenErrorMatrix.ElementWiseMultiply(zHidden.MapToNewMatrix(diffOfSigmoidFn_));
        Matrix deltaWeightMatrixHI = (msDeltaMatrixHI * inputMatrix.Transpose()) * learningRate_ * -1;
        Matrix deltaBiasMatrixH = msDeltaMatrixHI * learningRate_ * -1;
        // ADJUST WEIGHTS
        weightMatrixHO_ = weightMatrixHO_ + deltaWeightMatrixOH;
        weightMatrixIH_ = weightMatrixIH_ + deltaWeightMatrixHI;
        biasOutput_ = biasOutput_ + deltaBiasMatrixO;
        biasHidden_ = biasHidden_ + deltaBiasMatrixH;
    }

    void SaveModel(const std::string & iFilename) {
        std::ofstream modelFile(iFilename, modelFile.trunc);
        if(modelFile.good()) {
            modelFile << std::setw(4) << BuildModelJson();
        } else {
            std::cout << "WARNING : Model could not be saved." << std::endl;
        }
    }
    void LoadModel(const std::string & iFilename) {
        std::ifstream modelFile(iFilename);
        if(modelFile.good()) {
            std::stringstream ssBuff;
            ssBuff << modelFile.rdbuf();
            std::string modelJsonStr = ssBuff.str();
            ParseModelJson(modelJsonStr);
        } else {
            std::cout << "WARNING : Model could not be loaded." << std::endl;
        }
    }

private:
    json BuildModelJson() {
        json model;
        // LAYER LENGTHS
        model["Layer"]["InputLayerLength"] = inputMatrix_.rowNum;
        model["Layer"]["HiddenLayerLength"] = hiddenMatrix_.rowNum;
        model["Layer"]["OutputLayerLength"] = outputMatrix_.rowNum;
        // WEIGHTS ARRAYS
        json ihWeights = json::array();
        auto ihWeightsHandlerFn = [&](double val) {
            ihWeights.push_back(val);
        };
        weightMatrixIH_.IterateByRow(ihWeightsHandlerFn);
        json hoWeights = json::array();
        auto hoWeightsHandlerFn = [&](double val) {
            hoWeights.push_back(val);
        };
        weightMatrixHO_.IterateByRow(hoWeightsHandlerFn);
        model["Weight"]["InputToHidden"] = ihWeights;
        model["Weight"]["HiddenToOutput"] = hoWeights;
        // BIASES ARRAYS
        json hBiases = json::array();
        auto hBiasesHandlerFn = [&](double val) {
            hBiases.push_back(val);
        };
        biasHidden_.IterateByRow(hBiasesHandlerFn);
        json oBiases = json::array();
        auto oBiasesHandlerFn = [&](double val) {
            oBiases.push_back(val);
        };
        biasOutput_.IterateByRow(oBiasesHandlerFn);
        model["Bias"]["Hidden"] = hBiases;
        model["Bias"]["Output"] = oBiases;
        // LEARNING RATE
        model["LearningRate"] = learningRate_;
        // RETURNING MODEL JSON
        return model;
    }
    void ParseModelJson(const std::string & modelJsonStr) {
        json model = json::parse(modelJsonStr);
        // GET LAYER LENGTHS
        int inputLayerLength = model["Layer"]["InputLayerLength"];
        int hiddenLayerLength = model["Layer"]["HiddenLayerLength"];
        int outputLayerLength = model["Layer"]["OutputLayerLength"];
        // GET LEARNING RATE
        learningRate_ = model["LearningRate"];
        // GET WEIGHTS
        int ihWeightArrLen = model["Weight"]["InputToHidden"].size();
        int hoWeightArrLen = model["Weight"]["HiddenToOutput"].size();
        double * ihWeightArr = new double[ihWeightArrLen];
        double * hoWeightArr = new double[hoWeightArrLen];
        json ihWeightArrJson = model["Weight"]["InputToHidden"];
        json hoWeightArrJson = model["Weight"]["HiddenToOutput"];
        int indexCounter = 0;
        for(auto it = ihWeightArrJson.begin(); it != ihWeightArrJson.end(); it++, indexCounter++) {
            ihWeightArr[indexCounter] = it.value();
        }
        indexCounter = 0;
        for(auto it = hoWeightArrJson.begin(); it != hoWeightArrJson.end(); it++, indexCounter++) {
            hoWeightArr[indexCounter] = it.value();
        }
        // GET BIASES
        int hBiasArrLen = model["Bias"]["Hidden"].size();
        int oBiasArrLen = model["Bias"]["Output"].size();
        double * hBiasArr = new double[hBiasArrLen];
        double * oBiasArr = new double[oBiasArrLen];
        json hBiasArrJson = model["Bias"]["Hidden"];
        json oBiasArrJson = model["Bias"]["Output"];
        indexCounter = 0;
        for(auto it = hBiasArrJson.begin(); it != hBiasArrJson.end(); it++, indexCounter++) {
            hBiasArr[indexCounter] = it.value();
        }
        indexCounter = 0;
        for(auto it = oBiasArrJson.begin(); it != oBiasArrJson.end(); it++, indexCounter++) {
            oBiasArr[indexCounter] = it.value();
        }
        // RELAOD WEIGHTS
        weightMatrixIH_.ReloadWithArray(hiddenLayerLength, inputLayerLength, ihWeightArr, ihWeightArrLen);
        weightMatrixHO_.ReloadWithArray(outputLayerLength, hiddenLayerLength, hoWeightArr, hoWeightArrLen);
        // RELOAD BIASES
        biasHidden_.ReloadWithArray(hiddenLayerLength, 1, hBiasArr, hBiasArrLen);
        biasOutput_.ReloadWithArray(outputLayerLength, 1, oBiasArr, oBiasArrLen);
        // RELOAD INPUT/HIDDEN/OUTPUT MATRICES
        double * inputArr = new double[inputLayerLength]{0};
        double * hiddenArr = new double[hiddenLayerLength]{0};
        double * outputArr = new double[outputLayerLength]{0};
        inputMatrix_.ReloadWithArray(inputLayerLength, 1, inputArr, inputLayerLength);     // TODO : Giving same value twice? Better approach needed.
        hiddenMatrix_.ReloadWithArray(hiddenLayerLength, 1, hiddenArr, hiddenLayerLength);
        outputMatrix_.ReloadWithArray(outputLayerLength, 1, outputArr, outputLayerLength);
    }

// TODO : Later change to private.
public:
    double learningRate_;
    Matrix inputMatrix_;
    Matrix hiddenMatrix_;
    Matrix outputMatrix_;
    Matrix weightMatrixIH_;  // Input to Hidden
    Matrix weightMatrixHO_;  // Hidden to Output
    Matrix biasOutput_;
    Matrix biasHidden_;
    std::function<double(double)> sigmoidFn_;
    std::function<double(double)> diffOfSigmoidFn_;
};

#endif // NEURAL_NETWORK_HPP_