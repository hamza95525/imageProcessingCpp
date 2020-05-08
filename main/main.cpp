#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main() {
    std::string name;
    std::cout << "Prosze wprowadzic sciezke do pliku: ";
    std::cin >> name;

    cv::Mat img = cv::imread(name);
    //  cv::Mat dst;
    // cv::Vec3b color; //wydobycie koloru z obrazu
    // std::vector<int> Red, Green, Blue;

    std::vector<cv::Mat> BGR;

    if(!img.data){
        std::cout<<"Nie odnaleziono pliku!" << name << std::endl;
        return -1;
    }

    cv::split(img, BGR);

    int histSize = 256; //number of bins
    float range[] = {0, 256};
    const float* histRange = {range}; //range of values (0..255)

    bool uniform = true; bool accumulate = false;

    cv::Mat bHist, gHist, rHist;

    cv::calcHist( &BGR[0], 1, 0, cv::Mat(), bHist, 1, &histSize, &histRange, uniform, accumulate);
    cv::calcHist( &BGR[1], 1, 0, cv::Mat(), gHist, 1, &histSize, &histRange, uniform, accumulate);
    cv::calcHist( &BGR[2], 1, 0, cv::Mat(), rHist, 1, &histSize, &histRange, uniform, accumulate);

    int histWidth = 512;
    int histHeight = 512;
    int bin_w = cvRound((double ) histWidth / histSize);
    cv::Mat histImage(histHeight, histWidth, CV_8UC3, cv::Scalar(0,0,0));

    cv::normalize(bHist, bHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(gHist, gHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(rHist, rHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());


    for(int i = 1; i<histSize; ++i){
        cv::line(histImage, cv::Point(bin_w * (i-1), histHeight - cvRound(bHist.at<float>(i-1))),
                cv::Point(bin_w*(i), histHeight - cvRound(bHist.at<float>(i))),
                cv::Scalar(255, 0, 0), 2, 8, 0);

        cv::line(histImage, cv::Point(bin_w * (i-1), histHeight - cvRound(gHist.at<float>(i-1))),
                 cv::Point(bin_w*(i), histHeight - cvRound(gHist.at<float>(i))),
                 cv::Scalar(0, 255, 0), 2, 8, 0);

        cv::line(histImage, cv::Point(bin_w * (i-1), histHeight - cvRound(rHist.at<float>(i-1))),
                 cv::Point(bin_w*(i), histHeight - cvRound(rHist.at<float>(i))),
                 cv::Scalar(0, 0, 255), 2, 8, 0);
    }


    //show the picture

    cv::namedWindow("Histogram", cv::WINDOW_AUTOSIZE);
    cv::imshow("Histogram", histImage);

    const std::string window_name = "OpenCV";
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::imshow(window_name, img);

    cv::waitKey(0);
    return 0;
}


/*
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