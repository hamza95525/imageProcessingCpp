//
// Created by khamzat on 5/25/20.
//
#include "../headers/hitmiss.h"

hitmiss::hitmiss(cv::Mat inImg, const std::vector<std::vector<int>> &SE1, const std::vector<std::vector<int>> &SE2) : outImg(std::move(inImg)), SE1(SE1), SE2(SE2) {
}

cv::Mat hitmiss::binarization(cv::Mat inImg) {
    cv::Mat localOutImg;
    inImg.copyTo(localOutImg);
    cv::Vec3b color;

    for(int i = 0; i<inImg.rows; ++i){
        for(int j = 0; j<inImg.cols; ++j){
            color = inImg.at<cv::Vec3b>(i, j);
            for(int x = 0; x<3; x++){
                color[x] = (color[x] >= 127) ? 255 : 0;
            }

            localOutImg.at<cv::Vec3b>(i, j) = color;
        }

    }

    return localOutImg;
}

cv::Mat hitmiss::imComplement(const cv::Mat& inImg) {
    cv::Mat localOutImg;
    localOutImg = binarization(inImg);
    cv::Vec3b color;

    for(int i = 0; i<inImg.rows; i++) {
        for (int j = 0; j < inImg.cols; j++) {
            color = localOutImg.at<cv::Vec3b>(i, j);
            //std::cout << outImg.at<cv::Vec3b>(i, j);
              if(color[0] == 255){
                  color[0] = 0; color[1] = 0; color[2] = 0;
                  localOutImg.at<cv::Vec3b>(i, j) = color;
              } else {
                  color[0] = 255; color[1] = 255; color[2] = 255;
                  localOutImg.at<cv::Vec3b>(i, j) = color;
              }
          }

    }
    return localOutImg;
}

void hitmiss::rotate(std::vector<std::vector<int> > &SE) {
    int row = 0;
    int col = 0;
    int prev, curr;
    int m = 3;
    int n = 3;

    while(row < m && col < n){
        if(row+1 == m || col+1 == n )
            break;

        prev = SE[row+1][col];

        for(int i = col; i<n; i++){
            curr = SE[row][i];
            SE[row][i] = prev;
            prev = curr;
        }
        row++;

        for(int i = row; i<m; i++){
            curr = SE[i][n-1];
            SE[i][n-1] = prev;
            prev = curr;
        }
        n--;

        if(row < m){
            for(int i = n-1; i>= col; i--){
                curr = SE[m-1][i];
                SE[m-1][i] = prev;
                prev = curr;
            }
        }
        m--;

        if (col < n)
        {
            for (int i = m-1; i >= row; i--)
            {
                curr = SE[i][col];
                SE[i][col] = prev;
                prev = curr;
            }
        }
        col++;
    }
}

bool hitmiss::isEqual(const cv::Mat &imgOne, const cv::Mat &imgTwo) {
    std::vector<std::vector<cv::Vec3b>> colorsOne;
    std::vector<std::vector<cv::Vec3b>> colorsTwo;

    if(imgOne.rows != imgTwo.rows || imgOne.cols != imgTwo.cols)
        return false;

    for(int i = 0; i<imgOne.rows; i++){
        std::vector<cv::Vec3b> temp;
        for(int j = 0; j<imgOne.cols; j++){
            temp.push_back(imgOne.at<cv::Vec3b>(i, j));
        }
        colorsOne.push_back(temp);
    }

    for(int i = 0; i<imgTwo.rows; i++){
        std::vector<cv::Vec3b> temp;
        for(int j = 0; j<imgTwo.cols; j++){
            temp.push_back(imgTwo.at<cv::Vec3b>(i, j));
        }
        colorsTwo.push_back(temp);
    }

    for(int i = 0; i<colorsOne.size(); i++){
        for(int j = 0; j<colorsOne[i].size(); j++){
            if(colorsOne[i][j] != colorsTwo[i][j])
                return false;
        }
    }

    return true;
}

cv::Mat hitmiss::hitOrMiss(const cv::Mat& inImg, const std::vector<std::vector<int>>& SE ) {
    cv::Mat localOutImg;

    cv::Mat outcopy;
    localOutImg = binarization(inImg);
    localOutImg.copyTo(outcopy);

    cv::Vec3b color;
    int halfLength = (3-1)/2;
    bool cover;

    for(int i = halfLength; i<inImg.rows - halfLength; ++i){
        for(int j = halfLength; j<inImg.cols - halfLength; ++j){
            cover = true;

            for(int k = i - halfLength, x = 0; k<=i+halfLength; k++, x++){
                for(int l = j - halfLength, y = 0; l <= j+halfLength; l++, y++){
                    color = outcopy.at<cv::Vec3b>(k, l);

                    if( !((color[0] == SE[x][y]) || SE[x][y] == -1) ){
                        cover = false;
                        break;
                    }
                }
                if(!cover)
                    break;
            }
            if(cover) {
                for (int m = i - halfLength; m <= i + halfLength; m++) {
                    for (int n = j - halfLength; n <= j + halfLength; n++) {
                        color[0] = 255;
                        color[1] = 255;
                        color[2] = 255;
                        localOutImg.at<cv::Vec3b>(m, n) = color;
                    }
                }
            } else {
                for(int m = i - halfLength; m<=i+halfLength; m++){
                    for(int n = j - halfLength; n <= j+halfLength; n++){
                        color[0] = 0;
                        color[1] = 0;
                        color[2] = 0;
                        localOutImg.at<cv::Vec3b>(m, n) = color;
                    }
                }
            }

        }
    }

    return localOutImg;
}

cv::Mat hitmiss::convexHull(const cv::Mat& inImg) {
    cv::Mat copy;
    inImg.copyTo(copy);
    cv::Mat localOutImg = cv::Mat::zeros(inImg.rows, inImg.cols, CV_8UC3);

    while(!isEqual(copy, localOutImg)){
        copy.copyTo(localOutImg);

        for(int i = 0; i<8; i++){
            copy = customOr(copy, hitOrMiss(copy, SE1));
            copy = customOr(copy, hitOrMiss(copy, SE2));
            std::cout<<"- - -\n";
            showSE(SE1);
            rotate(SE1);
            rotate(SE2);
        }

        std::cout<<"FLAGA"<<std::endl;
    }

    return copy;
}

cv::Mat hitmiss::customOr(const cv::Mat& imgOne, const cv::Mat& imgTwo) {
    cv::Vec3b colorOne;
    cv::Vec3b colorTwo;
    cv::Vec3b color;
    cv::Mat localOutImg;

    cv::Mat binOne;
    cv::Mat binTwo;

    binOne = binarization(imgOne);
    binTwo = binarization(imgTwo);

    localOutImg = binarization(imgOne);

    for(int i = 0, k = 0; i<imgOne.rows; i++, k++){
        for(int j = 0, l = 0; j<imgOne.cols; j++, l++){
            colorOne = binOne.at<cv::Vec3b>(i, j);
            colorTwo = binTwo.at<cv::Vec3b>(k, l);

            if( (colorOne[0]==255 && colorOne[1]==255 && colorOne[2]==255) ||
                    (colorTwo[0]==255 && colorTwo[1]==255 && colorTwo[2]==255) ){
                color[0] = 255; color[1] = 255; color[2] = 255;
                localOutImg.at<cv::Vec3b>(i, j) = color;
            } else{
                color[0] = 0; color[1] = 0; color[2] = 0;
                localOutImg.at<cv::Vec3b>(i, j) = color;
            }
        }
    }

    return localOutImg;
}

cv::Mat hitmiss::customSum(const cv::Mat& imgOne,const cv::Mat& imgTwo) {
    cv::Vec3b colorOne;
    cv::Vec3b colorTwo;
    cv::Vec3b color;
    cv::Mat localOutImg;

    cv::Mat binOne;
    cv::Mat binTwo;

    binOne = binarization(imgOne);
    binTwo = binarization(imgTwo);

    localOutImg = binarization(imgOne);

    for(int i = 0, k = 0; i<imgOne.rows; i++, k++){
        for(int j = 0, l = 0; j<imgOne.cols; j++, l++){
            colorOne = binOne.at<cv::Vec3b>(i, j);
            colorTwo = binTwo.at<cv::Vec3b>(k, l);

            if( (colorOne[0]==255 && colorOne[1]==255 && colorOne[2]==255) &&
                (colorTwo[0]==255 && colorTwo[1]==255 && colorTwo[2]==255)){
                color[0] = 255; color[1] = 255; color[2] = 255;
                localOutImg.at<cv::Vec3b>(i, j) = color;
            } else{
                color[0] = 0; color[1] = 0; color[2] = 0;
                localOutImg.at<cv::Vec3b>(i, j) = color;
            }
        }
    }

    return localOutImg;
}
