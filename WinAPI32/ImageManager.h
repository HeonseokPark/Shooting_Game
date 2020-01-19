#pragma once
#include "SingletonBase.h"


class ImageManager :public SingletonBase<ImageManager>
{
private:
	//			키,밸류
	typedef map<string, BmpImg*>			ImgMap;
	typedef map<string, BmpImg*>::iterator	ImgIter;

	ImgMap	m_ImgMap;

	//백dc는 메모리 dc로 그려지지 않는 비트맵 dc
	BmpImg* m_BackDC;
	//프론트 dc 는 현재 핸들에서 받아온 그릴수있는 dc
	HDC		m_FrontDC;


public:
	ImageManager();
	~ImageManager();

	//메모리 DC 얻기
	HDC getMemDC() { return m_BackDC->getMemDC(); }
	//HDC 얻기
	HDC getHDC() { return m_FrontDC; }
	//키값으로 이미지 파일 초기화
	BmpImg* addImage(string strKey, const _TCHAR* fileName, int width, int height,
		bool isTrans = false, COLORREF transColor = RGB(255, 0, 255));
	//키값으로 이미지 파일 초기화(프레임추가)
	BmpImg* addImage(string strKey, const _TCHAR* fileName, int width, int height,
		int MaxFrameX, int MaxFrameY, bool isTrans = false, COLORREF transColor = RGB(255, 0, 255));
	//키값으로 이미지 찾기
	BmpImg* findImage(string strKey);
	//키값으로 이미지 삭제
	bool deleteImage(string strKey);
	//이미지 전체 삭제
	bool deleteAll();
	//키값의 밸류 렌더 시켜주기
	void render(string strKey, HDC hdc);
	void render(string strKey, HDC hdc, float destX, float DestY);
	void render(string strKey, HDC hdc, float destX, float destY, int destWidth, int destHeight,
		int sourX = 0, int sourY = 0, int sourWidth = 0, int sourHeight = 0);

	//알파 렌더
	void alphaRender(string strKey, HDC hdc, BYTE alpha);
	void alphaRender(string strKey, HDC hdc, BYTE alpha, float destX, float destY);
	void alphaRender(string strKey, HDC hdc, BYTE alpha, float destX, float destY, int destWidth, int destHeight,
		int sourX = 0, int sourY = 0, int sourWidth = 0, int sourHeight = 0);

	//프레임 렌더

	void frameRender(string strKey, HDC hdc, int frameX, int frameY, float destX, float destY, int destWidth, int destHeight);
	void frameAlphaRender(string strKey, HDC hdc, BYTE alpha, int frameX, int frameY, float destX, float destY, int destWidth, int destHeight);


	//루프 렌더
	void loopRender(string strKey, HDC hdc, float destX, float destY, int sourX, int sourY);
	void loopAlphaRender(string strKey, HDC hdc, BYTE alpha, float destX, float destY, int sourX, int sourY);

	//로테이트 렌더
	void rotateRender(string strKey, HDC hdc, float radian, bool reverse, float multiple, float destX, float destY);
	void frameRotateRender(string strKey, HDC hdc, float radian, bool reverse, float multiple, int frameX, int frameY, float destX, float destY);



};

