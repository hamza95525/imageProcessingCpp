//
// Created by khamzat on 5/15/20.
//

#include "../headers/imopen.h"

#include <utility>

imopen::imopen(cv::Mat inImg, double angle, int length, const strel& SE, const std::string& type)
{
    if(type == "bin"){
        //todo
        outImg = erosionBin(std::move(inImg), SE);
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

cv::Mat imopen::binarization(cv::Mat inImg) {
    cv::Mat localOutImg;
    inImg.copyTo(localOutImg);
    cv::Vec3b color;

    for(int i = 0; i<inImg.rows; ++i){
        for(int j = 0; j<inImg.cols; ++j){
            color = inImg.at<cv::Vec3b>(i, j);
            for(int x = 0; x<3; x++){
                color[x] = (color[x] >= 127) ? 255 : 0;
            }

            localOutImg.at<cv::Vec3b>(i, j) = color;
        }

    }

    return localOutImg;
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
    cv::Mat outImg;
    outImg = binarization(inImg);

    cv::Vec3b color;
    int halfLength = (SE.length-1)/2;

    for(int i = halfLength; i<inImg.rows - halfLength; ++i){
        for(int j = halfLength; j<inImg.cols - halfLength; ++j){
            std::cout<<outImg.at<cv::Vec3b>(i, j)<<" ";
            for(int k = i - halfLength, x = 0; k<=i+halfLength; k++, x++){
                for(int l = j - halfLength, y = 0; l <= j+halfLength; l++, y++){
                    color = outImg.at<cv::Vec3b>(k, l);
                    if(color[0] == 0 && color[1] == 0 && color[2] == 0 && SE.structuralElement[x][y] == 1){

                        for(int m = i - halfLength; m<=i+halfLength; m++){
                            for(int n = j - halfLength; n <= j+halfLength; n++){
                                color[0] = 0; color[1] = 0; color[2] = 0;
                                outImg.at<cv::Vec3b>(m, n) = color;
                            }
                        }
                    }
                }
            }

        }
        std::cout<<"\n";
    }

    return outImg;
}

cv::Mat imopen::dilateBin(const cv::Mat& inImg, const strel& SE) {
    cv::Mat outImg;
    outImg = binarization(inImg);

    cv::Vec3b color;
    int halfLength = (SE.length-1)/2;

    for(int i = halfLength; i<inImg.rows - halfLength; ++i){
        for(int j = halfLength; j<inImg.cols - halfLength; ++j){

            for(int k = i - halfLength, x = 0; k<=i+halfLength; k++, x++){
                for(int l = j - halfLength, y = 0; l <= j+halfLength; l++, y++){
                    color = outImg.at<cv::Vec3b>(k, l);
                    if(color[0] == 255 && color[1] == 255 && color[2] == 255){

                        for(int m = i - halfLength; m<=i+halfLength; m++){
                            for(int n = j - halfLength; n <= j+halfLength; n++){
                                color[0] = 255; color[1] = 255; color[2] = 255;
                                outImg.at<cv::Vec3b>(m, n) = color;
                            }
                        }
                        break;
                    }
                }

            }
        }
    }
    return outImg;
}

cv::Mat imopen::erosionMono(cv::Mat inImg, const strel& SE) {
    cv::Mat outImg;
    inImg.copyTo(outImg);

    cv::Vec3b color;
    double gray = 0;

    int halfLength = (SE.length-1)/2;


    for(int i = halfLength; i<inImg.rows - halfLength; ++i){
        for(int j = halfLength; j<inImg.cols - halfLength; ++j){
            std::vector<std::vector<double>> neighborsGray;
            std::vector<std::vector<cv::Vec3b>> neighborsBGR;

            for(int k = i - halfLength, x = 0; k<=i+halfLength; k++, x++){
                std::vector<double> tempGray;
                std::vector<cv::Vec3b> tempBGR;
                for(int l = j - halfLength, y = 0; l <= j+halfLength; l++, y++){
                    color = inImg.at<cv::Vec3b>(k, l);
                    gray = color[0]*0.114 + color[1]*0.587 + color[2]*0.299;
                    tempGray.push_back(gray);
                    tempBGR.push_back(color);
                }
                neighborsGray.push_back(tempGray);
                neighborsBGR.push_back(tempBGR);
            }

            std::vector<int> coords = minCoords(neighborsGray);

            for(int k = i - halfLength, x = 0; k<=i+halfLength; k++, x++){
                for(int l = j - halfLength, y = 0; l <= j+halfLength; l++, y++){
                    if(SE.structuralElement[x][y] == 1)
                        outImg.at<cv::Vec3b>(k, l) = neighborsBGR[coords[0]][coords[1]];
                }
            }
        }
    }

    return outImg;
}

cv::Mat imopen::dilateMono(cv::Mat inImg, const strel& SE) {
    cv::Mat outImg;
    inImg.copyTo(outImg);

    cv::Vec3b color;
    double gray = 0;

    int halfLength = (SE.length-1)/2;


    for(int i = halfLength; i<inImg.rows - halfLength; ++i){
        for(int j = halfLength; j<inImg.cols - halfLength; ++j){
            std::vector<std::vector<double>> neighborsGray;
            std::vector<std::vector<cv::Vec3b>> neighborsBGR;

            for(int k = i - halfLength, x = 0; k<=i+halfLength; k++, x++){
                std::vector<double> tempGray;
                std::vector<cv::Vec3b> tempBGR;
                for(int l = j - halfLength, y = 0; l <= j+halfLength; l++, y++){
                    color = inImg.at<cv::Vec3b>(k, l);
                    gray = color[0]*0.114 + color[1]*0.587 + color[2]*0.299;
                    tempGray.push_back(gray);
                    tempBGR.push_back(color);
                }
                neighborsGray.push_back(tempGray);
                neighborsBGR.push_back(tempBGR);
            }

            std::vector<int> coords = maxCoords(neighborsGray);

            for(int k = i - halfLength, x = 0; k<=i+halfLength; k++, x++){
                for(int l = j - halfLength, y = 0; l <= j+halfLength; l++, y++){
                    if(SE.structuralElement[x][y] == 1)
                        outImg.at<cv::Vec3b>(k, l) = neighborsBGR[coords[0]][coords[1]];
                }
            }
        }
    }

    return outImg;
}
