//
// Created by khamzat on 5/15/20.
//

#include "../headers/imopen.h"

#include <utility>

imopen::imopen(double angle, double length) :
    x1(0), y1(0), angle(angle),length(length)
{
    x2 = round( x1 + cos(angle*PI / 180.0 ) * length );
    y2 = round( y1 + sin( angle*PI/180.0 ) * length );

    std::cout << "( " << x1 << ", " << y1 << " )" << " - ( " << x2 << ", " << y2 << " )\n";
    for(int x = x1; x <= x2; ++x){
        std::vector<bool> temp;
        for(int y = y1; y <= y2; ++y){
            temp.push_back(false);
        }
        structuralElement.push_back(temp);
    }
}

void imopen::bresenhamsLineAlgorithm() {
    int d, dx, dy, ai, bi, xi, yi;
    int x = x1, y = y1;

    dx = (x1 < x2) ? (x2 - x1) : (x1 - x2);
    xi = (x1 < x2) ? 1 : -1;

    dy = (y1 < y2) ? (y2 - y1) : (y1 - y2);
    yi = (y1 < y2) ? 1 : -1;

    structuralElement[x][y] = true;

    if(dx > dy){
        ai = (dy - dx) * 2;
        bi = dy * 2;
        d = bi - dx;

        while(x != x2){
            if(d>=0){
                x += xi;
                y += yi;
                d += ai;
            } else {
                d += bi;
                x += xi;
            }
            structuralElement[x][y] = true;
        }
    } else {
        ai = (dx - dy) * 2;
        bi = dx * 2;
        d = bi - dy;

        while(y != y2){
            if(d>=0){
                x += xi;
                y += yi;
                d += ai;
            } else {
                d += bi;
                y += yi;
            }
            structuralElement[x][y] = true;
        }
    }
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

cv::Mat imopen::erosionBin(cv::Mat inImg) {
    cv::Mat outImg;
    inImg.copyTo(outImg);

    cv::Vec3b color;
    int halfLength = (length-1)/2;

    for(int i = halfLength; i<inImg.rows - halfLength; ++i){
        for(int j = halfLength; j<inImg.cols - halfLength; ++j){

            for(int k = i - halfLength, x = 0; k<=i+halfLength; k++, x++){
                for(int l = j - halfLength, y = 0; l <= j+halfLength; l++, y++){
                    color = inImg.at<cv::Vec3b>(k, l);
                    if(color[0] == 0 && color[1] == 0 && color[2] == 0 && structuralElement[x][y] == 1){

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
    }

    return outImg;
}

cv::Mat imopen::dilateBin(cv::Mat inImg) {
    cv::Mat outImg;
    inImg.copyTo(outImg);

    cv::Vec3b color;
    int halfLength = (length-1)/2;

    for(int i = halfLength; i<inImg.rows - halfLength; ++i){
        for(int j = halfLength; j<inImg.cols - halfLength; ++j){

            for(int k = i - halfLength, x = 0; k<=i+halfLength; k++, x++){
                for(int l = j - halfLength, y = 0; l <= j+halfLength; l++, y++){
                    color = inImg.at<cv::Vec3b>(k, l);
                    if(color[0] != 0 && color[1] != 0 && color[2] != 0){

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

cv::Mat imopen::openBin(cv::Mat inImg) {
    cv::Mat outImg = erosionBin(std::move(inImg));
    outImg = dilateBin(outImg);

    return outImg;
}

cv::Mat imopen::erosionMono(cv::Mat inImg) {
    cv::Mat outImg;
    inImg.copyTo(outImg);

    cv::Vec3b color;
    double gray = 0;

    int halfLength = (length-1)/2;


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
                    if(structuralElement[x][y] == 1)
                        outImg.at<cv::Vec3b>(k, l) = neighborsBGR[coords[0]][coords[1]];
                }
            }
        }
    }

    return outImg;
}

cv::Mat imopen::dilateMono(cv::Mat inImg) {
    cv::Mat outImg;
    inImg.copyTo(outImg);

    cv::Vec3b color;
    double gray = 0;

    int halfLength = (length-1)/2;


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
                    if(structuralElement[x][y] == 1)
                        outImg.at<cv::Vec3b>(k, l) = neighborsBGR[coords[0]][coords[1]];
                }
            }
        }
    }

    return outImg;
}

cv::Mat imopen::openMono(cv::Mat inImg) {
    cv::Mat outImg = erosionMono(std::move(inImg));
    outImg = dilateMono(outImg);

    return outImg;
}