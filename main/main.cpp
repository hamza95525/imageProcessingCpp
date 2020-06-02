#include <iostream>
#include "headers/histo.h"
#include "headers/imopen.h"
#include "headers/strel.h"
#include "headers/hitmiss.h"


int main() {
    std::string name1;
    std::cout << "Prosze wprowadzic sciezke do pliku: ";
    std::cin >> name1;
    cv::Mat img = cv::imread(name1);

    if(!img.data) {
        std::cout << "Nie odnaleziono pliku!" << name1 << std::endl;
        return -1;
    }

    int n_Klass = 0;
    double stdDev = 0;

    std::cout<<"Prosze wprowadzic odchylenie standardowe do rozkladu gaussa oraz ilosc klas: ";
    std::cin>>stdDev;
    std::cin>>n_Klass;

    histogram Histogram(img, stdDev, n_Klass);
    Histogram.rgb2gray(img);
    Histogram.randomNumberDistribution();
    Histogram.createLUT();
    cv::Mat out = Histogram.equalizeMono();

    cv::namedWindow("in", cv::WINDOW_AUTOSIZE);
    cv::imshow("in", img);
    cv::namedWindow("out", cv::WINDOW_AUTOSIZE);
    cv::imshow("out", out);

    cv::waitKey(0);
    return 0;
}


/*
 *     //WYPUKLE OTOCZENIE
    std::vector<std::vector<int>> SE1 = {
            {255, 255, -1},
            {255, 0 , -1},
            {255, -1, 0}};
    std::vector<std::vector<int>> SE2 = {
            {255, 255, 255},
            {255, 0, -1},
            {255, 0, -1}};

    cv::Mat out =  hitmiss::hitOrMiss(img, SE2);

    cv::namedWindow("in", cv::WINDOW_AUTOSIZE);
    cv::imshow("in", img);
    cv::namedWindow("out", cv::WINDOW_AUTOSIZE);
    cv::imshow("out", out);

 * */