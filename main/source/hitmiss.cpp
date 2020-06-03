//
// Created by khamzat on 5/25/20.
//
#include "../headers/hitmiss.h"

hitmiss::hitmiss(cv::Mat inImg, const std::vector<std::vector<int>> &SE1, const std::vector<std::vector<int>> &SE2) : SE1(SE1), SE2(SE2) {
    outImg = thickening(inImg);
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
            cv::Vec3b tempOne = colorsOne[i][j];
            cv::Vec3b tempTwo = colorsTwo[i][j];

            if(tempOne[0] != tempTwo[0] && tempOne[1] != tempTwo[1] && tempOne[2] != tempTwo[2])
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
    bool cover;

    for(int i = 1; i<inImg.rows - 1; ++i){
        for(int j = 1; j<inImg.cols - 1; ++j){
            cover = true;
            int x = 0;
            int y = 0;

            for(int k = 0; k<3; k++){
                for(int l = 0; l < 3; l++){
                    x = i + k - 1;
                    y = j + l - 1;

                    color = outcopy.at<cv::Vec3b>(x, y);

                   if( !((color[0] == SE[k][l]) || SE[k][l] == -1) ){
                        cover = false;
                        break;
                   }
                }
                if(!cover)
                    break;
            }

            if(cover){
                color[0] = 255;
                color[1] = 255;
                color[2] = 255;
                localOutImg.at<cv::Vec3b>(i, j) = color;
            } else{
                color[0] = 0;
                color[1] = 0;
                color[2] = 0;
                localOutImg.at<cv::Vec3b>(i, j) = color;
            }


        }
    }

    return localOutImg;
}

cv::Mat hitmiss::thickening(const cv::Mat& inImg) {
    cv::Mat copy;
    inImg.copyTo(copy);
    cv::Mat localOutImg = cv::Mat::zeros(inImg.rows, inImg.cols, CV_8UC3);

    while(!isEqual(copy, localOutImg)){
        copy.copyTo(localOutImg);

        for(int i = 0; i<4; i++){
            copy = customOr(copy, hitOrMiss(copy, SE1));
            copy = customOr(copy, hitOrMiss(copy, SE2));
            rotate(SE1);
            rotate(SE2);
        }
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
