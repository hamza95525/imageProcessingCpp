//
// Created by khamzat on 5/11/20.
//

#include "../headers/histo.h"

histogram::histogram(const cv::Mat& inImg, double mean, double stdDev, int nKlass)
    : inImg(inImg), nWidth(inImg.cols), mean(mean), stdDev(stdDev), nKlass(nKlass) {
    //todo
    dyst.resize(256);
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
        }
    }

    histo = arr;

}

void histogram::randomNumberDistribution() {
    std::default_random_engine e;

    std::normal_distribution<double> distrN(mean, stdDev);

    for(int i=0; i<2*inImg.cols*inImg.rows; i++){
        int num = distrN(e);
        if(num >= 0 && num < 256)
            dyst[num]++;
    }

}

void histogram::createLUT() {
    int n = 0;

    for(int i = 0; i<256; i++){
        std::cout<<dyst[i] << "---" <<( (n+1)*inImg.rows*inImg.cols / (double)nKlass )<<"\n";

        if( dyst[i] >= (n+1)*inImg.rows*inImg.cols / (double)nKlass ){
            n++;
            if( n >= nKlass )
                n = nKlass - 1;
        }
        LUT.push_back( (int)((double)((256-1)*n)/(double)(nKlass-1)) );
    }

    for(int i = 0; i<256; i++)
        std::cout<<"LUT[" << i << "]" << LUT[i]<<std::endl;
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