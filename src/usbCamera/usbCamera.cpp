#include "usbCamera.h"
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;


class Camera :public usbCamera
{
public:
    int openCamera(int index) override;//打开索引对应的摄像头
    int setResolution(int width, int height) ;//设置摄像头分辨率
	int setCameraParam(CameraParam param, int value)override;//设置摄像头参数
	int getCameraParam(CameraParam param)override;//设置摄像头参数
    int getImageData(unsigned char * data, ImageType imageType) override;//获取当前帧数据
    int saveImageData(unsigned char * data, ImageType imageType, char * saveImageName) override;//保存当前帧数据
    int showCamera(char * windowsName) override;//显示摄像头
    int saveImage(char * saveImageName) override;//保存当前帧图像
	int showImage(unsigned char * data, ImageType imageType, char * windowsName) override;//显示图像
private:
	int setFlip(Mat &image);
    VideoCapture m_vc;
    Mat m_image;
    bool m_flag = false;
    int m_width;
    int m_height;
	int m_flip = -1;
};

int Camera::openCamera(int index) //打开索引对应的摄像头
{
    if (!m_vc.open(index))
    {
        m_flag = false;
        return -1;
    }
	m_flag = true;
	m_width = m_vc.get(CAP_PROP_FRAME_WIDTH);
	m_height = m_vc.get(CAP_PROP_FRAME_HEIGHT);
    return 0;
}
int Camera::setResolution(int width, int height) //设置摄像头分辨率
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

int Camera::setCameraParam(CameraParam param, int value)//设置摄像头参数
{
	if (!m_flag)
	{
		return -1;
	}
	bool re = true;
	switch (param)
	{
	case BRIGHT:
		re = m_vc.set(CAP_PROP_BRIGHTNESS, value);
		break;
	case CONTRAST:
		re = m_vc.set(CAP_PROP_CONTRAST, value);
		break;
	case SATURATION:
		re = m_vc.set(CAP_PROP_SATURATION, value);
		break;
	case FRAME_WIDTH:
		re = m_vc.set(CAP_PROP_FRAME_WIDTH, value);
		m_width = value;
		break;
	case FRAME_HEIGHT:
		re = m_vc.set(CAP_PROP_FRAME_HEIGHT, value);
		m_height = value;
		break;
	case FLIPPING:
		m_flip = value;
		break;
	default:
		return -1;
	}
	if (!re)
	{
		return -1;
	}
	return 0;
}
int Camera::getCameraParam(CameraParam param)//设置摄像头参数
{
	if (!m_flag)
	{
		return -1;
	} 
	int value = 0;
	switch (param)
	{
	case BRIGHT:
		value = m_vc.get(CAP_PROP_BRIGHTNESS);
		break;
	case CONTRAST:
		value = m_vc.get(CAP_PROP_CONTRAST);
		break;
	case SATURATION:
		value = m_vc.get(CAP_PROP_SATURATION);
		break;
	case FRAME_WIDTH:
		value = m_vc.get(CAP_PROP_FRAME_WIDTH);
		break;
	case FRAME_HEIGHT:
		value = m_vc.get(CAP_PROP_FRAME_HEIGHT);
		break;
	case FLIPPING:
		value=m_flip;
		break;
	default:
		return -1;
	}
 
	return value;
}

int Camera::setFlip(Mat &image)
{
	switch (m_flip)
	{
	case 0:
		flip(image,image,0);
		break;
	case 1:
		flip(image, image, 1);
		break;
	case 2:
		flip(image, image, -1);
		break;
	default:
		break;
	}
	return 0;
}

int Camera::getImageData(unsigned char * data, ImageType imageType) //获取当前帧数据
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
	setFlip(image);
    Mat *dst;
    switch (imageType)
    {
    case RGB:
		dst = new  Mat(m_height, m_width, CV_8UC3, data);
        cvtColor(image, *dst, CV_BGR2RGB);
        break;
	case BGR:
		dst = new Mat(m_height, m_width, CV_8UC3, data);
        image.copyTo(*dst) ;
        break;
	case GRAY:
		dst = new Mat(m_height, m_width, CV_8UC1, data);
        cvtColor(image, *dst, CV_BGR2GRAY);
        break;
    default:
        return -1;
	}

	//memccpy(data, dst.data, sizeof(uchar), dst.rows*dst.cols*dst.channels());
	//memccpy(data, dst.data, sizeof(uchar), dst.rows*dst.cols*dst.channels());
	//imwrite("dst.jpg",*dst);
    return 0;
}
int Camera::saveImageData(unsigned char * data, ImageType imageType, char * saveImageName) //保存当前帧数据
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
	case GRAY:
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
int Camera::showImage(unsigned char * data, ImageType imageType, char * windowsName) //保存当前帧数据
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
	case GRAY:
		dst = Mat(m_height, m_width, CV_8UC1, data);
		break;
	default:
		return -1;
	}
	if (dst.empty())
	{
		return -1;
	}
	namedWindow(windowsName, WINDOW_AUTOSIZE);
	imshow(windowsName, dst);
	waitKey(50);
	return 0;
}
int Camera::showCamera(char * windowsName)//显示摄像头
{
    if (!m_flag)
    {
        return -1;
    }
    Mat image;
	m_vc >> image;
	setFlip(image);
    if (image.empty())
    {
        return -1;
    }
    namedWindow(windowsName, WINDOW_AUTOSIZE);
    imshow(windowsName, image);
    waitKey(50);
    return 0;
}
int Camera::saveImage(char * saveImageName)//保存当前帧图像
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
	setFlip(image);
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