//
// Created by khamzat on 5/11/20.
//

#include "../headers/histo.h"

histogram::histogram(const cv::Mat& inImg, double stdDev, int nKlass)
    : inImg(inImg), nWidth(inImg.cols), stdDev(stdDev), nKlass(nKlass) {
    //todo
}

void histogram::rgb2gray() {
    cv::Vec3b color;

    for(int i = 0; i<inImg.rows; i++){
        std::vector<int> temp;
        for(int j = 0; j<inImg.cols; j++){
            color = inImg.at<cv::Vec3b>(i, j);
            temp.push_back(color[0]*0.114 + color[1]*0.587 + color[2]*0.299);
        }
        gray.push_back(temp);
    }

    std::vector<int> arr(256);
    for(int i = 0; i<inImg.rows; i++){
        for(int j = 0; j<inImg.cols; j++){
            arr[gray[i][j]]++;
        }
    }

    histoGray = arr;
}


void histogram::takeRGB() {
    cv::Vec3b color;

    for(int i = 0; i<inImg.rows; i++){
        std::vector<int> tempBlue;
        std::vector<int> tempGreen;
        std::vector<int> tempRed;
        for(int j = 0; j<inImg.cols; j++){
            color = inImg.at<cv::Vec3b>(i, j);

            tempBlue.push_back(color[0]);
            tempGreen.push_back(color[1]);
            tempRed.push_back(color[2]);
        }
        blue.push_back(tempBlue);
        green.push_back(tempGreen);
        red.push_back(tempRed);
    }


    std::vector<int> arrB(256);
    std::vector<int> arrG(256);
    std::vector<int> arrR(256);
    for(int i = 0; i<inImg.rows; i++){
        for(int j = 0; j<inImg.cols; j++){
            arrB[blue[i][j]]++;
            arrG[green[i][j]]++;
            arrR[red[i][j]]++;
        }
    }

    histoBlue = arrB;
    histoGreen = arrG;
    histoRed = arrR;
}


void histogram::randomNumberDistribution() {
    int N = 255;

    for (int i = 0; i < N; i++)
        dyst.push_back(exp(-1 * pow((double)i - 127, 2) / (2. * pow(stdDev, 2))) / (stdDev * sqrt(2. * 3.1415)) );

    for (int i = 1; i < N; i++)
        dyst[i] += (dyst[i - 1] >= 1) ? 1 : dyst[i - 1];

}

void histogram::createLUT() {
    int n = 0;

    for(int i = 0; i<256; i++){

        if( dyst[i] >= (n+1) / (double)nKlass ){
            n++;
            if( n >= nKlass )
                n = nKlass - 1;
        }
        LUT.push_back( (int)((double)((256-1)*n)/(double)(nKlass-1)) );
    }
}

cv::Mat histogram::equalizeMono() {
    cv::Mat outImg = cv::Mat::zeros(inImg.rows, inImg.cols, CV_8UC3);
    cv::Vec3b color;

    for(int i = 0; i<outImg.rows; i++){
        for(int j = 0; j<outImg.cols; j++){
            gray[i][j] = LUT[gray[i][j]];

            color[0] = gray[i][j];
            color[1] = gray[i][j];
            color[2] = gray[i][j];

            outImg.at<cv::Vec3b>(i, j) = color;
        }
    }

    return outImg;
}

cv::Mat histogram::equalizeRGB(){
    cv::Mat outImg = cv::Mat::zeros(inImg.rows, inImg.cols, CV_8UC3);
    cv::Vec3b color;

    for(int i = 0; i<outImg.rows; i++){
        for(int j = 0; j<outImg.cols; j++){
            blue[i][j] = LUT[blue[i][j]];
            green[i][j] = LUT[green[i][j]];
            red[i][j] = LUT[red[i][j]];

            color[0] = blue[i][j];
            color[1] = green[i][j];
            color[2] = red[i][j];

            outImg.at<cv::Vec3b>(i, j) = color;
        }
    }

    return outImg;
}