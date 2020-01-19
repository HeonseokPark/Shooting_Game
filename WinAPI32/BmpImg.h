#pragma once
class BmpImg
{
public:

	typedef struct tagImage
	{
		HDC			hMemDC;		//�޸�DC
		HBITMAP		hBit;		//��Ʈ��(���ο� ��Ʈ�� �ڵ�)
		HBITMAP		hOBit;		//�õ� ��Ʈ��(���� ��Ʈ�� �ڵ�)
		int			width;		//�̹��� ����ũ��
		int			height;		//�̹��� ����ũ��

		int			maxFrameX;	//�ִ� x������ ����
		int			maxFrameY;	//�ִ� y������ ����
		int			FrameWidth;	//�����Ӵ� ����
		int			FrameHeight;//�����Ӵ� ����

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
	LPIMAGE_INFO	m_imageInfo;	//�̹��� ����
	_TCHAR*			m_fileName;		//�̹��� �̸�
	bool			m_isTranse;		//���� ���ٰ��ΰ�? (����Ÿ)
	COLORREF		m_transColor;	//���� ���� RGB(����Ÿ = RGB(255,0,255))

	LPIMAGE_INFO	m_imageInfo2;	//�̹��� ����
	BLENDFUNCTION	m_blendFunc;	//���ĺ��带 ���� ����

public:
	BmpImg();
	~BmpImg();

	//DC ������
	HDC getMemDC() { return m_imageInfo->hMemDC; }

	//�� ��Ʈ�� �ʱ�ȭ(Backdc��)
	HRESULT init(int width, int height);
	//�̹��� ���� �ʱ�ȭ
	HRESULT init(const _TCHAR* fileName, int width, int height,
		bool isTrans = false, COLORREF transeColor = RGB(255, 0, 255));
	//�̹��� ���� �ʱ�ȭ(������ �߰�)
	HRESULT init(const _TCHAR* fileName, int width, int height,int MaxFrameX, int MaxFrameY,
		bool isTrans = false, COLORREF transeColor = RGB(255, 0, 255));

	//����Ű ����
	void setTransColor(bool isTrans, COLORREF transColor);

	//������
	void release();

	//����(���� ������ ��ġ�� �̹����� ���)
	void render(HDC hdc);
	void render(HDC hdc, int destX, int destY);
	void render(HDC hdc, int destX, int destY , int destWidth, int destHeight,
		int sourX = 0, int sourY = 0 ,int sourWidth = 0, int sourHeight = 0);

	//���� ����
	void alphaRender(HDC hdc, BYTE alpha);
	void alphaRender(HDC hdc, BYTE alpha, int destX, int destY);
	void alphaRender(HDC hdc, BYTE alpha, int destX, int destY, int destWidth, int destHeight,
		int sourX = 0, int sourY = 0, int sourWidth = 0, int sourHeight = 0);

	//������ ����
	void frameRender(HDC hdc, int frameX, int frameY, int destX, int destY, int destWidth, int destHeight);
	void frameAlphaRender(HDC hdc, BYTE alpha, int frameX, int frameY, int destX, int destY, int destWidth, int destHeight);
	
	//���� ����
	void loopRender(HDC hdc, int destX, int destY, int sourX, int sourY);
	void loopAlphaRender(HDC hdc, BYTE alpha, int destX, int destY, int sourX, int sourY);

	//������Ʈ ����
	void rotateRender(HDC hdc, float radian, bool reverse, float multiple, int destX, int destY);
	void frameRotateRender(HDC hdc, float radian, bool reverse, float multiple, int frameX, int frameY, int destX, int destY);

};

