//
// Created by khamzat on 5/15/20.
//

#ifndef MAIN_IMOPEN_H
#define MAIN_IMOPEN_H

#include <math.h>
#include <iostream>
#include <vector>
#define PI 3.14159265

class imopen {
    int x1;
    int x2;
    int y1;
    int y2;
    int angle;
    int length;
    std::vector<std::vector<int>> structuralElement;

public:
    imopen(int angle, int length);
    void bresenhamsLineAlgorithm();
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


#endif //MAIN_IMOPEN_H
