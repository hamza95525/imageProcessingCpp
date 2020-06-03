//
// Created by khamzat on 5/11/20.
//

#ifndef MAIN_HISTO_H
#define MAIN_HISTO_H

#include <iostream>
#include <random>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define PI 3.14159265359


class histogram {
    cv::Mat inImg;
    cv::Mat outImg;

    int nWidth;
    int nKlass;
    double stdDev;
    std::string type;

    std::vector<std::vector<double>> gray;
    std::vector<int> histoGray;

    std::vector<std::vector<double>> blue;
    std::vector<std::vector<double>> green;
    std::vector<std::vector<double>> red;

    std::vector<int> histoBlue;
    std::vector<int> histoGreen;
    std::vector<int> histoRed;

    std::vector<double> dyst;
    std::vector<double> LUT;

public:
    histogram(const cv::Mat& inImg, double stdDev, int nKlass, std::string type);

    cv::Mat returnOut(){
        return outImg;
    };

private:
    void takeRGB();
    void rgb2gray();
    void randomNumberDistribution();
    void createLUT();

    cv::Mat equalizeMono();
    cv::Mat equalizeRGB();
};


#endif //MAIN_HISTO_H
