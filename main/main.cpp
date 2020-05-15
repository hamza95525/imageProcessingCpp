#include <iostream>
#include "histo.h"
#include "imopen.h"

int main() {
  /*  std::string name;
    std::cout << "Prosze wprowadzic sciezke do pliku: ";
    std::cin >> name;

    cv::Mat img = cv::imread(name);
    std::vector<cv::Mat> BGR;
    cv::Vec3b color; //wydobycie koloru z obrazu
    std::vector<float> Red, Green, Blue;

    if(!img.data) {
        std::cout << "Nie odnaleziono pliku!" << name << std::endl;
        return -1;
    }
    cv::split(img, BGR);*/

    //=================HISTOGRAM===================
  /*  histogram Histogram(256, 512, 256, 0, 256); //size, width, height, range = [0,256]
    Histogram.calculate(BGR);
    Histogram.normalize();
    Histogram.drawHisto();
    Histogram.show();*/
    //=============================================
    imopen Otwarcie(45, 5);
    Otwarcie.bresenhamsLineAlgorithm();
    Otwarcie.showLine();

    cv::waitKey(0);
    return 0;
}
