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
	BRIGHT = 0,//����
	CONTRAST=1,//�Աȶ�
	SATURATION=2,//���Ͷ�
	FRAME_WIDTH = 3,//֡��
	FRAME_HEIGHT=4,//֡��
	FLIPPING=5,//��ת��0-��ֱ��1-ˮƽ��2-ˮƽ+��ֱ
};


class usbCamera
{
public:
    virtual int openCamera(int index) = 0;//��������Ӧ������ͷ
	//virtual int setResolution(int width, int height) = 0;//��������ͷ�ֱ��� 
	virtual int setCameraParam(CameraParam param, int value) = 0;//��������ͷ����
	virtual int getCameraParam(CameraParam param) = 0;//��ȡ����ͷ����
    virtual int getImageData(unsigned char * data, ImageType imageType) = 0;//��ȡ��ǰ֡����
    virtual int saveImageData(unsigned char * data, ImageType imageType,char * saveImageName) = 0;//���浱ǰ֡����
    virtual int showCamera(char * windowsName) = 0;//��ʾ����ͷ
	virtual int saveImage(char * saveImageName) = 0;//���浱ǰ֡ͼ��
	virtual int showImage(unsigned char * data, ImageType imageType, char * windowsName) = 0;//��ʾͼ��
};


LOCAPI void creatCamera(usbCamera** camera);
LOCAPI void releaseCamera(usbCamera* camera);