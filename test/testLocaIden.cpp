#include "locaIden.h"
#include <tchar.h>
#include <iostream>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

 
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