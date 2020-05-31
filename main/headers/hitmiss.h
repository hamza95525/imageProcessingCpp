//
// Created by khamzat on 5/25/20.
//

#ifndef MAIN_HITMISS_H
#define MAIN_HITMISS_H

#include <math.h>
#include <iostream>
#include <algorithm>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

class hitmiss {
    //todo
    cv::Mat outImg;
    std::vector<std::vector<int>> SE1;
    std::vector<std::vector<int>> SE2;


public:
    hitmiss(cv::Mat inImg, const std::vector<std::vector<int>> &SE1, const std::vector<std::vector<int>> &SE2);

    cv::Mat customSum(const cv::Mat& imgOne, const cv::Mat& imgTwo);
    cv::Mat customOr(const cv::Mat& imgOne, const cv::Mat& imgTwo);
    static bool isEqual(const cv::Mat &imgOne, const cv::Mat &imgTwo);
    static void rotate(std::vector<std::vector<int>> &SE);
    cv::Mat hitOrMiss(const cv::Mat& inImg, const std::vector<std::vector<int>>& SE );
    cv::Mat convexHull(const cv::Mat& inImg);

    cv::Mat erosion(const cv::Mat& inImg, std::vector<std::vector<int>> SE);

    cv::Mat imComplement(const cv::Mat& inImg);
    cv::Mat binarization(cv::Mat inImg);

    void showSE(std::vector<std::vector<int>> SE){
        for(int i = 0; i<SE.size(); i++){
            for(int j = 0; j<SE[i].size(); j++){
                std::cout<<SE[i][j]<<" ";
            }
            std::cout<<"\n";
        }
    }
};

#endif //MAIN_HITMISS_H
