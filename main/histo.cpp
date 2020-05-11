//
// Created by khamzat on 5/11/20.
//

#include "histo.h"

histo::histo(int size, int width, int height, float rangeBegin, float rangeEnd) : histSize(size), histWidth(width), histHeight(height){
    //constructor
    range[0] = rangeBegin; range[1] = rangeEnd;
    bin_w = cvRound((double ) histWidth/histSize);

    cv::Mat histIm(histHeight, histWidth, CV_8UC3, cv::Scalar(0,0,0));
    histImage = histIm;
}

void histo::calculate(std::vector<cv::Mat> &color) {
    cv::calcHist( &color[0], 1, 0, cv::Mat(), bHist, 1, &histSize, &histRange, uniform, accumulate); //blue
    cv::calcHist( &color[1], 1, 0, cv::Mat(), gHist, 1, &histSize, &histRange, uniform, accumulate); //green
    cv::calcHist( &color[2], 1, 0, cv::Mat(), rHist, 1, &histSize, &histRange, uniform, accumulate); //red
}

void histo::normalize() {
    cv::normalize(bHist, bHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(gHist, gHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(rHist, rHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
}

cv::Mat histo::drawHisto() {
    for(int i = 1; i<histSize; ++i){
        cv::line(histImage, cv::Point(bin_w * (i-1), histHeight - cvRound(bHist.at<float>(i-1))),
                 cv::Point(bin_w*(i), histHeight - cvRound(bHist.at<float>(i))),
                 cv::Scalar(255, 0, 0), 2, 8, 0);

        cv::line(histImage, cv::Point(bin_w * (i-1), histHeight - cvRound(gHist.at<float>(i-1))),
                 cv::Point(bin_w*(i), histHeight - cvRound(gHist.at<float>(i))),
                 cv::Scalar(0, 255, 0), 2, 8, 0);

        cv::line(histImage, cv::Point(bin_w * (i-1), histHeight - cvRound(rHist.at<float>(i-1))),
                 cv::Point(bin_w*(i), histHeight - cvRound(rHist.at<float>(i))),
                 cv::Scalar(0, 0, 255), 2, 8, 0);
    }

    return histImage;
}