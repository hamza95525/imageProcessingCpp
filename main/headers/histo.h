//
// Created by khamzat on 5/11/20.
//

#ifndef MAIN_HISTO_H
#define MAIN_HISTO_H

#include <iostream>
#include <random>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RED cv::Scalar(255, 0, 0)
#define BLUE cv::Scalar(0, 0, 255)
#define GREEN cv::Scalar(0, 255, 0)
#define WHITE cv::Scalar(255, 255, 255)

class histogram {
    cv::Mat inImg;

    int nWidth;
    int nKlass;
    double stdDev;

    std::vector<std::vector<int>> gray;
    std::vector<int> histoGray;

    std::vector<std::vector<int>> blue;
    std::vector<std::vector<int>> green;
    std::vector<std::vector<int>> red;

    std::vector<int> histoBlue;
    std::vector<int> histoGreen;
    std::vector<int> histoRed;


    std::vector<double> dyst;
    std::vector<int> LUT;

public:
    histogram(const cv::Mat& inImg, double stdDev, int nKlass);

    void takeRGB();
    void rgb2gray();
    void randomNumberDistribution();
    void createLUT();

    cv::Mat equalizeMono();
    cv::Mat equalizeRGB();
};


#endif //MAIN_HISTO_H
