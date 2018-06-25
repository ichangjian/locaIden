#include "locaIden.h"
#include <tchar.h>
#include <iostream>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

int getCheckerboardUnitLength(unsigned char * imageData, int imageWidth, int imageHeight, int checkerboardRow, int checkerboardCol, int boardSize, float *unit)
{
    Mat img = Mat(imageHeight, imageWidth, CV_8UC1, imageData);
    Mat view = img.clone(); 
    vector<Point2f> pointbuf; 

    bool found = findChessboardCorners(view, Size(checkerboardCol, checkerboardRow), pointbuf, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

    if (found)
    {
        cornerSubPix(view, pointbuf, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));
        drawChessboardCorners(view, Size(checkerboardCol, checkerboardRow), Mat(pointbuf), found);
    }
    imwrite("Checkerboard.jpg", view);
    return 0;
}
int setIdenImage(unsigned char * imageData, int imageWidth, int imageHeight, CardType cardType)
{
    Mat img = Mat(imageHeight, imageWidth, CV_8UC1, imageData);
    Mat src =img.clone(); 
    GaussianBlur(src, src, Size(21, 21), 2, 2); 
    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros(src.size(), CV_32FC1);
    int blockSize = 11;
    int apertureSize = 7;
    double k = 0.04;
    cornerHarris(src, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);

    vector<Point2f> corners;
    for (int j = 0; j < dst_norm.rows; j++)
    {
        for (int i = 0; i < dst_norm.cols; i++)
        {
            if (dst_norm.at<float>(j, i) >55)
            {
                corners.push_back(Point2f(i, j));
                //circle(dst_norm_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);
            }
        }
    }
    Size zeroZone = Size(-1, -1);
    TermCriteria criteria = TermCriteria(
        CV_TERMCRIT_EPS + CV_TERMCRIT_ITER,
        40, //maxCount=40  
        0.001);  //epsilon=0.001  
    cornerSubPix(src, corners, Size(7, 7), zeroZone, criteria);
    for (int i = 0; i < corners.size(); i++)
    {

        circle(src, corners[i], 15, Scalar(0, 255, 255), 1, 8, 0);

    }  
    imwrite("IdenImage.jpg", src);
    return 0;
}
int getIdenRectanglePosition(RectangleType rectType, float postionX[4], float postionY[4])
{
    for (size_t i = 0; i < 4; i++)
    {
        postionY[i] = i;
        postionX[i] = i;
    }
    return 0;
}
int getIdenSerialNumber(char number[8])
{
    for (size_t i = 0; i < 8; i++)
    {
        number[i] = '0';
    }
    number[7] = 1;
    return 0;
}

int main()
{
    //float *kx, float *ky, float *d;
    Mat img=imread("G:/DSC01048/1.jpg",0);
    getCheckerboardUnitLength(img.data, img.cols, img.rows, 9,5, 1, NULL);
    //setIdenImage(img.data, img.cols, img.rows, CardType::CARD_UP);
    float postionX[4]; float postionY[4];
    getIdenRectanglePosition(RECT_HORIZONTAL, postionX, postionY);
    for (size_t i = 0; i < 4; i++)
    {
        cout<<postionY[i];
        cout << postionX[i] <<endl;
    }
    return 0;
}