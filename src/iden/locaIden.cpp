#include "locaIden.h"
#include <tchar.h>
#include <iostream>
#include<algorithm> 
#include <opencv2\opencv.hpp>
#include <ctime>
using namespace std;
using namespace cv;

float length(cv::Point2f a, cv::Point2f b)
{
    return sqrt((a.x - b.x) *(a.x - b.x) + (a.y - b.y)* (a.y - b.y));
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
float getCheckerboardPixelLength(Mat& img, Subdiv2D& subdiv, Scalar delaunay_color, bool saveImage)
{
    time_t T1 = clock();
    std::vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<cv::Point2f> pt(3);
    Size size = img.size();
    cv::Rect rect(0, 0, size.width, size.height);
    Mat image;
    if (saveImage)
    {
        cvtColor(img, image, CV_GRAY2BGR);
    }
    int num = 0;
    int gap = 10;
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
            if (saveImage)
            {
                drawDelaunay(image, L, pt[0], pt[1]);
                drawDelaunay(image, L, pt[1], pt[2]);
                drawDelaunay(image, L, pt[2], pt[0]);
            }
            //cout <<i<<"a\t"<< a << "\t" << b << "\t" << c << "\n";
            if (a < 200)
            {
                continue;
            }
            if (b - gap < c)
            {
                num += 2;
                dist += b;
                dist += c;
                //cout << i << "c\t" << "\n\n";                
                if (saveImage)
                {
                    drawDelaunay(image, L, pt[0], pt[1]);
                    drawDelaunay(image, L, pt[1], pt[2]);
                    drawDelaunay(image, L, pt[2], pt[0]);
                }

            }
            else if (b > a - gap)
            {

                num += 2;
                dist += b;
                dist += a;
                //cout << i << "b\t" << "\n\n";
                if (saveImage)
                {
                    drawDelaunay2(image, L, pt[0], pt[1]);
                    drawDelaunay2(image, L, pt[1], pt[2]);
                    drawDelaunay2(image, L, pt[2], pt[0]);
                }

            }

        }
    }

    time_t T2 = clock();
    if (saveImage)
    {
        char c[512];
        sprintf_s(c, "%fs", double(T2 - T1) / CLOCKS_PER_SEC);
        putText(image, c, cv::Point(50, 50), 1, 2, Scalar(0, 0, 255));
        imwrite("Checkerboard2.jpg", image);
    }
    if (num == 0)
    {
        return 0;
    }
    return dist / num;

}
int loca::getCheckerboardUnitLength(unsigned char * imageData, int imageWidth, int imageHeight, loca::Rect rectRegion, int boardSize, float *unit, bool saveImage)
{
    if (boardSize == 0)
    {
        return -1;
    }
    time_t T1 = clock();
    Mat img = Mat(imageHeight, imageWidth, CV_8UC3, imageData).clone();
    cvtColor(img, img, CV_BGR2GRAY);
    Mat src = img(cv::Rect(rectRegion.x, rectRegion.y, rectRegion.with, rectRegion.height)).clone(); time_t T2 = clock();
    //cout << float(T2 - T1) / CLOCKS_PER_SEC << endl;
    medianBlur(src, src, 11);
    GaussianBlur(src, src, Size(0, 0), 1, 1);
    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros(src.size(), CV_32FC1);
    int blockSize = 21;
    int apertureSize = 7;
    double k = 0.04;
    cornerHarris(src, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    //convertScaleAbs(dst_norm, dst_norm_scaled);
    int old_i = 0, old_j = 0;
    int gap = 5;
    vector<cv::Point2f> corners;
    Mat tp;
    blur(dst_norm, tp, Size(51, 51));
    Mat dd = dst_norm - tp;
    
    for (int j = 50; j < dst_norm.rows - 50; j++)
    {
        for (int i = 50; i < dst_norm.cols - 50; i++)
        {
            if (dd.at<float>(j, i) >50)
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
    cornerSubPix(src, corners, Size(51, 51), zeroZone, criteria);

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
    if (corners.size()>100)
    {
        return -1;
    }
    *unit = getCheckerboardPixelLength(src, subdiv, Scalar(255), saveImage) / boardSize;

    time_t T3 = clock();
    //cout << float(T3 - T1) / CLOCKS_PER_SEC << endl;
    if (saveImage)
    {
        for (int i = 0; i < corners.size(); i++)
        {
            circle(src, corners[i], 5, Scalar(255), 1, 8, 0);
        }
        char c[512];
        sprintf_s(c, "%fs", double(T3 - T1) / CLOCKS_PER_SEC);
        putText(src, c, cv::Point(50, 50), 1, 2, Scalar(125));
        imwrite("Checkerboard1.jpg", src);
    }

    return 0;
}



Mat idenImage;
loca::CardType cardTyp;

int loca::setIdenImage(unsigned char * imageData, int imageWidth, int imageHeight, loca::CardType cardType)
{
    cardTyp = cardType;
    Mat img = Mat(imageHeight, imageWidth, CV_8UC3, imageData).clone();
    cvtColor(img, img, CV_BGR2GRAY);
    Mat src = img.clone();
    //GaussianBlur(src, src, Size(0,0), 1, 1);
    medianBlur(src, src, 11);
    idenImage = src.clone();
    return 0;
    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros(src.size(), CV_32FC1);
    int blockSize = 11;
    int apertureSize = 7;
    double k = 0.04;
    cornerHarris(src, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);

    cv::Mat dilated, localMax;
    //默认3*3核膨胀，膨胀之后，除了局部最大值点和原来相同，其它非局部最大值点被  
    //3*3邻域内的最大值点取代  
    cv::dilate(dst_norm, dilated, cv::Mat());
    //与原图相比，只剩下和原图值相同的点，这些点都是局部最大值点，保存到localMax  
    cv::compare(dst_norm, dilated, localMax, cv::CMP_EQ);

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
    cornerSubPix(src, corners, Size(11, 11), zeroZone, criteria);
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


int  loca::getIdenRectanglePosition(loca::Rect rectRegion, loca::RectangleType rectType, loca::Point2f postion[4], bool saveImage)
{
    return -1;
    cv::Rect rect(rectRegion.x, rectRegion.y, rectRegion.with, rectRegion.height);
    Mat img = idenImage(rect).clone();
    Mat dst;
    GaussianBlur(img, dst, Size(21, 21), 2, 2);
    //Mat dst;
    adaptiveThreshold(dst, dst, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 137, 50);
    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(dst.clone(), contours, cv::noArray(), cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
    vector<cv::RotatedRect> rests;
    cv::Point2f points[4];
    for (size_t i = 0; i < contours.size(); i++)
    {
        cv::Rect rect = boundingRect(contours[i]);
        float WHR = 1.0*rect.width / rect.height;
        //cout << rect << endl;
        switch (rectType)
        {
        case loca::RECT_HORIZONTAL:
            if ((rect.width > 80 && rect.height > 30))
                if (WHR > 2)
                {
                    RotatedRect rotRect = minAreaRect(contours[i]);
                    rests.push_back(rotRect);
                    //rectangle(img, rect, Scalar(125), 5);
                    rotRect.points(points); 

                    if (saveImage)
                    {
                        circle(img, points[0], 5, Scalar(200));
                        circle(img, points[1], 5, Scalar(200));
                        circle(img, points[2], 5, Scalar(200));
                        circle(img, points[3], 5, Scalar(200));
                        circle(img, rotRect.center, 5, Scalar(200), -1);

                    }
                }
            break;
        case loca::RECT_VERTICAL:
            if ((rect.width > 20 && rect.height > 80))
                if (1.0 / WHR > 2)
                {
                    RotatedRect rotRect = minAreaRect(contours[i]);
                    rests.push_back(rotRect);
                    //rectangle(img, rect, Scalar(255), 5);      
                    rotRect.points(points);                   
                     

                    if (saveImage)
                    {
                        circle(img, points[0], 5, Scalar(200));
                        circle(img, points[1], 5, Scalar(200));
                        circle(img, points[2], 5, Scalar(200));
                        circle(img, points[3], 5, Scalar(200));
                        circle(img, rotRect.center, 5, Scalar(200), -1);

                    }

                }
            break;
        default:
            break;
        }
    }//for i

    if (saveImage)
    {
        imwrite("getIdenRectangleRC.jpg", img);
    }
    if (rests.size() != 1)
    {
        return -1;
    }
    for (size_t i = 0; i < 4; i++)
    {
        postion[i].x = points[i].x;
        postion[i].y = points[i].y;
    }
    return 0;

    return 0;
}
 
 
float getAngel(cv::Point2f pts[4])
{
    int max1=0, max2=0;
    for (size_t i = 0; i < 4; i++)
    {
        //cout << pts[i] << endl;
        if (pts[i].y>pts[max1].y)
        {
            max1 = i;
        }
    }
    max2 = (max1 + 1) % 4;
    for (size_t i = 0; i < 4; i++)
    {
        //cout << pts[i] << endl;
        if (pts[i].y>pts[max2].y)
        {
            if (max1 != i)
            {
                max2 = i;
            }
        }
    }
 
    cv::Point2f pt1;
    cv::Point2f pt2;
    if (pts[max1].x<pts[max2].x)
    {
        pt1 = pts[max1];
        pt2 = pts[max2];
    }
    else
    {

        pt2 = pts[max1];
        pt1= pts[max2];
    }
    cv::Point2f tp = pt2 - pt1;
    pt2 = cv::Point2f(pt2.x, pt1.y) - pt1;
    pt1 = tp;
    float theta = 0;
    theta = (pt1.x*pt2.x + pt1.y *pt2.y) / (sqrt(pt1.x*pt1.x + pt1.y *pt1.y)*sqrt(pt2.x*pt2.x + pt2.y *pt2.y));
    theta = acos(theta);
    theta = theta * 180.0 / CV_PI;
    if (pt1.y>0)
    {
        theta = -theta;
    }

    return theta;
}

int loca::getIdenRectangleRC(loca::Rect rectRegion, loca::RectangleType rectType, int markWidth, int markHeight, float* rotation, loca::Point2f *centre, bool saveImage)
{
    cv::Rect rectSrc(rectRegion.x, rectRegion.y, rectRegion.with, rectRegion.height);
    Mat img = idenImage(rectSrc).clone();
    //img = imread("G:/DSC01048/1/getIdenRectangleRC2.jpg", 0);
    Mat dst;
    blur(img,dst,Size(17,17));
    GaussianBlur(dst, dst, Size(21, 21), 2, 2);
    if (saveImage)
    {
        char c[512];
        sprintf_s(c, "%0.1f %0.1f %0.1f %0.1f", rectRegion.x, rectRegion.y, rectRegion.with, rectRegion.height);
        putText(img, c, cv::Point(50, 50), 1, 2, Scalar(125));
        sprintf_s(c, "rectType %d,%d,%d", (int)rectType,markWidth, markHeight);
        putText(img, c, cv::Point(50, 150), 1, 2, Scalar(125));
        imwrite("getIdenRectangleRC1.jpg", img);
        imwrite("getIdenRectangleRC0.jpg", idenImage);
    }
    //Mat dst;
    adaptiveThreshold(dst, dst, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 137, 50);
    //threshold(dst, dst, 0, 255, THRESH_OTSU);

    if (saveImage)
    {
 
        imwrite("getIdenRectangleRC3.jpg", dst);
    }
    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(dst.clone(), contours, cv::noArray(), cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
    vector<cv::RotatedRect> rests;
    for (size_t i = 0; i < contours.size(); i++)
    {
        RotatedRect rotRect = minAreaRect(contours[i]);
        // boundingRect(contours[i]);
        //float WHR = 1.0*rect.width / rect.height;
        cv::Rect rect;
        switch (rectType)
        {
        case loca::RECT_HORIZONTAL:
            if (rotRect.size.width>rotRect.size.height)
            {
                rect = cv::Rect(0, 0, rotRect.size.width, rotRect.size.height);
            }
            else
            {
                rect = cv::Rect(0, 0, rotRect.size.height, rotRect.size.width);
            }
            
            if ((rect.width > markWidth*0.6 && rect.height > markHeight*0.6) && (rect.width < markWidth*1.3 && rect.height < markHeight*1.3))
                //if (WHR > 1.5)
                {
                   
                    rests.push_back(rotRect);
                    //rectangle(img, rect, Scalar(125), 5);
                    cv::Point2f points[4];
                    rotRect.points(points);                  
                    float angel = getAngel(points);
                    *rotation = angel;
                    centre->x = rotRect.center.x;
                    centre->y = rotRect.center.y;
             
                    if (saveImage)
                    {
                        circle(img, points[0], 5, Scalar(200), -1);
                        circle(img, points[1], 5, Scalar(200), -1);
                        circle(img, points[2], 5, Scalar(200), -1);
                        circle(img, points[3], 5, Scalar(200), -1);
                        circle(img, rotRect.center, 5, Scalar(200), -1);

                    }
                }
            break;
        case loca::RECT_VERTICAL:
            if (rotRect.size.width>rotRect.size.height)
            {
                rect = cv::Rect(0, 0, rotRect.size.height, rotRect.size.width);
            }
            else
            {
                rect = cv::Rect(0, 0, rotRect.size.width, rotRect.size.height);
            }
            if ((rect.width > markWidth*0.6 && rect.height > markHeight*0.6) && (rect.width < markWidth*1.3 && rect.height < markHeight*1.3))
                //if (1.0 / WHR > 2)
                {
                    //RotatedRect rotRect = minAreaRect(contours[i]);
                    rests.push_back(rotRect);
                    //rectangle(img, rect, Scalar(255), 5);                    
                    cv::Point2f points[4];
                    rotRect.points(points);

                    float angel = getAngel(points);
                    *rotation = angel;
                    centre->x = rotRect.center.x;
                    centre->y = rotRect.center.y;

                    if (saveImage)
                    {
                        circle(img, points[0], 5, Scalar(200), -1);
                        circle(img, points[1], 5, Scalar(200), -1);
                        circle(img, points[2], 5, Scalar(200), -1);
                        circle(img, points[3], 5, Scalar(200), -1);
                        circle(img, rotRect.center, 5, Scalar(200), -1);

                    }

                }
            break;
        default:
            break;
        }
    }//for i
    if (saveImage)
    {
        imwrite("getIdenRectangleRC2.jpg",img);
    }
    if (rests.size() != 1)
    {
        return -1;
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
