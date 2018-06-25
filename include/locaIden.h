#pragma once
#include <stdio.h> 
#ifdef LOCADLL
#define LOCAPI //extern "C" _declspec(dllimport) 
#else
#define LOCAPI extern "C" _declspec(dllexport) 
#endif

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
//imageData ��ͼ���ַ
//imageWidth ��ͼ���
//imageHeight ��ͼ���
//checkerboardRow �����̸�ڿ�����
//checkerboardCol �����̸�ڿ�����
//boardSize �����̸��Ϻڷ���ĳߴ�mm
//unit ��ת����λ
LOCAPI int getCheckerboardUnitLength(unsigned char * imageData, int imageWidth, int imageHeight, int checkerboardRow, int checkerboardCol, int boardSize, float *unit);

//����ӡˢ��ͼ��
//imageData ��ͼ���ַ
//imageWidth ��ͼ���
//imageHeight ��ͼ���
//cardType ��ӡˢ������
LOCAPI int setIdenImage(unsigned char * imageData, int imageWidth, int imageHeight, CardType cardType);

//��ȡ�����ĸ��ǵ�ͼ������
//rectType �����ε�����
//postionX[4], postionY[4],�����ĸ��ǵ�x��y���꣬�����Ͻǿ�ʼ˳ʱ������
LOCAPI int getIdenRectanglePosition(RectangleType rectType, float postionX[4], float postionY[4]);

//��ȡ����
LOCAPI int getIdenSerialNumber(char number[8]);

