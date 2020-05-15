//
// Created by khamzat on 5/15/20.
//

#include "imopen.h"

imopen::imopen(int angle, int length) :
    x1(0), y1(0), angle(angle),length(length)
{
    x2 = round (x1 + cos(angle*PI / 180.0 ) * length );
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

void imopen::erosion(cv::Mat binImg) {
    std::vector<std::vector<bool> > neighbors;

    for(int i = 0; i<binImg.rows; ++i){
        for(int j = 0; j<binImg.cols; ++j){
            std::cout<< binImg.at<bool>(i, j) << " ";

            if(binImg.at<bool>(i, j)){
                for(auto & k : structuralElement){
                    std::vector<bool> temp;
                    for(int l = 0; l<k.size(); ++l){
                        //todo;
                    }
                }
            }
        }
        std::cout << std::endl;
    }
}