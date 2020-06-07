#include <iostream>
#include "headers/histo.h"
#include "headers/imopen.h"
#include "headers/strel.h"
#include "headers/hitmiss.h"
#include "headers/entropyfilt.h"

cv::Mat menu(char nNumber, const cv::Mat& img);

int main() {
    std::string name1;
    std::cout << "Prosze wprowadzic sciezke do pliku: ";
    std::cin >> name1;
    cv::Mat img = cv::imread(name1, CV_32F);

    if(!img.data) {
        std::cout << "Nie odnaleziono pliku!" << name1 << std::endl;
        return -1;
    }

    char n;
    std::cout << "Prosze podac numer operacji do wykonania\n1.Wyrownanie histogramu\t2.Filtracja entropii\t3.Otwarcie\t4.Wypukle otoczenie\n";
    std::cin >> n;

    cv::Mat out = menu(n, img);

    cv::namedWindow("in", cv::WINDOW_AUTOSIZE);
    cv::imshow("in", img);

    cv::namedWindow("out", cv::WINDOW_AUTOSIZE);
    cv::imshow("out", out);

    cv::waitKey(0);
    return 0;
}

cv::Mat menu(char nNumber, const cv::Mat& img){
    cv::Mat out;

    switch(nNumber){
        case '1':{
            int n_Klass = 0;
            double stdDev = 0;
            std::string type;

            std::cout<<"Prosze wprowadzic typ obrazu(RGB/Mono), odchylenie standardowe do rozkladu gaussa oraz ilosc klas:\n";
            std::cin>>type;
            std::cin>>stdDev;
            std::cin>>n_Klass;

            histogram Histogram(img, stdDev, n_Klass, type);
            out = Histogram.returnOut();

            break;
        }

        case '2':{
            int range = 0;
            std::string type;

            std::cout<<"Prosze wprowadzic typ obrazu(RGB/Mono) oraz zasieg:\n";
            std::cin >> type;
            std::cin >> range;

            entropyfilt Entropia(img, range, type);
            out = Entropia.returnOut();

            break;
        }

        case '3':{
            int length = 0;
            double angle = 0;
            std::string type;

            std::cout << "Prosze wprowadzic typ obrazu(bin/mono) oraz kat i dlugosci elementu strukturalnego:\n";
            std::cin >> type;
            std::cin >> angle;
            std::cin >> length;

            strel SE(length, angle);
            imopen Otwarcie(img, angle, length, SE, type);
            out = Otwarcie.returnOut();
            break;
        }

        case '4':{
            std::vector<std::vector<int>> SE1 = {
                    {255, 255, -1},
                    {255, 0 , -1},
                    {255, -1, 0}};
            std::vector<std::vector<int>> SE2 = {
                    {255, 255, 255},
                    {255, 0, -1},
                    {255, 0, -1}};

            hitmiss HIT(img, SE1, SE2);
            out =  HIT.returnOut();
            break;
        }
        default:{
            std::cout << "Nalezy podac numer od 1 do 4!\n";
            break;
        }
    }

    return out;
}
