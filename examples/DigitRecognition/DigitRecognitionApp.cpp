#include <iostream>
#include <iomanip>
#include "MnistDataReader.hpp"

int main() {
    auto loadedMnistData = MNIST.Read("train-images.idx3-ubyte", "train-labels.idx1-ubyte", 60000);
    loadedMnistData[1111]->PrintMnistData();
}