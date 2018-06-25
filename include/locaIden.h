#pragma once
#include <stdio.h> 
#ifdef LOCADLL
#define LOCAPI //extern "C" _declspec(dllimport) 
#else
#define LOCAPI extern "C" _declspec(dllexport) 
#endif

//印刷板的类型，上、中、下
enum CardType
{
    CARD_UP = 0,
    CARD_MIDDLE = 1,
    CARD_DOWN = 2,

};

//矩形的类型，水平、竖直
enum RectangleType
{
    RECT_HORIZONTAL = 0,
    RECT_VERTICAL = 1,

};

//跟据棋盘格获取图像像素到实际距离的单位
//imageData ，图像地址
//imageWidth ，图像宽
//imageHeight ，图像高
//checkerboardRow ，棋盘格黑块行数
//checkerboardCol ，棋盘格黑块列数
//boardSize ，棋盘格上黑方块的尺寸mm
//unit ，转换单位
LOCAPI int getCheckerboardUnitLength(unsigned char * imageData, int imageWidth, int imageHeight, int checkerboardRow, int checkerboardCol, int boardSize, float *unit);

//设置印刷板图像
//imageData ，图像地址
//imageWidth ，图像宽
//imageHeight ，图像高
//cardType ，印刷板类型
LOCAPI int setIdenImage(unsigned char * imageData, int imageWidth, int imageHeight, CardType cardType);

//获取矩形四个角的图上坐标
//rectType ，矩形的类型
//postionX[4], postionY[4],矩形四个角的x、y坐标，以左上角开始顺时针排列
LOCAPI int getIdenRectanglePosition(RectangleType rectType, float postionX[4], float postionY[4]);

//获取数字
LOCAPI int getIdenSerialNumber(char number[8]);

