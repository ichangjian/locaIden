#include "usbCamera.h"
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class MyClass
{
public:
    MyClass();
    ~MyClass();

private:

};

MyClass::MyClass()
{
}

MyClass::~MyClass()
{
}

class Camera :public usbCamera
{
public:
    int openCamera(int index) override;//��������Ӧ������ͷ
    int setResolution(int width, int height) override;//��������ͷ�ֱ���
    int getImageData(unsigned char * data, ImageType imageType) override;//��ȡ��ǰ֡����
    int saveImageData(unsigned char * data, ImageType imageType, char * saveImageName) override;//���浱ǰ֡����
    int showCamera(char * windowsName) override;//��ʾ����ͷ
    int saveImage(char * saveImageName) override;//���浱ǰ֡ͼ��
    ~Camera(){ cout << "aaaaaaaaaaaa\n"; };
private:
    VideoCapture m_vc;
    Mat m_image;
    bool m_flag = false;
    int m_width;
    int m_height;
    
};

int Camera::openCamera(int index) //��������Ӧ������ͷ
{
    if (!m_vc.open(index))
    {
        m_flag = false;
        return -1;
    }
    m_flag = true;
    return 0;
}
int Camera::setResolution(int width, int height) //��������ͷ�ֱ���
{
    if (!m_flag)
    {
        return -1;
    }
    if (!m_vc.set(CAP_PROP_FRAME_WIDTH, width))
        return -1;
    if (!m_vc.set(CAP_PROP_FRAME_HEIGHT, height))
        return -1;
    m_width = width;
    m_height = height;
    return 0;
}
int Camera::getImageData(unsigned char * data, ImageType imageType) //��ȡ��ǰ֡����
{
    if (!m_flag)
    {
        return -1;
    }
    Mat image;
    m_vc >> image;
    if (image.empty())
    {
        return -1;
    }
    Mat dst;
    switch (imageType)
    {
    case RGB:
        cvtColor(image, dst, CV_BGR2RGB);
        break;
    case BGR:
        dst = image;
        break;
    case gray:
        cvtColor(image, dst, CV_BGR2GRAY);
        break;
    default:
        return -1;
    }
    memccpy(data, dst.data, sizeof(uchar), dst.rows*dst.cols*dst.channels());
    return 0;
}
int Camera::saveImageData(unsigned char * data, ImageType imageType, char * saveImageName) //���浱ǰ֡����
{
    Mat dst;
    switch (imageType)
    {
    case RGB:
        dst = Mat(m_height, m_width, CV_8UC3, data);
        cvtColor(dst, dst, CV_RGB2BGR);
        break;
    case BGR:
        dst = Mat(m_height, m_width, CV_8UC3, data);
        break;
    case gray:
        dst = Mat(m_height, m_width, CV_8UC1, data);
        break;
    default:
        return -1;
    }
    if (dst.empty())
    {
        return -1;
    }
    if (!imwrite(saveImageName, dst))
        return -1;
    return 0;
}
int Camera::showCamera(char * windowsName)//��ʾ����ͷ
{
    if (!m_flag)
    {
        return -1;
    }
    Mat image;
    m_vc >> image;
    if (image.empty())
    {
        return -1;
    }
    namedWindow(windowsName, WINDOW_AUTOSIZE);
    imshow(windowsName, image);
    waitKey(10);
    return 0;
}
int Camera::saveImage(char * saveImageName)//���浱ǰ֡ͼ��
{
    if (!m_flag)
    {
        return -1;
    }
    Mat image;
    m_vc >> image;
    if (image.empty())
    {
        return -1;
    }
    if (!imwrite(saveImageName, image))
        return -1;
    return 0;
}


LOCAPI void creatCamera(usbCamera** camera)
{
    *camera = new Camera();
}
LOCAPI void releaseCamera(usbCamera* camera)
{
    delete (Camera*)camera;
}