#include "locaIden.h"
#include <tchar.h>
#include <iostream>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

 
int main()
{
    //float *kx, float *ky, float *d;
    Mat img=imread("G:/DSC01048/chess2.bmp");
    float unit=0;
    loca::Rect rectRegion{ 864, 648, 864, 648, };
    loca::getCheckerboardUnitLength(img.data, img.cols, img.rows, rectRegion,10, &unit, true);
    cout << unit << endl;
    loca::setIdenImage(img.data, img.cols, img.rows, loca::CardType::CARD_UP);
    loca::Point2f pt;
    rectRegion = { 1000, 1000, 600, 400 };
    //rectRegion = { 900, 50, 800, 300 };
    //rectRegion = { 900, 400, 800, 1100 };
    //rectRegion = { 900, 1600, 800, 300 };

    //rectRegion = { 1000, 600, 600, 700 };

    //rectRegion = { 600, 100, 600, 700 };
    loca::getIdenRectangleRC(rectRegion, loca::RectangleType::RECT_HORIZONTAL, &unit, &pt, true);
    loca::Point2f postion[4];
    loca::getIdenRectanglePosition(rectRegion, loca::RectangleType::RECT_VERTICAL, postion, true);
    for (size_t i = 0; i < 4; i++)
    {
        cout << postion[i].x << "\t" << postion[i].y <<endl;
    }
    getchar();
    return 0;
}