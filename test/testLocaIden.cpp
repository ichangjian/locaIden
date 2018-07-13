#include "locaIden.h"
#include "usbCamera.h"
#include <tchar.h>
#include <iostream>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

void testIden();
void testCamera();
int main()
{
    for (size_t i = 0; i < 100; i++)
    {
        cout << "\nkai shi\n";
        testCamera();
    }
    return 0;
}


void testIden()
{
    //float *kx, float *ky, float *d;
    Mat img = imread("G:/DSC01048/up2.bmp");
    float unit = 0;
    loca::Rect rectRegion{ 864, 648, 864, 648, };
    //loca::getCheckerboardUnitLength(img.data, img.cols, img.rows, rectRegion,10, &unit, true);
    cout << unit << endl;
    loca::setIdenImage(img.data, img.cols, img.rows, loca::CardType::CARD_UP);
    loca::Point2f pt;
    rectRegion = { 1400, 70, 600, 400 };
    //rectRegion = { 900, 50, 800, 300 };   
    float angel = 0;
    loca::getIdenRectangleRC(rectRegion, loca::RectangleType::RECT_VERTICAL, 15, 270, &angel, &pt, true);
    circle(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), 10, Scalar(0, 0, 255));
    line(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), cv::Point2f(rectRegion.x + pt.x + 1000, rectRegion.y + pt.y - tan(angel*3.1415926 / 180)*(1000)), Scalar(255, 0, 0), 4);
    line(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), cv::Point2f(rectRegion.x + pt.x - tan((angel)*3.1415926 / 180)*(1000), rectRegion.y + pt.y - 1000), Scalar(255, 0, 0), 4);
    cout << "angel\t" << angel << "\t" << endl;

    rectRegion = { 900, 400, 800, 1100 };
    rectRegion = { 1300, 400, 400, 700 };
    loca::getIdenRectangleRC(rectRegion, loca::RectangleType::RECT_HORIZONTAL, 180, 30, &angel, &pt, true);
    circle(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), 10, Scalar(0, 0, 255));
    line(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), cv::Point2f(rectRegion.x + pt.x + 1000, rectRegion.y + pt.y - tan(angel*3.1415926 / 180)*(1000)), Scalar(255, 0, 0), 4);
    line(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), cv::Point2f(rectRegion.x + pt.x - tan((angel)*3.1415926 / 180)*(1000), rectRegion.y + pt.y - 1000), Scalar(255, 0, 0), 4);
    cout << "angel\t" << angel << "\t" << endl;

    rectRegion = { 1300, 1000, 800, 200 };
    //rectRegion = { 600, 100, 600, 700 };
    loca::getIdenRectangleRC(rectRegion, loca::RectangleType::RECT_HORIZONTAL, 180, 25, &angel, &pt, true);
    circle(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), 10, Scalar(0, 0, 255));
    line(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), cv::Point2f(rectRegion.x + pt.x + 1000, rectRegion.y + pt.y - tan(angel*3.1415926 / 180)*(1000)), Scalar(255, 0, 0), 4);
    line(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), cv::Point2f(rectRegion.x + pt.x - tan((angel)*3.1415926 / 180)*(1000), rectRegion.y + pt.y - 1000), Scalar(255, 0, 0), 4);
    cout << "angel\t" << angel << "\t" << endl;

    //rectRegion = { 1000, 600, 600, 700 };
    loca::Point2f postion[4];
    loca::getIdenRectanglePosition(rectRegion, loca::RectangleType::RECT_HORIZONTAL, postion, true);
    for (size_t i = 0; i < 4; i++)
    {
        cout << postion[i].x << "\t" << postion[i].y << endl;
    }
    getchar();
}
void testCamera()
{
    usbCamera *camera=nullptr;
    creatCamera(&camera);
    cout<<camera->openCamera(0);
    cout << camera->setResolution(1920, 1080);
    for (size_t i = 0; i < 10; i++)
    {
        cout << camera->showCamera("show");
    }
    cout << camera->saveImage("tp.jpg");
    unsigned char *data = new unsigned char[1920 * 1080*3];
    camera->getImageData(data, ImageType::BGR);
    camera->saveImageData(data, ImageType::BGR, "tp2.jpg");
    Mat img(480,640,CV_8UC3,data);
    cvtColor(img, img, CV_RGB2BGR);
    releaseCamera(camera);
    delete[] data;
    //getchar();
}