#include <iostream>
#include "histo.h"
#include "imopen.h"

int main() {
    std::string name;
    std::cout << "Prosze wprowadzic sciezke do pliku: ";
    std::cin >> name;
    cv::Mat img = cv::imread(name);

    if(!img.data) {
        std::cout << "Nie odnaleziono pliku!" << name << std::endl;
        return -1;
    }

    //=================HISTOGRAM===================
  /*
    std::vector<cv::Mat> BGR;
    cv::Vec3b color; //wydobycie koloru z obrazu
    std::vector<float> Red, Green, Blue;
    cv::split(img, BGR);
    histogram Histogram(256, 512, 256, 0, 256); //size, width, height, range = [0,256]
    Histogram.calculate(BGR);
    Histogram.normalize();
    Histogram.drawHisto();
    Histogram.show();*/
    //=============================================

    int length;
    int angle;
    std::cout << "Prosze wprowadzic dlugosc: ";
    std::cin >> length;
    std::cout << "Prosze wprowadzic nachylenie: ";
    std::cin >> angle;

    imopen Otwarcie(angle, length);
    Otwarcie.bresenhamsLineAlgorithm();
    // Otwarcie.showLine();
    cv::Mat erode = Otwarcie.erosion(img);
    cv::Mat dilate = Otwarcie.dilate(img);
    cv::Mat openned = Otwarcie.open(img);


    cv::namedWindow("wejsciowy", cv::WINDOW_AUTOSIZE);
    cv::imshow("wejsciowy", img);

    cv::namedWindow("wyjsciowyErozja", cv::WINDOW_AUTOSIZE);
    cv::imshow("wyjsciowyErozja", erode);

    cv::namedWindow("wyjsciowyDylacja", cv::WINDOW_AUTOSIZE);
    cv::imshow("wyjsciowyDylacja", dilate);

    cv::namedWindow("wyjsciowy", cv::WINDOW_AUTOSIZE);
    cv::imshow("wyjsciowy", openned);

    cv::waitKey(0);
    return 0;
}
