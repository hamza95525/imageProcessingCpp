//
// Created by khamzat on 5/15/20.
//

#include "imopen.h"

#include <utility>

imopen::imopen(int angle, int length) :
    x1(0), y1(0), angle(angle),length(length)
{
    x2 = round ( x1 + cos(angle*PI / 180.0 ) * length );
    y2 = round ( y1 + sin( angle*PI/180.0 ) * length );

    for(int x = x1; x <= x2; ++x){
        std::vector<bool> temp;
        for(int y = y1; y <= y2; ++y){
            temp.push_back(false);
        }
        structuralElement.push_back(temp);
    }
}

void imopen::bresenhamsLineAlgorithm() {
    int m_new = 2 * (y2 - y1);
    int slope_error_new = m_new - (x2 - x1);

    for(int x = x1, y = y1; x <= x2; ++x){
        for(int i = x1; i <= x2; i++){
            for(int j = y1; j <= y2; j++){
                if(i == x && j == y) {
                    structuralElement[i][j] = true;
                }
            }
        }

        slope_error_new += m_new;
        if(slope_error_new >= 0){
            y++;
            slope_error_new -= 2 * (x2 - x1);
        }
    }
}

cv::Mat imopen::erosion(cv::Mat inImg) {
    std::vector<cv::Vec3b> neighbors;
    cv::Mat outImg;
    inImg.copyTo(outImg);

    cv::Vec3b color;
    int halfLength = (length-1)/2;
    bool include = true;

    for(int i = halfLength; i<inImg.rows - halfLength; ++i){
        for(int j = halfLength; j<inImg.cols - halfLength; ++j){

            for(int k = i - halfLength, x = 0; k<=i+halfLength; k++, x++){
                for(int l = j - halfLength, y = 0; l <= j+halfLength; l++, y++){
                    color = inImg.at<cv::Vec3b>(k, l);
                    if(color[0] == 0 && color[1] == 0 && color[2] == 0 && structuralElement[x][y] == 1){
                        include = false;
                        break;
                    }
                }
                if(!include)
                    break;
            }

            if(!include){
                for(int k = i - halfLength, x = 0; k<=i+halfLength; k++, x++){
                    for(int l = j - halfLength, y = 0; l <= j+halfLength; l++, y++){
                        if(structuralElement[x][y] == 1) {
                            color[0] = 0;
                            color[1] = 0;
                            color[2] = 0;
                            outImg.at<cv::Vec3b>(k, l) = color;
                        }
                    }
                }
            }
            include = true;
        }
    }

    return outImg;
}