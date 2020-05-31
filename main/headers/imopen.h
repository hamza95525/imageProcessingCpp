//
// Created by khamzat on 5/15/20.
//

#ifndef MAIN_IMOPEN_H
#define MAIN_IMOPEN_H

#include <math.h>
#include <iostream>
#include <algorithm>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "../headers/strel.h"
#define PI 3.14159265359

class imopen {
    cv::Mat outImg;

public:
    imopen(cv::Mat inImg, double angle, int length, const strel& SE, const std::string& type);
    imopen(cv::Mat inImg);

    cv::Mat erosionBin(const cv::Mat& inImg, const strel& SE);
    cv::Mat dilateBin(const cv::Mat& inImg, const strel& SE);

    cv::Mat erosionMono(cv::Mat inImg, const strel& SE);
    cv::Mat dilateMono(cv::Mat inImg, const strel& SE);

    cv::Mat returnOut(){
        return outImg;
    }

private:
    cv::Mat binarization(cv::Mat inImg);
    static std::vector<int> minCoords(std::vector<std::vector<double>> &grayArr);
    static std::vector<int> maxCoords(std::vector<std::vector<double>> &grayArr);
};


#endif //MAIN_IMOPEN_H
