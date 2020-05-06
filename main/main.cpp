#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main() {
    const std::string name = "../portrait-of-a-wolf.jpg";
    cv::Mat img = cv::imread(name);

    if(!img.data){
        std::cout<<"Nie odnaleziono pliku!" << name << std::endl;
        return -1;
    }

    const std::string window_name = "OpenCV";
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::imshow(window_name, img);

    cv::waitKey(0);
    return 0;
}
