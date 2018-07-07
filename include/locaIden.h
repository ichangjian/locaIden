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
    //ӡˢ������ͣ��ϡ��С���
    enum CardType
    {
        CARD_UP = 0,
        CARD_MIDDLE = 1,
        CARD_DOWN = 2,

    };

    //���ε����ͣ�ˮƽ����ֱ
    enum RectangleType
    {
        RECT_HORIZONTAL = 0,
        RECT_VERTICAL = 1,

    };

    //�������̸��ȡͼ�����ص�ʵ�ʾ���ĵ�λ
    //imageData ���Ҷ�ͼ���ַ
    //imageWidth ��ͼ���
    //imageHeight ��ͼ���
    //rectRegion ,���������̸����ڵ�����
    //boardSize �����̸��Ϻڷ���ĳߴ�mm
    //unit ��ת����λ(����/mm)
    //saveImage ���������ͼ��
    //return������ֵ0 ������-1 ����
    LOCAPI int getCheckerboardUnitLength(unsigned char * imageData, int imageWidth, int imageHeight, Rect rectRegion, int boardSize, float *unit, bool saveImage = false);

    //����ӡˢ��ͼ��
    //imageData ���Ҷ�ͼ���ַ
    //imageWidth ��ͼ���
    //imageHeight ��ͼ���
    //cardType ��ӡˢ������
    //return������ֵ0 ������-1 ����
    LOCAPI int setIdenImage(unsigned char * imageData, int imageWidth, int imageHeight, CardType cardType);

    //��ȡ�����ĸ��ǵ�ͼ������
    //rectRegion ,�������ڵ�����
    //rectType �����ε�����
    //postion[4],�����ĸ��ǵ�x��y���꣬�����Ͻǿ�ʼ˳ʱ������
    //saveImage ���������ͼ��
    //return������ֵ0 ������-1 ����
    LOCAPI int getIdenRectanglePosition(loca::Rect rectRegion, RectangleType rectType, Point2f postion[4], bool saveImage = false);


    //��ȡ��������ͼ�����꼰��ˮƽ�ߵ�ƫ��
    //rectRegion ,�������ڵ�����
    //rotation ��������ˮƽ�ߵ�ƫ��,��λ�㣬ˮƽ������Ϊ��������Ϊ��
    //centre,��������ͼ�����꣬��rectRegion �����Ͻ�Ϊԭ��
    //saveImage ���������ͼ��
    //return������ֵ0 ������-1 ����
    LOCAPI int getIdenRectangleRC(Rect rectRegion, RectangleType rectType, float* rotation, Point2f* centre, bool saveImage = false);

    //��ȡ����
    //return������ֵ0 ������-1 ����
    LOCAPI int getIdenSerialNumber(char number[8]);

}