#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main() {
    std::string name;
    std::cout << "Prosze wprowadzic sciezke do pliku: ";
    std::cin >> name;

    cv::Mat img = cv::imread(name);
    cv::Mat dst;

    if(!img.data){
        std::cout<<"Nie odnaleziono pliku!" << name << std::endl;
        return -1;
    }

    img.convertTo(dst, CV_8U); //CV_8U == unsigned 8bit/pixel
    float arrOut[dst.rows][dst.cols]; //2D array of

    //pass data from picture to 2D array
    for(int i=0; i<dst.rows; ++i)
        for(int j=0; j<dst.cols; ++j)
            arrOut[i][j] = dst.at<float>(i, j);

    //show the array
    for(int i = 0; i < dst.rows; ++i)
        for(int j=0; j<dst.cols; ++j){
            std::cout << arrOut[i][j]<<" ";
            if(j==(dst.cols-1))
                std::cout<<std::endl;
        }

    //show the picture
    const std::string window_name = "OpenCV";
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::imshow(window_name, img);

    cv::waitKey(0);
    return 0;
}
