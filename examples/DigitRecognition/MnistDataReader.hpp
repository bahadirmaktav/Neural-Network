#ifndef MNIST_DATA_READER_HPP_
#define MNIST_DATA_READER_HPP_

#include <math.h>

#include <string>
#include <fstream>
#include <iostream>

#define MNIST MnistDataReader::Instance()

struct MnistData {
    int rowNum;
    int colNum;
    double * data;
    int label;
    MnistData()
    : rowNum(28)
    , colNum(28)
    , data(nullptr)
    , label(-1) 
    {}
    void PrintMnistData() {
        if(data == nullptr) {
            std::cout << "You cannot print the MNIST sample with empty data!" << std::endl;
        }
        std::cout << "Label : " << label << std::endl;
        std::cout << "Gray Scale Image Matrix :" << std::endl;
        for(int i = 0; i < rowNum; i++) {
            for(int j = 0; j < colNum; j++) {
                int buff = (data[colNum * i + j] * 255);
                if(buff < 10) {
                    std::cout << buff << "   ";
                } else if(buff < 100) {
                    std::cout << buff << "  ";                    
                } else {
                    std::cout << buff << " ";
                }
            }
            std::cout << "\n";
        }
    }
};

class MnistDataReader {
public:
    static MnistDataReader & Instance() {
        static MnistDataReader mnistReader;
        return mnistReader;
    }
    MnistDataReader(const MnistDataReader &) = delete;
    MnistDataReader & operator = (const MnistDataReader &) = delete;
    MnistData ** Read(const std::string & imageFilename, const std::string & labelFilename, int numberOfReads) {
        std::ifstream imageFile(imageFilename, imageFile.binary);
        std::ifstream labelFile(labelFilename, imageFile.binary);
        if(imageFile.good() && labelFile.good()) {
            int iMagicNumber = 0;
            int iNumOfImages = 0;
            int iNumOfRows = 0;
            int iNumOfCols = 0;
            imageFile.read((char *)&iMagicNumber, sizeof(iMagicNumber));
            iMagicNumber = ReverseLittleToBigEndian(iMagicNumber);
            imageFile.read((char *)&iNumOfImages, sizeof(iNumOfImages));
            iNumOfImages = ReverseLittleToBigEndian(iNumOfImages);
            imageFile.read((char *)&iNumOfRows, sizeof(iNumOfRows));
            iNumOfRows = ReverseLittleToBigEndian(iNumOfRows);
            imageFile.read((char *)&iNumOfCols, sizeof(iNumOfCols));
            iNumOfCols = ReverseLittleToBigEndian(iNumOfCols);
            int lMagicNumber = 0;
            int lNumOfImages = 0;
            labelFile.read((char *)&lMagicNumber, sizeof(lMagicNumber));
            lMagicNumber = ReverseLittleToBigEndian(lMagicNumber);
            labelFile.read((char *)&lNumOfImages, sizeof(lNumOfImages));
            lNumOfImages = ReverseLittleToBigEndian(lNumOfImages);
            int imagePixelSize = iNumOfRows * iNumOfCols;
            MnistData ** imageList = new MnistData*[numberOfReads];
            for(int k = 0; k < numberOfReads; k++) {
                MnistData * image = new MnistData();
                image->data = new double[imagePixelSize];
                unsigned char buffer = 0;
                labelFile.read((char *)&buffer, sizeof(buffer));
                image->label = buffer;
                image->rowNum = iNumOfRows;
                image->colNum = iNumOfCols;
                for(int i = 0; i < imagePixelSize; i++) {
                    unsigned char pixelBuffer = 0;
                    imageFile.read((char *)&pixelBuffer, sizeof(pixelBuffer));
                    image->data[i] = (double)(255 - pixelBuffer) / 255;
                }
                imageList[k] = image;
            }
            return imageList;
        } else {
            throw std::runtime_error("ERROR : Image file or label file could not be opened.");
        }
    }
private:
    MnistDataReader() {}
    ~MnistDataReader() {}
    int ReverseLittleToBigEndian(int val) {
        unsigned char c1 = val & 255;
        unsigned char c2 = (val >> 8) & 255;
        unsigned char c3 = (val >> 16) & 255;
        unsigned char c4 = (val >> 24) & 255;
        return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + ((int)c4);
    }
};

#endif // MNIST_DATA_READER_HPP_