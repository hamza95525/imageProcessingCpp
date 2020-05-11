//
// Created by khamzat on 5/11/20.
//

#ifndef MAIN_HISTO_H
#define MAIN_HISTO_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class histo {
private:
    int histSize;
    int histWidth;
    int histHeight;
    int bin_w;
    float range[2];
    const float* histRange = {range}; //range of values (0..255)
    bool uniform = true;
    bool accumulate = false;

    cv::Mat bHist, gHist, rHist;
    cv::Mat histImage;

public:
    histo(int size, int width, int height, float rangeBegin, float rangeEnd);
    void calculate(std::vector<cv::Mat> &color);
    void normalize();
    cv::Mat drawHisto();
};


#endif //MAIN_HISTO_H
