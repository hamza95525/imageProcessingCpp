#include <iostream>
#include "headers/histo.h"
#include "headers/imopen.h"

int main() {
    std::string name;
    std::cout << "Prosze wprowadzic sciezke do pliku: ";
    std::cin >> name;
    cv::Mat img = cv::imread(name);

    if(!img.data) {
        std::cout << "Nie odnaleziono pliku!" << name << std::endl;
        return -1;
    }

    double length;
    double angle;
    std::cout << "Prosze wprowadzic dlugosc: ";
    std::cin >> length;
    std::cout << "Prosze wprowadzic nachylenie: ";
    std::cin >> angle;

    imopen Otwarcie(angle, length);
    Otwarcie.bresenhamsLineAlgorithm();
    Otwarcie.showLine();
    cv::Mat openned = Otwarcie.openMono(img);
   // cv::Mat erosion = Otwarcie.erosionMono(img);
   // cv::Mat dilate = Otwarcie.dilateMono(img);

    cv::namedWindow("wejsciowy", cv::WINDOW_AUTOSIZE);
    cv::imshow("wejsciowy", img);

    cv::namedWindow("otwarty", cv::WINDOW_AUTOSIZE);
    cv::imshow("otwarty", openned);

    cv::waitKey(0);
    return 0;
}
