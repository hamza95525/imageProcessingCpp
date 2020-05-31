//
// Created by khamzat on 5/26/20.
//

#include "../headers/strel.h"

strel::strel() : x1(0), y1(0), x2(0), y2(0), angle(0),length(0) {
    //default
}

strel::strel(int length, double angle) :
    x1(0), y1(0), angle(angle),length(length)
{

    x2 = round( x1 + cos(angle*PI / 180.0 ) * length );
    y2 = round( y1 + sin( angle*PI/180.0 ) * length );

    for(int x = x1; x <= x2; ++x){
        std::vector<bool> temp;
        for(int y = y1; y <= y2; ++y){
            temp.push_back(false);
        }
        structuralElement.push_back(temp);
    }

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
