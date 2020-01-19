#pragma once
#include "SingletonBase.h"


class ImageManager :public SingletonBase<ImageManager>
{
private:
	//			Ű,���
	typedef map<string, BmpImg*>			ImgMap;
	typedef map<string, BmpImg*>::iterator	ImgIter;

	ImgMap	m_ImgMap;

	//��dc�� �޸� dc�� �׷����� �ʴ� ��Ʈ�� dc
	BmpImg* m_BackDC;
	//����Ʈ dc �� ���� �ڵ鿡�� �޾ƿ� �׸����ִ� dc
	HDC		m_FrontDC;


public:
	ImageManager();
	~ImageManager();

	//�޸� DC ���
	HDC getMemDC() { return m_BackDC->getMemDC(); }
	//HDC ���
	HDC getHDC() { return m_FrontDC; }
	//Ű������ �̹��� ���� �ʱ�ȭ
	BmpImg* addImage(string strKey, const _TCHAR* fileName, int width, int height,
		bool isTrans = false, COLORREF transColor = RGB(255, 0, 255));
	//Ű������ �̹��� ���� �ʱ�ȭ(�������߰�)
	BmpImg* addImage(string strKey, const _TCHAR* fileName, int width, int height,
		int MaxFrameX, int MaxFrameY, bool isTrans = false, COLORREF transColor = RGB(255, 0, 255));
	//Ű������ �̹��� ã��
	BmpImg* findImage(string strKey);
	//Ű������ �̹��� ����
	bool deleteImage(string strKey);
	//�̹��� ��ü ����
	bool deleteAll();
	//Ű���� ��� ���� �����ֱ�
	void render(string strKey, HDC hdc);
	void render(string strKey, HDC hdc, float destX, float DestY);
	void render(string strKey, HDC hdc, float destX, float destY, int destWidth, int destHeight,
		int sourX = 0, int sourY = 0, int sourWidth = 0, int sourHeight = 0);

	//���� ����
	void alphaRender(string strKey, HDC hdc, BYTE alpha);
	void alphaRender(string strKey, HDC hdc, BYTE alpha, float destX, float destY);
	void alphaRender(string strKey, HDC hdc, BYTE alpha, float destX, float destY, int destWidth, int destHeight,
		int sourX = 0, int sourY = 0, int sourWidth = 0, int sourHeight = 0);

	//������ ����

	void frameRender(string strKey, HDC hdc, int frameX, int frameY, float destX, float destY, int destWidth, int destHeight);
	void frameAlphaRender(string strKey, HDC hdc, BYTE alpha, int frameX, int frameY, float destX, float destY, int destWidth, int destHeight);


	//���� ����
	void loopRender(string strKey, HDC hdc, float destX, float destY, int sourX, int sourY);
	void loopAlphaRender(string strKey, HDC hdc, BYTE alpha, float destX, float destY, int sourX, int sourY);

	//������Ʈ ����
	void rotateRender(string strKey, HDC hdc, float radian, bool reverse, float multiple, float destX, float destY);
	void frameRotateRender(string strKey, HDC hdc, float radian, bool reverse, float multiple, int frameX, int frameY, float destX, float destY);



};

