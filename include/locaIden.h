#pragma once
#include <stdio.h> 
#define LOCAPI extern "C" _declspec(dllexport) 

namespace loca{

    struct Point2f
    {
        float x;
        float y;
    };

    struct Rect
    {
        float x;
        float y;
        float with;
        float height;

    };
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
    //imageData ，灰度图像地址
    //imageWidth ，图像宽
    //imageHeight ，图像高
    //rectRegion ,需计算的棋盘格所在的区域
    //boardSize ，棋盘格上黑方块的尺寸mm
    //unit ，转换单位(像素/mm)
    //saveImage ，保存过程图像
    //return，返回值0 正常，-1 出错
    LOCAPI int getCheckerboardUnitLength(unsigned char * imageData, int imageWidth, int imageHeight, Rect rectRegion, int boardSize, float *unit, bool saveImage = false);

    //设置印刷板图像
    //imageData ，灰度图像地址
    //imageWidth ，图像宽
    //imageHeight ，图像高
    //cardType ，印刷板类型
    //return，返回值0 正常，-1 出错
    LOCAPI int setIdenImage(unsigned char * imageData, int imageWidth, int imageHeight, CardType cardType);

    //获取矩形四个角的图上坐标
    //rectRegion ,矩形所在的区域
    //rectType ，矩形的类型
    //postion[4],矩形四个角的x、y坐标，以左上角开始顺时针排列
    //saveImage ，保存过程图像
    //return，返回值0 正常，-1 出错
    LOCAPI int getIdenRectanglePosition(loca::Rect rectRegion, RectangleType rectType, Point2f postion[4], bool saveImage = false);


    //获取矩形中心图上坐标及与水平线的偏角
    //rectRegion ,矩形所在的区域
    //rotation ，矩形与水平线的偏角,单位°，水平线往上为正，往下为负
    //centre,矩形中心图上坐标，以rectRegion 的左上角为原点
    //saveImage ，保存过程图像
    //return，返回值0 正常，-1 出错
    LOCAPI int getIdenRectangleRC(Rect rectRegion, RectangleType rectType, float* rotation, Point2f* centre, bool saveImage = false);

    //获取数字
    //return，返回值0 正常，-1 出错
    LOCAPI int getIdenSerialNumber(char number[8]);

}