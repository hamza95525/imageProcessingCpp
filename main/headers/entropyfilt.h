//
// Created by khamzat on 6/5/20.
//

#ifndef MAIN_ENTROPYFILT_H
#define MAIN_ENTROPYFILT_H
#include <math.h>
#include <iostream>
#include <algorithm>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "../headers/strel.h"

class entropyfilt{
    int range;
    std::string type;
    cv::Mat outImg;


public:
    entropyfilt(const cv::Mat& inImg, int range, const std::string& type);
    cv::Mat returnOut(){
        return outImg;
    }

private:
    cv::Mat filtGray(const cv::Mat & inImg);
    cv::Mat filtRGB(const cv::Mat& inImg);
};

#endif //MAIN_ENTROPYFILT_H
