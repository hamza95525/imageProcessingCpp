//
// Created by khamzat on 6/5/20.
//
#include "../headers/entropyfilt.h"

entropyfilt::entropyfilt(const cv::Mat &inImg, int range, const std::string& type)
    : range(range), type(type)
{
    if(type == "Mono")
        outImg = filtGray(inImg);
    else if(type == "RGB")
        outImg = filtRGB(inImg);
    else
        std::cout << "Nalezy wprowadzic typ jako RGB lub Mono!\n";
}

cv::Mat entropyfilt::filtGray(const cv::Mat &inImg) {
    cv::Mat localOutImg = cv::Mat::zeros(inImg.rows, inImg.cols, CV_8UC3);

    double entropy = 0;
    double propabilityN = 0;
    std::vector<std::vector<double>> entropyMatrix;

    for(int i = 0; i<inImg.rows; i++){
        std::vector<double> temp;
        for(int j=0; j<inImg.cols; j++){
            temp.push_back(0);
        }
        entropyMatrix.push_back(temp);
    }

    double maxEntropy = -100;

    for(int i = 0; i<inImg.rows; i++) {
        for (int j = 0; j < inImg.cols; j++) {
             int *imgHisto = new int[256];
             int x = 0;
             int y = 0;

              for(int k = 0; k<= range; k++){
                  for (int l = 0; l <= range; l++){
                      x = i + k;
                      y = j + l;

                      cv::Vec3b color = inImg.at<cv::Vec3b>(x, y);
                      imgHisto[ (int)( color[0]*0.114 + color[1]*0.587 + color[2]*0.299) ]++;
                  }
              }

              entropy = 0;
              propabilityN = 0;
              for (int n = 0; n < 256; n++){
                  if ( imgHisto[n] > 0){
                      propabilityN = (double)imgHisto[n] / (double)(range*range);
                      entropy += - ( (log2(propabilityN) * propabilityN) );
                  }
              }

              entropyMatrix[i][j] = entropy;
              maxEntropy = (entropy > maxEntropy) ? entropy : maxEntropy;
        }
    }

    for(int i = 0; i<inImg.rows; i++){
        for(int j = 0; j<inImg.cols; j++){
            cv::Vec3b color;
            color[0] =  ( (entropyMatrix[i][j] ) / (maxEntropy) ) * 255;
            color[1] =  ( (entropyMatrix[i][j] ) / (maxEntropy) ) * 255;
            color[2] =  ( (entropyMatrix[i][j] ) / (maxEntropy) ) * 255;
            localOutImg.at<cv::Vec3b>(i, j) = color;
        }
    }
    return localOutImg;
}

cv::Mat entropyfilt::filtRGB(const cv::Mat &inImg) {
    cv::Mat localOutImg = cv::Mat::zeros(inImg.rows, inImg.cols, CV_8UC3);

    double entropyB = 0;
    double entropyG = 0;
    double entropyR = 0;

    double propabilityB = 0;
    double propabilityG = 0;
    double propabilityR = 0;

    std::vector<std::vector<double>> entropyMatrixB;
    std::vector<std::vector<double>> entropyMatrixG;
    std::vector<std::vector<double>> entropyMatrixR;

    for(int i = 0; i<inImg.rows; i++){
        std::vector<double> tempB;
        std::vector<double> tempG;
        std::vector<double> tempR;
        for(int j=0; j<inImg.cols; j++){
            tempB.push_back(0);
            tempG.push_back(0);
            tempR.push_back(0);
        }
        entropyMatrixB.push_back(tempB);
        entropyMatrixG.push_back(tempG);
        entropyMatrixR.push_back(tempR);
    }

    double maxEntropyB = -100;
    double maxEntropyG = -100;
    double maxEntropyR = -100;


    for(int i = 0; i<inImg.rows; i++) {
        for (int j = 0; j < inImg.cols; j++) {
            int *imgHistoB = new int[256];
            int *imgHistoG = new int[256];
            int *imgHistoR = new int[256];

            int x = 0;
            int y = 0;

            for (int k = 0; k <= range; k++) {
                for (int l = 0; l <= range; l++) {
                    x = i + k;
                    y = j + l;

                    cv::Vec3b color = inImg.at<cv::Vec3b>(x, y);
                    imgHistoB[ color[0] ]++;
                    imgHistoG[ color[1] ]++;
                    imgHistoR[ color[2] ]++;
                }
            }

            entropyB = 0; entropyG = 0; entropyR = 0;
            propabilityB = 0; propabilityG = 0; propabilityR = 0;
            for (int n = 0; n < 256; n++){
                if ( imgHistoB[n] > 0){
                    propabilityB = (double)imgHistoB[n] / (double)(range*range);
                    entropyB += - ( (log2(propabilityB) * propabilityB)/8 );
                }
                if ( imgHistoG[n] > 0){
                    propabilityG = (double)imgHistoG[n] / (double)(range*range);
                    entropyG += - ( (log2(propabilityG) * propabilityG)/8 );
                }
                if ( imgHistoR[n] > 0){
                    propabilityR = (double)imgHistoR[n] / (double)(range*range);
                    entropyR += - ( (log2(propabilityR) * propabilityR)/8 );
                }
            }

            entropyMatrixB[i][j] = entropyB;
            entropyMatrixG[i][j] = entropyG;
            entropyMatrixR[i][j] = entropyR;

            maxEntropyB = (entropyB > maxEntropyB) ? entropyB : maxEntropyB;
            maxEntropyG = (entropyG > maxEntropyG) ? entropyG : maxEntropyG;
            maxEntropyR = (entropyR > maxEntropyR) ? entropyR : maxEntropyR;

        }
    }

    for(int i = 0; i<inImg.rows; i++){
        for(int j = 0; j<inImg.cols; j++){
            cv::Vec3b color;
            color[0] =  ( (entropyMatrixB[i][j] ) / (maxEntropyB) ) * 255;
            color[1] =  ( (entropyMatrixG[i][j] ) / (maxEntropyG) ) * 255;
            color[2] =  ( (entropyMatrixR[i][j] ) / (maxEntropyR) ) * 255;
            localOutImg.at<cv::Vec3b>(i, j) = color;
        }
    }

    return localOutImg;
}