//
// Created by khamzat on 5/11/20.
//

#include "../headers/histo.h"

histogram::histogram(cv::Mat inImg, double mean, double stdDev) : nWidth(inImg.cols), mean(mean), stdDev(stdDev) {
    //todo
    gauss.resize(nWidth);
}

void histogram::rgb2gray(const cv::Mat& inImg) {
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
            std::cout<<gray[i][j] << " ";
        }
        std::cout<<std::endl;
    }

    histo = arr;

    for(int i = 0; i<256; i++)
        std::cout<<"hist["<<i<<"] = "<< histo[i]<< "\n";
}

void histogram::randomNumberDistribution() {
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);

    std::normal_distribution<double> distrN(mean, stdDev);

    for(int i=0; i<800; i++){
        int num = distrN(e);
        if(num >= 0 && num < nWidth)
            gauss[num]++;
    }

    for(int i = 0; i<nWidth; i++)
        std::cout << ": " << std::string(gauss[i], '*') << std::endl;

}