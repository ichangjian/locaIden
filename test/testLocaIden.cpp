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
	testIden();


    for (size_t i = 0; i < 100; i++)
    {
        cout << "\nkai shi\n";
        //testCamera();
    }
    return 0;
}


void testIden()
{
    //float *kx, float *ky, float *d;
    Mat img = imread("H:/iden/Î¢ÐÅÍ¼Æ¬_20180727153530.jpg");
    float unit = 0;
    loca::Rect rectRegion{ 864, 648, 864, 648, };
    //loca::getCheckerboardUnitLength(img.data, img.cols, img.rows, rectRegion,10, &unit, true);
    cout << unit << endl;
    loca::setIdenImage(img.data, img.cols, img.rows, loca::CardType::CARD_UP);
    loca::Point2f pt;
    rectRegion = { 800, 200, 1400, 600 };
    //rectRegion = { 900, 50, 800, 300 };   
    float angel = 0;
	loca::getIdenRectangleRC(rectRegion, loca::RectangleType::RECT_HORIZONTAL, 220,120,  &angel, &pt, true);
    circle(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), 10, Scalar(0, 0, 255));
    line(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), cv::Point2f(rectRegion.x + pt.x + 1000, rectRegion.y + pt.y - tan(angel*3.1415926 / 180)*(1000)), Scalar(255, 0, 0), 2);
    line(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), cv::Point2f(rectRegion.x + pt.x - tan((angel)*3.1415926 / 180)*(1000), rectRegion.y + pt.y - 1000), Scalar(255, 0, 0), 2);
    cout << "angel\t" << angel << "\t" << endl;

	rectRegion = {1000, 800, 800, 1400 };// rectRegion = { 1700, 700, 600, 1400 };
    loca::getIdenRectangleRC(rectRegion, loca::RectangleType::RECT_VERTICAL, 52, 1020, &angel, &pt, true);
    circle(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), 10, Scalar(0, 0, 255));
    line(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), cv::Point2f(rectRegion.x + pt.x + 1000, rectRegion.y + pt.y - tan(angel*3.1415926 / 180)*(1000)), Scalar(255, 0, 0), 2);
    line(img, cv::Point2f(rectRegion.x + pt.x, rectRegion.y + pt.y), cv::Point2f(rectRegion.x + pt.x - tan((angel)*3.1415926 / 180)*(1000), rectRegion.y + pt.y - 1000), Scalar(255, 0, 0), 2);
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
	cout << camera->openCamera(0);
	cout << camera->setCameraParam(CameraParam::FRAME_WIDTH, 640);
	cout << camera->setCameraParam(CameraParam::FRAME_HEIGHT, 480);

    for (size_t i = 0; i < 10; i++)
    {
		cout << i;
		cout << camera->setCameraParam(CameraParam::FLIPPING, 0);
		cout << camera->showCamera("show1");
		cout << camera->setCameraParam(CameraParam::FLIPPING, 1);
		cout << camera->showCamera("show2");
    }
    cout << camera->saveImage("tp.jpg");
	unsigned char *data = new unsigned char[640 * 480*3]();
	camera->getImageData(data, ImageType::BGR);
	camera->saveImageData(data, ImageType::BGR, "tp2.jpg");
	for (size_t i = 0; i < 10; i++)
	{
		camera->showImage(data, ImageType::BGR, "image");

	}
    releaseCamera(camera);
    delete[] data;
    //getchar();
}