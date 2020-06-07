//
// Created by khamzat on 5/15/20.
//

#include "../headers/imopen.h"

#include <utility>

imopen::imopen(cv::Mat inImg, double angle, int length, const strel& SE, const std::string& type)
{
    if(type == "bin"){
        //todo
        outImg = erosionBin(inImg, SE);
        outImg = dilateBin(outImg, SE);
    } else if(type == "mono"){
        //todo
        outImg = erosionMono(std::move(inImg), SE);
        outImg = dilateMono(outImg, SE);
    } else {
        std::cout << "Nalezy wprowadzic typ jako 'mono' lub 'bin'!\n ";
    }
}

imopen::imopen(cv::Mat inImg) : outImg(std::move(inImg)){
}

cv::Mat imopen::binarization(const cv::Mat& inImg) {
    cv::Mat locallocalOutImg;
    inImg.copyTo(locallocalOutImg);
    cv::Vec3b color;

    for(int i = 0; i<inImg.rows; ++i){
        for(int j = 0; j<inImg.cols; ++j){
            color = inImg.at<cv::Vec3b>(i, j);
            for(int x = 0; x<3; x++){
                color[x] = (color[x] >= 127) ? 255 : 0;
            }

            locallocalOutImg.at<cv::Vec3b>(i, j) = color;
        }

    }

    return locallocalOutImg;
}

std::vector<int> imopen::minCoords(std::vector<std::vector<double> > &grayArr) {
    int x = 0;
    int y = 0;
    double min = grayArr[x][y];

    for(int i = 0; i<grayArr.size(); ++i){
        for(int j = 0; j<grayArr[i].size(); ++j){
            if(min > grayArr[i][j]){
                min = grayArr[i][j];
                x = i;
                y = j;
            }
        }
    }
    std::vector<int> coordinates;
    coordinates.push_back(x);
    coordinates.push_back(y);

    return coordinates;
}

std::vector<int> imopen::maxCoords(std::vector<std::vector<double> > &grayArr) {
    int x = 0;
    int y = 0;
    double max = grayArr[x][y];

    for(int i = 0; i<grayArr.size(); ++i){
        for(int j = 0; j<grayArr[i].size(); ++j){
            if(max < grayArr[i][j]){
                max = grayArr[i][j];
                x = i;
                y = j;
            }
        }
    }
    std::vector<int> coordinates;
    coordinates.push_back(x);
    coordinates.push_back(y);

    return coordinates;
}

cv::Mat imopen::erosionBin(const cv::Mat& inImg, const strel& SE) {
    cv::Mat localOutImg;
    localOutImg = binarization(inImg);

    cv::Mat outcopy;
    localOutImg = binarization(inImg);
    localOutImg.copyTo(outcopy);
    
    cv::Vec3b color;

    for(int i = 1; i<inImg.rows - 1; ++i){
        for(int j = 1; j<inImg.cols - 1; ++j){
            int x =0;
            int y =0;

            for(int k = 0; k < SE.structuralElement.size(); k++){
                for(int l = 0; l < SE.structuralElement[k].size(); l++){
                    x = i + k - 1;
                    y = j + l - 1;
                    color = outcopy.at<cv::Vec3b>(x, y);

                    if(color[0] == 0 && color[1] == 0 && color[2] == 0 && SE.structuralElement[k][l] == 1){
                        color[0] = 0; color[1] = 0; color[2] = 0;
                        localOutImg.at<cv::Vec3b>(i, j) = color;
                    }
                }
            }
        }

    }

    return localOutImg;
}

cv::Mat imopen::dilateBin(const cv::Mat& inImg, const strel& SE) {
    cv::Mat localOutImg;
    localOutImg = binarization(inImg);

    cv::Mat outcopy;
    localOutImg = binarization(inImg);
    localOutImg.copyTo(outcopy);

    cv::Vec3b color;

    for(int i = 1; i<inImg.rows - 1; ++i){
        for(int j = 1; j<inImg.cols - 1; ++j){
            int x =0;
            int y =0;

            for(int k = 0; k < SE.structuralElement.size(); k++){
                for(int l = 0; l < SE.structuralElement[k].size(); l++){
                    x = i + k - 1;
                    y = j + l - 1;
                    color = outcopy.at<cv::Vec3b>(x, y);

                    if(color[0] == 255 && color[1] == 255 && color[2] == 255 ){
                        color[0] = 255; color[1] = 255; color[2] = 255;
                        localOutImg.at<cv::Vec3b>(i, j) = color;
                    }
                }
            }
        }

    }
    return localOutImg;
}

cv::Mat imopen::erosionMono(const cv::Mat& inImg, const strel& SE) {
    cv::Mat outImg;
    inImg.copyTo(outImg);

    cv::Vec3b color;
    double gray = 0;

    for(int i = 1; i<inImg.rows - 1; ++i){
        for(int j = 1; j<inImg.cols - 1; ++j){
            std::vector<std::vector<double>> neighborsGray;
            std::vector<std::vector<cv::Vec3b>> neighborsBGR;
            int x = 0;
            int y = 0;

            for(int k = 0; k< SE.structuralElement.size(); k++){
                std::vector<double> tempGray;
                std::vector<cv::Vec3b> tempBGR;
                for(int l = 0; l<SE.structuralElement[k].size(); l++){
                    x = i + k - 1;
                    y = j + l - 1;
                    color = inImg.at<cv::Vec3b>(x, y);
                    gray = color[0]*0.114 + color[1]*0.587 + color[2]*0.299;
                    tempGray.push_back(gray);
                    tempBGR.push_back(color);
                }
                neighborsGray.push_back(tempGray);
                neighborsBGR.push_back(tempBGR);
            }

            std::vector<int> coords = minCoords(neighborsGray);

            for(int k = 0; k< SE.structuralElement.size(); k++){
                for(int l = 0; l < SE.structuralElement[k].size(); l++){
                    if(SE.structuralElement[k][l] == 1)
                        outImg.at<cv::Vec3b>(i, j) = neighborsBGR[coords[0]][coords[1]];
                }
            }
        }
    }

    return outImg;
}

cv::Mat imopen::dilateMono(const cv::Mat& inImg, const strel& SE) {
    cv::Mat outImg;
    inImg.copyTo(outImg);

    cv::Vec3b color;
    double gray = 0;

    for(int i = 1; i<inImg.rows - 1; ++i){
        for(int j = 1; j<inImg.cols - 1; ++j){
            std::vector<std::vector<double>> neighborsGray;
            std::vector<std::vector<cv::Vec3b>> neighborsBGR;
            int x = 0;
            int y = 0;

            for(int k = 0; k< SE.structuralElement.size(); k++){
                std::vector<double> tempGray;
                std::vector<cv::Vec3b> tempBGR;
                for(int l = 0; l<SE.structuralElement[k].size(); l++){
                    x = i + k - 1;
                    y = j + l - 1;
                    color = inImg.at<cv::Vec3b>(x, y);
                    gray = color[0]*0.114 + color[1]*0.587 + color[2]*0.299;
                    tempGray.push_back(gray);
                    tempBGR.push_back(color);
                }
                neighborsGray.push_back(tempGray);
                neighborsBGR.push_back(tempBGR);
            }

            std::vector<int> coords = maxCoords(neighborsGray);

            for(int k = 0; k< SE.structuralElement.size(); k++){
                for(int l = 0; l < SE.structuralElement[k].size(); l++){
                    if(SE.structuralElement[k][l] == 1)
                        outImg.at<cv::Vec3b>(i, j) = neighborsBGR[coords[0]][coords[1]];
                }
            }
        }
    }

    return outImg;
}

