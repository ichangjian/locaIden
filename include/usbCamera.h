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
    virtual int openCamera(int index) = 0;//��������Ӧ������ͷ
    virtual int setResolution(int width,int height) = 0;//��������ͷ�ֱ���
    virtual int getImageData(unsigned char * data, ImageType imageType) = 0;//��ȡ��ǰ֡����
    virtual int saveImageData(unsigned char * data, ImageType imageType,char * saveImageName) = 0;//���浱ǰ֡����
    virtual int showCamera(char * windowsName) = 0;//��ʾ����ͷ
    virtual int saveImage(char * saveImageName) = 0;//���浱ǰ֡ͼ��
};


LOCAPI void creatCamera(usbCamera** camera);
LOCAPI void releaseCamera(usbCamera* camera);