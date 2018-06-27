#include "locaIden.h"
#include <tchar.h>
#include <iostream>
#include<algorithm> 
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

float length(cv::Point2f a, cv::Point2f b)
{
    return abs((a.x - b.x) + (a.y - b.y));
}

void drawDelaunay(Mat& img, float L[3], cv::Point2f pt1, cv::Point2f pt2)
{
    for (size_t i = 0; i < 3; i++)
    {
        if (L[i] == length(pt1, pt2))
        {
            if (i == 0){
                line(img, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA, 0);
            }
            else
            {
                line(img, pt1, pt2, Scalar(255, 0, 0), 1, CV_AA, 0);
            }
        }
    }
}
void drawDelaunay2(Mat& img, float L[3], cv::Point2f pt1, cv::Point2f pt2)
{
    for (size_t i = 0; i < 3; i++)
    {
        if (L[i] == length(pt1, pt2))
        {
            if (i == 2){
                line(img, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA, 0);
            }
            else
            {
                line(img, pt1, pt2, Scalar(0, 255, 0), 1, CV_AA, 0);
            }
        }
    }
}
float getCheckerboardPixelLength(Mat& img, Subdiv2D& subdiv, Scalar delaunay_color)
{

    std::vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<cv::Point2f> pt(3);
    Size size = img.size();
    cv::Rect rect(0, 0, size.width, size.height);
    //Mat image;
    //cvtColor(img, image, CV_GRAY2BGR);
    int num = 0;
    double dist = 0;
    for (size_t i = 0; i < triangleList.size(); i++)
    {
        Vec6f t = triangleList[i];
        pt[0] = cv::Point2f((t[0]), (t[1]));
        pt[1] = cv::Point2f((t[2]), (t[3]));
        pt[2] = cv::Point2f((t[4]), (t[5]));

        // Draw rectangles completely inside the image.  
        if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
        {
            float a = length(pt[0], pt[1]);
            float b = length(pt[2], pt[1]);
            float c = length(pt[0], pt[2]);
            float t;
            if (a < b) { t = a; a = b; b = t; }
            if (a < c){ t = a; a = c; c = t; }
            if (b < c){ t = b; b = c; c = t; }
            float L[3] = { a, b, c };
            //cout <<i<<"a\t"<< a << "\t" << b << "\t" << c << "\n";
            if (b - 2 < c)
            {
                num += 2;
                dist += b;
                dist += c;
                //cout << i << "c\t" << "\n\n";
                //drawDelaunay(image,L, pt[0], pt[1]);
                //drawDelaunay(image, L, pt[1], pt[2]);
                //drawDelaunay(image, L, pt[2], pt[0]);
            }
            else if (b > a - 2)
            {

                num += 2;
                dist += b;
                dist += a;
                //cout << i << "b\t" << "\n\n";
                //drawDelaunay2(image, L, pt[0], pt[1]);
                //drawDelaunay2(image, L, pt[1], pt[2]);
                //drawDelaunay2(image, L, pt[2], pt[0]);
            }

        }
    }
    return dist / num;

}
int loca::getCheckerboardUnitLength(unsigned char * imageData, int imageWidth, int imageHeight, int boardSize, float *unit, bool saveImage)
{
    Mat img = Mat(imageHeight, imageWidth, CV_8UC1, imageData);
    Mat src = img.clone();

    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros(src.size(), CV_32FC1);
    int blockSize = 11;
    int apertureSize = 7;
    double k = 0.04;
    cornerHarris(src, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);
    int old_i = 0, old_j = 0;
    int gap = 5;
    vector<cv::Point2f> corners;
    for (int j = 0; j < dst_norm.rows; j++)
    {
        for (int i = 0; i < dst_norm.cols; i++)
        {
            if (dst_norm.at<float>(j, i) >80)
            {
                if (i > old_i + gap || j > old_j + gap)
                {
                    corners.push_back(cv::Point2f(i, j));
                    old_i = i; old_j = j;
                }

            }
        }
    }
    Size zeroZone = Size(-1, -1);
    TermCriteria criteria = TermCriteria(
        CV_TERMCRIT_EPS + CV_TERMCRIT_ITER,
        40, //maxCount=40  
        0.001);  //epsilon=0.001  
    cornerSubPix(src, corners, Size(11, 11), zeroZone, criteria);

    cv::Point2f oldPT(0, 0), gapPT(0.1, 0.1);
    Subdiv2D subdiv(cv::Rect(0, 0, src.cols, src.rows));

    for (size_t i = 0; i < corners.size(); i++)
    {
        subdiv.insert(corners[i]);
        for (size_t j = corners.size() - 1; j > i; j--)
        {
            if ((corners[i].x - gap) < corners[j].x&&\
                (corners[i].x + gap) > corners[j].x&&\
                (corners[i].y - gap) < corners[j].y&&\
                (corners[i].y + gap) > corners[j].y)
            {
                corners.erase(corners.begin() + j);
            }
        }
    }
    *unit = getCheckerboardPixelLength(src, subdiv, Scalar(255)) / boardSize;
    if (saveImage)
    {
        for (int i = 0; i < corners.size(); i++)
        {
            circle(src, corners[i], 5, Scalar(255), 1, 8, 0);
        }

        imwrite("Checkerboard.jpg", src);
    }

    return 0;
}



Mat idenImage;
int loca::setIdenImage(unsigned char * imageData, int imageWidth, int imageHeight, loca::CardType cardType)
{
    Mat img = Mat(imageHeight, imageWidth, CV_8UC1, imageData);
    Mat src = img.clone();
    GaussianBlur(src, src, Size(0,0), .5, .5);
    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros(src.size(), CV_32FC1);
    int blockSize = 11;
    int apertureSize = 7;
    double k = 0.04;
    cornerHarris(src, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);

    vector<cv::Point2f> corners;
    for (int j = 0; j < dst_norm.rows; j++)
    {
        for (int i = 0; i < dst_norm.cols; i++)
        {
            if (dst_norm.at<float>(j, i) >100)
            {
                corners.push_back(cv::Point2f(i, j));
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
    /* GaussianBlur(img,img,Size(21,21),2,2);
    //Mat dst;
    adaptiveThreshold(img, dst, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,137,1);
    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(
        dst,
        contours,
        cv::noArray(),
        cv::RETR_LIST,
        cv::CHAIN_APPROX_SIMPLE
        );
    vector<Rect> rests;
    for (size_t i = 0; i < contours.size(); i++)
    {
       Rect rect= boundingRect(contours[i]);
       if ((rect.width>30 && rect.height>300) || (rect.width>300 && rect.height>30))
       {
           rests.push_back(rect);
           rectangle(img, rect, Scalar(255), 5);
       }
    }*/
    return 0;
}


int  loca::getIdenRectanglePosition(loca::RectangleType rectType, loca::Point2f postion[4], bool saveImage)
{
    for (size_t i = 0; i < 4; i++)
    {
        postion[i].x = i;
        postion[i].y = i;
    }
    return 0;
}

int loca::getIdenRectangleRC(loca::Rect rectRegion, loca::RectangleType rectType, float* rotation, loca::Point2f* centre, bool saveImage)
{
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
