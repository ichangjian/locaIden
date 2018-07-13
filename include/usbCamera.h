#pragma once
#include <stdio.h> 
#define LOCAPI extern "C" _declspec(dllexport) 

enum ImageType
{
    RGB = 0,
    BGR = 1,
    gray = 2,
};

class usbCamera
{
public:
    virtual int openCamera(int index) = 0;//打开索引对应的摄像头
    virtual int setResolution(int width,int height) = 0;//设置摄像头分辨率
    virtual int getImageData(unsigned char * data, ImageType imageType) = 0;//获取当前帧数据
    virtual int saveImageData(unsigned char * data, ImageType imageType,char * saveImageName) = 0;//保存当前帧数据
    virtual int showCamera(char * windowsName) = 0;//显示摄像头
    virtual int saveImage(char * saveImageName) = 0;//保存当前帧图像
};


LOCAPI void creatCamera(usbCamera** camera);
LOCAPI void releaseCamera(usbCamera* camera);