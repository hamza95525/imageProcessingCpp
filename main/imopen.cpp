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
        std::vector<int> temp;
        for(int y = y1; y <= y2; ++y){
            temp.push_back(0);
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
                    structuralElement[i][j] = 1;
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
