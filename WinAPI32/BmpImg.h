#pragma once
class BmpImg
{
public:

	typedef struct tagImage
	{
		HDC			hMemDC;		//메모리DC
		HBITMAP		hBit;		//비트맵(새로운 비트맵 핸들)
		HBITMAP		hOBit;		//올드 비트맵(기존 비트맵 핸들)
		int			width;		//이미지 가로크기
		int			height;		//이미지 세로크기

		int			maxFrameX;	//최대 x프레임 갯수
		int			maxFrameY;	//최대 y프레임 갯수
		int			FrameWidth;	//프레임당 길이
		int			FrameHeight;//프레임당 높이

		tagImage()
		{
			hMemDC = nullptr;
			hBit = nullptr;
			hOBit = nullptr;
			width = 0;
			height = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			FrameWidth = 0;
			FrameHeight = 0;
		}
	}IMAGE_INFO, *LPIMAGE_INFO;

private:
	LPIMAGE_INFO	m_imageInfo;	//이미지 정보
	_TCHAR*			m_fileName;		//이미지 이름
	bool			m_isTranse;		//배경색 없앨것인가? (마젠타)
	COLORREF		m_transColor;	//배경색 없앨 RGB(마젠타 = RGB(255,0,255))

	LPIMAGE_INFO	m_imageInfo2;	//이미지 정보
	BLENDFUNCTION	m_blendFunc;	//알파블렌드를 위한 정보

public:
	BmpImg();
	~BmpImg();

	//DC 얻어오기
	HDC getMemDC() { return m_imageInfo->hMemDC; }

	//빈 비트맵 초기화(Backdc용)
	HRESULT init(int width, int height);
	//이미지 파일 초기화
	HRESULT init(const _TCHAR* fileName, int width, int height,
		bool isTrans = false, COLORREF transeColor = RGB(255, 0, 255));
	//이미지 파일 초기화(프레임 추가)
	HRESULT init(const _TCHAR* fileName, int width, int height,int MaxFrameX, int MaxFrameY,
		bool isTrans = false, COLORREF transeColor = RGB(255, 0, 255));

	//투명키 셋팅
	void setTransColor(bool isTrans, COLORREF transColor);

	//릴리즈
	void release();

	//렌더(내가 지정한 위치에 이미지를 출력)
	void render(HDC hdc);
	void render(HDC hdc, int destX, int destY);
	void render(HDC hdc, int destX, int destY , int destWidth, int destHeight,
		int sourX = 0, int sourY = 0 ,int sourWidth = 0, int sourHeight = 0);

	//알파 렌더
	void alphaRender(HDC hdc, BYTE alpha);
	void alphaRender(HDC hdc, BYTE alpha, int destX, int destY);
	void alphaRender(HDC hdc, BYTE alpha, int destX, int destY, int destWidth, int destHeight,
		int sourX = 0, int sourY = 0, int sourWidth = 0, int sourHeight = 0);

	//프레임 렌더
	void frameRender(HDC hdc, int frameX, int frameY, int destX, int destY, int destWidth, int destHeight);
	void frameAlphaRender(HDC hdc, BYTE alpha, int frameX, int frameY, int destX, int destY, int destWidth, int destHeight);
	
	//루프 렌더
	void loopRender(HDC hdc, int destX, int destY, int sourX, int sourY);
	void loopAlphaRender(HDC hdc, BYTE alpha, int destX, int destY, int sourX, int sourY);

	//로테이트 렌더
	void rotateRender(HDC hdc, float radian, bool reverse, float multiple, int destX, int destY);
	void frameRotateRender(HDC hdc, float radian, bool reverse, float multiple, int frameX, int frameY, int destX, int destY);

};

