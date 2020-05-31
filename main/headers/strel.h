//
// Created by khamzat on 5/26/20.
//

#ifndef MAIN_STREL_H
#define MAIN_STREL_H

#include <math.h>
#include <iostream>
#include <algorithm>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#define PI 3.14159265359

class strel{
    int x1;
    int x2;
    int y1;
    int y2;
    double angle;

public:
    int length;
    std::vector<std::vector<bool>> structuralElement;

public:
    strel();
    strel(int length, double angle);

public:
    void showX2Y2(){
        std::cout<<x2<<std::endl<<y2<<std::endl;
    }
    void showLine(){
        for (int i = 0; i < structuralElement.size(); ++i) {
            for(int j = 0; j < structuralElement[i].size(); ++j){
                std::cout << structuralElement[i][j];
            }
            std::cout<<std::endl;
        }
    }
};

#endif //MAIN_STREL_H
