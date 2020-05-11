#include <iostream>
#include "histo.h"

int main() {
    std::string name;
    std::cout << "Prosze wprowadzic sciezke do pliku: ";
    std::cin >> name;

    cv::Mat img = cv::imread(name);
    std::vector<cv::Mat> BGR;

    if(!img.data){
        std::cout<<"Nie odnaleziono pliku!" << name << std::endl;
        return -1;
    }

    cv::split(img, BGR); 

    histo Histogram(256, img.cols, img.rows, 0, 256);
    Histogram.calculate(BGR);
    Histogram.normalize();

    cv::Mat histoImg = Histogram.drawHisto();

    bool uniform = true; bool accumulate = false;

    cv::Mat bHist, gHist, rHist;

    //show the picture

    cv::namedWindow("Histogram", cv::WINDOW_AUTOSIZE);
    cv::imshow("Histogram", histoImg);

    const std::string window_name = "OpenCV";
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::imshow(window_name, img);

    cv::waitKey(0);
    return 0;
}


/*
 *
 *     //cv::Mat dst;
    //cv::Vec3b color; //wydobycie koloru z obrazu
    //std::vector<int> Red, Green, Blue;
 *     img.convertTo(dst, CV_8U); //CV_8U == unsigned 8bit/pixel
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


         //get every pixels RGB values
    for(int y = 0; y < img.rows; ++y){
        for(int x = 0; x < img.cols; ++x){
            color = img.at<cv::Vec3b>(cv::Point(x,y));

            //BGR == RGB
            Blue.push_back(color.val[0]);
            Green.push_back(color.val[1]);
            Red.push_back(color.val[2]);
        }
    }

        */