#pragma once
#include <stdio.h> 
#define LOCAPI extern "C" _declspec(dllexport) 

enum ImageType
{
    RGB = 0,
    BGR = 1,
    GRAY = 2,
};

enum CameraParam
{
	BRIGHT = 0,//亮度
	CONTRAST=1,//对比度
	SATURATION=2,//饱和度
	FRAME_WIDTH = 3,//帧宽
	FRAME_HEIGHT=4,//帧高
	FLIPPING=5,//翻转，0-垂直，1-水平，2-水平+垂直
};


class usbCamera
{
public:
    virtual int openCamera(int index) = 0;//打开索引对应的摄像头
	//virtual int setResolution(int width, int height) = 0;//设置摄像头分辨率 
	virtual int setCameraParam(CameraParam param, int value) = 0;//设置摄像头参数
	virtual int getCameraParam(CameraParam param) = 0;//获取摄像头参数
    virtual int getImageData(unsigned char * data, ImageType imageType) = 0;//获取当前帧数据
    virtual int saveImageData(unsigned char * data, ImageType imageType,char * saveImageName) = 0;//保存当前帧数据
    virtual int showCamera(char * windowsName) = 0;//显示摄像头
	virtual int saveImage(char * saveImageName) = 0;//保存当前帧图像
	virtual int showImage(unsigned char * data, ImageType imageType, char * windowsName) = 0;//显示图像
};


LOCAPI void creatCamera(usbCamera** camera);
LOCAPI void releaseCamera(usbCamera* camera);