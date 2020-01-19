#include "stdafx.h"
#include "BmpImg.h"
#pragma comment(lib,"msimg32.lib")//���ĺ��带 ����ϱ� ���ؼ� ���̺귯�� �߰�

BmpImg::BmpImg()
	:m_imageInfo(nullptr)
	, m_imageInfo2(nullptr)
	, m_fileName(nullptr)
	, m_isTranse(NULL)
	, m_transColor(RGB(0, 0, 0))
{
}


BmpImg::~BmpImg()
{
}

//CreateCompatibleDC
//��Ʈ���� ����ϱ� ���ؼ� �޸�DC�� ����� �ִ� �Լ�
//ȭ�� DC�� �޸� DC ȣȯ

//CreateCompatibleBitmap
//���� DC�� ȣȯ�Ǵ� ��Ʈ���� ������ �ִ� �Լ�

//SelectObject
//�⺻�� ��Ʈ�� �ڵ��� ��ȯ�ϰ�, ���ο� ��Ʈ���� �޸� DC�� �����ϰ�
//�޸� DC�� ��Ʈ���� �׷���, ȭ�� DC �ƴϹǷ� ����Ϳ� ��� x

//�õ� ��Ʈ��, ��Ʈ�� ���� ������
//���� ���õǾ� �ִ� DC�� GDI ������Ʈ�� ����,���� �� �� ���� �����̴�(������ ������ġ)
//���� ����� ���� ������Ʈ�� �ٲ��ְų� 
//���� ������Ʈ�� �ٲ� �� �� ���� �ؾ��Ѵ�

HRESULT BmpImg::init(int width, int height)
{
	//���ʱ�ȭ ������, �̹��� ������ ���� ��� �մٸ� ������ ���� ���� ��
	if (m_imageInfo != nullptr) release();

	//DC ��������(���� �����쿡 ���� ȭ��DC�� �ӽ÷� �޾ƿ�)
	HDC hdc = GetDC(g_hwnd);

	//�̹��� ���� �����ϱ�
	m_imageInfo = new IMAGE_INFO;
	//��Ʈ�� dc ȣȯ�ǰ� �����
	m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	//���� ���� ��Ʈ��
	m_imageInfo->hBit = static_cast<HBITMAP>(CreateCompatibleBitmap(hdc, width, height));
	//DC���Լ� ������ �޾ƿ���, ���� ����� ��. ���������� �õ� ��Ʈ���̶� �س��� ����
	m_imageInfo->hOBit = static_cast<HBITMAP>(SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit));
	m_imageInfo->width = width;
	m_imageInfo->height = height;

	//�̹��� ���� �����ϱ�
	m_imageInfo2 = new IMAGE_INFO;
	//��Ʈ�� DC ȣȯ�Ǵ°� �����
	m_imageInfo2->hMemDC = CreateCompatibleDC(hdc);
	//���� ���� ��Ʈ��
	m_imageInfo2->hBit = static_cast<HBITMAP>(CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY));
	//DC���Լ� ������ �޾ƿ���, ���� ����� ��. ���������� �õ� ��Ʈ���̶� �س��� ����
	m_imageInfo2->hOBit = static_cast<HBITMAP>(SelectObject(m_imageInfo2->hMemDC, m_imageInfo2->hBit));
	m_imageInfo2->width = WINSIZEX;
	m_imageInfo2->height = WINSIZEY;
	
	//���� ���� �ɼ�
	m_blendFunc.BlendFlags = 0;
	m_blendFunc.AlphaFormat = 0; // 32��Ʈ�ΰ�� AC_SRC_ALPHA �� �����ϰ� �׿ܴ� 0���� ����
	m_blendFunc.BlendOp = AC_SRC_OVER;



	//�����̸�
	m_fileName = nullptr;

	//����Ű �÷� ����
	m_isTranse = false;
	m_transColor = RGB(0, 0, 0);

	//���ҽ��� �����µ� ���� ���� ��
	if (m_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//DC ����
	ReleaseDC(g_hwnd, hdc);

	return S_OK;
}

HRESULT BmpImg::init(const _TCHAR* fileName, int width, int height, bool isTrans, COLORREF transeColor)
{
	//���ʱ�ȭ ������, �̹��� ������ ���� ��� �մٸ� ������ ���� ���ٰ�
	if (m_imageInfo != nullptr) release();

	//DC ��������(���� �����쿡 ���� ȭ��DC�� �ӽ÷� �޾ƿ�)
	HDC hdc = GetDC(g_hwnd);

	//�̹��� ���� �����ϱ�
	m_imageInfo = new IMAGE_INFO;
	m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	m_imageInfo->hBit = static_cast<HBITMAP>(LoadImage(g_hInst, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE));
	m_imageInfo->hOBit = static_cast<HBITMAP>(SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit));
	m_imageInfo->width = width;
	m_imageInfo->height = height;


	//�̹��� ���� �����ϱ�
	m_imageInfo2 = new IMAGE_INFO;
	//��Ʈ�� dc ȣȯ�Ǵ°� �����
	m_imageInfo2->hMemDC = CreateCompatibleDC(hdc);
	//���� ���� ��Ʈ��
	m_imageInfo2->hBit = static_cast<HBITMAP>(CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY));
	//dc���Լ� ������ �޾ƿ��� ���� ����� �� ���������� �õ� ��Ʈ���̶� �س��� ����
	m_imageInfo2->hOBit = static_cast<HBITMAP>(SelectObject(m_imageInfo2->hMemDC, m_imageInfo2->hBit));
	m_imageInfo2->width = WINSIZEX;
	m_imageInfo2->height = WINSIZEY;

	//���� ���� �ɼ�
	m_blendFunc.BlendFlags = 0;
	m_blendFunc.AlphaFormat = 0; // 32��Ʈ�ΰ�� AC_SRC_ALPHA �� �����ϰ� �׿ܴ� 0���� ����
	m_blendFunc.BlendOp = AC_SRC_OVER;


	//���� �̸�
	int len = _tcslen(fileName);
	m_fileName = new _TCHAR[len + 1];
	_tcscpy_s(m_fileName, len + 1, fileName);

	//����Ű �÷� ����
	m_isTranse = isTrans;
	m_transColor = transeColor;

	//���ҽ��� �����µ� ���� ���� ��
	if (m_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//DC ����
	ReleaseDC(g_hwnd, hdc);

	return S_OK;
}

HRESULT BmpImg::init(const _TCHAR * fileName, int width, int height, int MaxFrameX, int MaxFrameY, bool isTrans, COLORREF transeColor)
{
	//���ʱ�ȭ ������, �̹��� ������ ���� ��� �մٸ� ������ ���� ���ٰ�
	if (m_imageInfo != nullptr) release();

	//DC ��������(���� �����쿡 ���� ȭ��DC�� �ӽ÷� �޾ƿ�)
	HDC hdc = GetDC(g_hwnd);

	//�̹��� ���� �����ϱ�
	m_imageInfo = new IMAGE_INFO;
	m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	m_imageInfo->hBit = static_cast<HBITMAP>(LoadImage(g_hInst, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE));
	m_imageInfo->hOBit = static_cast<HBITMAP>(SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit));
	m_imageInfo->width = width;
	m_imageInfo->height = height;
	m_imageInfo->maxFrameX = MaxFrameX;
	m_imageInfo->maxFrameY = MaxFrameY;
	m_imageInfo->FrameWidth = static_cast<int>(width / MaxFrameX);
	m_imageInfo->FrameHeight = static_cast<int>(height / MaxFrameY);

	//�̹��� ���� �����ϱ�
	m_imageInfo2 = new IMAGE_INFO;
	//��Ʈ�� dc ȣȯ�Ǵ°� �����
	m_imageInfo2->hMemDC = CreateCompatibleDC(hdc);
	//���� ���� ��Ʈ��
	m_imageInfo2->hBit = static_cast<HBITMAP>(CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY));
	//dc���Լ� ������ �޾ƿ��� ���� ����� �� ���������� �õ� ��Ʈ���̶� �س��� ����
	m_imageInfo2->hOBit = static_cast<HBITMAP>(SelectObject(m_imageInfo2->hMemDC, m_imageInfo2->hBit));
	m_imageInfo2->width = WINSIZEX;
	m_imageInfo2->height = WINSIZEY;

	//���� ���� �ɼ�
	m_blendFunc.BlendFlags = 0;
	m_blendFunc.AlphaFormat = 0; // 32��Ʈ�ΰ�� AC_SRC_ALPHA �� �����ϰ� �׿ܴ� 0���� ����
	m_blendFunc.BlendOp = AC_SRC_OVER;


	//���� �̸�
	int len = _tcslen(fileName);
	m_fileName = new _TCHAR[len + 1];
	_tcscpy_s(m_fileName, len + 1, fileName);

	//����Ű �÷� ����
	m_isTranse = isTrans;
	m_transColor = transeColor;

	//���ҽ��� �����µ� ���� ������
	if (m_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//DC ����
	ReleaseDC(g_hwnd, hdc);

	return S_OK;
}

void BmpImg::setTransColor(bool isTrans, COLORREF transColor)
{
	m_isTranse = isTrans;
	m_transColor = transColor;
}

void BmpImg::release()
{
	//�̹��� ������ ���� �ִٸ� ������ ���� ��!!
	if (m_imageInfo)
	{
		//�õ� ��Ʈ�� ����ֱ�
		SelectObject(m_imageInfo->hMemDC, m_imageInfo->hOBit);
		//���� ��Ʈ�� ����
		DeleteObject(m_imageInfo->hBit);
		//�޾ƿ� dc ����
		DeleteDC(m_imageInfo->hMemDC);
		//�õ� ��Ʈ�� ����ֱ�
		SelectObject(m_imageInfo2->hMemDC, m_imageInfo2->hOBit);
		//���� ��Ʈ�� ����
		DeleteObject(m_imageInfo2->hBit);
		//�޾ƿ� dc ����
		DeleteDC(m_imageInfo2->hMemDC);
		SAFE_DELETE(m_imageInfo);
		SAFE_DELETE(m_imageInfo2);
		SAFE_DELETE(m_fileName);

		m_isTranse = false;
		m_transColor = RGB(0, 0, 0);
	}
}

void BmpImg::render(HDC hdc)
{
	//���� ���� ������
	if (m_isTranse)
	{
		//GdiTransparentBlt : ��Ʈ���� �ҷ��� �� Ư������ �����ϰ� �������ִ� �Լ�
		GdiTransparentBlt(
			hdc,					//����� ����� DC
			0,						//����� ��ǥ�� ������ x
			0,						//����� ��ǥ�� ������ y
			m_imageInfo->width,		//����� �̹��� ���� ũ��
			m_imageInfo->height,	//����� �̹��� ���� ũ��
			m_imageInfo->hMemDC,	//����� ��� DC
			0,						//���� ���� ����x
			0,						//���� ���� ����y
			m_imageInfo->width,		//���� ���� ���� ũ��
			m_imageInfo->height,	//���� ���� ���� ũ��
			m_transColor);			//�����Ҷ� ������ ����(����Ÿ)
	}
	//���� �̹��� �״�� ����� ������
	else
	{
		//DC���� �������� ��Ӻ��� ���ִ� �Լ� SRCCOPY: �ҽ������� ��󿵿��� �����Ѵ�
		BitBlt(hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void BmpImg::render(HDC hdc, int destX, int destY)
{
	//���� ���ٰų�?
	if (m_isTranse)
	{
		//GdiTransparentBlt : ��Ʈ���� �ҷ��� �� Ư������ �����ϰ� �������ִ� �Լ�
		GdiTransparentBlt(
			hdc,					//����� ����� DC
			destX,					//����� ��ǥ�� ������x
			destY,					//����� ��ǥ�� ������y
			m_imageInfo->width,		//����� �̹��� ����ũ��
			m_imageInfo->height,	//����� �̹��� ����ũ��
			m_imageInfo->hMemDC,	//����� ��� DC
			0,						//���� ��������x
			0,						//���� ��������y
			m_imageInfo->width,		//���� ���� ����ũ��
			m_imageInfo->height,	//���� ���� ����ũ��
			m_transColor);			//�����Ҷ� ������ ����(����Ÿ)
	}
	//���� �̹��� �״�� ����Ұǰ�?
	else
	{
		//DC���� �������� ��Ӻ��� ���ִ� �Լ� SRCCOPY: �ҽ������� ��󿵿��� �����Ѵ�
		BitBlt(hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void BmpImg::render(HDC hdc, int destX, int destY, int destWidth, int destHeight, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (sourWidth == 0)sourWidth = m_imageInfo->width - sourX;
	if (sourHeight == 0)sourHeight = m_imageInfo->height - sourY;
	//���� ���ٰų�?
	if (m_isTranse)
	{
		//GdiTransparentBlt : ��Ʈ���� �ҷ��� �� Ư������ �����ϰ� �������ִ� �Լ�
		GdiTransparentBlt(
			hdc,					//����� ����� DC
			destX,					//����� ��ǥ�� ������x
			destY,					//����� ��ǥ�� ������y
			destWidth,				//����� �̹��� ����ũ��
			destHeight,				//����� �̹��� ����ũ��
			m_imageInfo->hMemDC,	//����� ��� DC
			sourX,					//���� ��������x
			sourY,					//���� ��������y
			sourWidth,				//���� ���� ����ũ��
			sourHeight,				//���� ���� ����ũ��
			m_transColor);			//�����Ҷ� ������ ����(����Ÿ)
	}
	//���� �̹��� �״�� ����Ұǰ�?
	else
	{
		//bitblt���� ���̱� ���̱� ��� �߰��� �༮
		StretchBlt(hdc, destX, destY, destWidth, destHeight,
			m_imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight, SRCCOPY);
	}
}

void BmpImg::alphaRender(HDC hdc, BYTE alpha)
{
	//���İ� �ʱ�ȭ
	m_blendFunc.SourceConstantAlpha = alpha;


	//���� ���ٰǰ�?
	if (m_isTranse)
	{
		//1. ��Ӻ��� g_MemDc-> 2�� �޸� dc
		BitBlt(m_imageInfo2->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			hdc, 0, 0, SRCCOPY);
		//2. ����ؾ� �� �̹����� 2�� �޸� dc�� ����� �����ϰ� �׷��ش�
		GdiTransparentBlt(m_imageInfo2->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_transColor);
		//3. 2���� �ִ� dc �� �츮�� ���� ȭ�鿡 �Ű��ش�
		AlphaBlend(hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo2->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_blendFunc);


	}
	//���� �̹��� �״�� ����Ұǰ�?
	else
	{
		//DC���� �������� ��Ӻ��� ���ִ� �Լ� SRCCOPY: �ҽ������� ��󿵿��� �����Ѵ�
		AlphaBlend(hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,m_blendFunc);
	}


}

void BmpImg::alphaRender(HDC hdc, BYTE alpha, int destX, int destY)
{
	//���İ� �ʱ�ȭ
	m_blendFunc.SourceConstantAlpha = alpha;


	//���� ���ٰǰ�?
	if (m_isTranse)
	{
		
		//1. ��Ӻ��� g_MemDc-> 2�� �޸� dc
		BitBlt(m_imageInfo2->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			hdc, destX, destY, SRCCOPY);
		//2. ����ؾ� �� �̹����� 2�� �޸� dc�� ����� �����ϰ� �׷��ش�
		GdiTransparentBlt(m_imageInfo2->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_transColor);
		//3. 2���� �ִ� dc �� �츮�� ���� ȭ�鿡 �Ű��ش�
		AlphaBlend(hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo2->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_blendFunc);


	}
	//���� �̹��� �״�� ����Ұǰ�?
	else
	{
		//DC���� �������� ��Ӻ��� ���ִ� �Լ� SRCCOPY: �ҽ������� ��󿵿��� �����Ѵ�
		AlphaBlend(hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_blendFunc);
	}



}

void BmpImg::alphaRender(HDC hdc, BYTE alpha, int destX, int destY, int destWidth, int destHeight, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (sourWidth == 0)sourWidth = m_imageInfo->width - sourX;
	if (sourHeight == 0)sourHeight = m_imageInfo->height - sourY;
	//���İ� �ʱ�ȭ
	m_blendFunc.SourceConstantAlpha = alpha;


	//���� ���ٰǰ�?
	if (m_isTranse)
	{
		////1. ��Ӻ��� g_MemDc-> 2�� �޸� dc
		BitBlt(m_imageInfo2->hMemDC, 0, 0, m_imageInfo2->width, m_imageInfo2->height,
			hdc, destX, destY, SRCCOPY);
		//2. ����ؾ� �� �̹����� 2�� �޸� dc�� ����� �����ϰ� �׷��ش�
		GdiTransparentBlt(m_imageInfo2->hMemDC, 0, 0, destWidth, destHeight,
			m_imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight, m_transColor);
		//3. 2���� �ִ� dc �� �츮�� ���� ȭ�鿡 �Ű��ش�
		AlphaBlend(hdc, destX, destY, destWidth, destHeight,
			m_imageInfo2->hMemDC, 0, 0, destWidth, destHeight, m_blendFunc);


	}
	//���� �̹��� �״�� ����Ұǰ�?
	else
	{
		//DC���� �������� ��Ӻ��� ���ִ� �Լ� SRCCOPY: �ҽ������� ��󿵿��� �����Ѵ�
		AlphaBlend(hdc, destX, destY, destWidth, destHeight,
			m_imageInfo->hMemDC,sourX, sourY, sourWidth, sourHeight, m_blendFunc);
	}
}

void BmpImg::frameRender(HDC hdc, int frameX, int frameY, int destX, int destY, int destWidth, int destHeight)
{
	//������ ���� ��ġ
	int sourX = m_imageInfo->FrameWidth * frameX;
	int sourY = m_imageInfo->FrameHeight * frameY;


	//���� ���ٰ��ΰ�?
	if (m_isTranse)
	{
		//GdiTransparentBlt : ��Ʈ���� �ҷ��� �� Ư������ �����ϰ� �������ִ� �Լ�
		GdiTransparentBlt(
			hdc,					//����� ����� DC
			destX,					//����� ��ǥ�� ������ x
			destY,					//����� ��ǥ�� ������ y
			destWidth,				//����� �̹��� ���� ũ��
			destHeight,				//����� �̹��� ���� ũ��
			m_imageInfo->hMemDC,	//����� ��� DC
			sourX,					//���� ��������x
			sourY,					//���� ��������y
			m_imageInfo->FrameWidth,				//���� ���� ���� ũ��
			m_imageInfo->FrameHeight,				//���� ���� ���� ũ��
			m_transColor);			//������ �� ������ ����(����Ÿ)
	}
	//���� �̹��� �״�� ����� ������
	else
	{
		//bitblt���� ���̱� ���̱� ��� �߰��� ��
		StretchBlt(hdc, destX, destY, destWidth, destHeight,
			m_imageInfo->hMemDC, sourX, sourY, m_imageInfo->FrameWidth, m_imageInfo->FrameHeight, SRCCOPY);
	}
}

void BmpImg::frameAlphaRender(HDC hdc, BYTE alpha, int frameX, int frameY, int destX, int destY, int destWidth, int destHeight)
{
	//������ ���� ��ġ
	int sourX = m_imageInfo->FrameWidth * frameX;
	int sourY = m_imageInfo->FrameHeight * frameY;
	//���İ� �ʱ�ȭ
	m_blendFunc.SourceConstantAlpha = alpha;


	//���� ���ٰǰ�?
	if (m_isTranse)
	{
		////1. ��Ӻ��� g_MemDc-> 2�� �޸� dc
		BitBlt(m_imageInfo2->hMemDC, 0, 0, m_imageInfo2->width, m_imageInfo2->height,
			hdc, destX, destY, SRCCOPY);
		//2. ����ؾ� �� �̹����� 2�� �޸� dc�� ����������ϰ� �׷��ش�
		GdiTransparentBlt(m_imageInfo2->hMemDC, 0, 0, destWidth, destHeight,
			m_imageInfo->hMemDC, sourX, sourY, m_imageInfo->FrameWidth, m_imageInfo->FrameHeight, m_transColor);
		//3. 2���� �ִ� dc �� �츮�� ���� ȭ�鿡 �Ű��ش�
		AlphaBlend(hdc, destX, destY, destWidth, destHeight,
			m_imageInfo2->hMemDC, 0, 0, destWidth, destHeight, m_blendFunc);


	}
	//���� �̹��� �״�� ����Ұǰ�?
	else
	{
		//DC���� �������� ��Ӻ��� ���ִ� �Լ� SRCCOPY: �ҽ������� ��󿵿��� �����Ѵ�
		AlphaBlend(hdc, destX, destY, destWidth, destHeight,
			m_imageInfo->hMemDC, sourX, sourY, m_imageInfo->FrameWidth, m_imageInfo->FrameHeight, m_blendFunc);
	}
}

void BmpImg::loopRender(HDC hdc, int destX, int destY, int sourX, int sourY)
{
	//��ǥ�� ������ʰ� �� ����ֱ�
	if (sourX < 0) sourX = m_imageInfo->width + (sourX % m_imageInfo->width);
	if (sourY < 0) sourY = m_imageInfo->height + (sourY % m_imageInfo->height);
	if (sourX > m_imageInfo->width) sourX = (sourX % m_imageInfo->width);
	if (sourY > m_imageInfo->height) sourY = (sourY % m_imageInfo->height);

	//�̹��� ũ�⿡ �°� ���� ����
	render(hdc,
		destX,							//���x��ǥ
		destY,							//���y��ǥ
		m_imageInfo->width - sourX, 	//��±���
		m_imageInfo->height - sourY,	//��³���
		sourX, 							//�̹���x��ǥ
		sourY,							//�̹���y��ǥ
		m_imageInfo->width - sourX,		//�̹���xũ��
		m_imageInfo->height - sourY);	//�̹���yũ��
	if (sourX != 0)
	{
		//����ִ� �̵��� ��ŭ ��ǥ�̵� �� �׷� �ֱ�
		render(hdc,
			destX + m_imageInfo->width - sourX,//���x��ǥ
			destY, 							  //���y��ǥ
			sourX, 							  //��±���
			m_imageInfo->height - sourY,	  //��³���
			0,								  //�̹���x��ǥ
			sourY,							  //�̹���y��ǥ
			sourX,							  //�̹���xũ��
			m_imageInfo->height - sourY);	 //�̹���yũ��
	}
	if (sourY != 0)
	{
		//����ִ� �̵��� ��ŭ ��ǥ�̵� �� �׷� �ֱ�
		render(hdc,
			destX,									//���x��ǥ
			destY + m_imageInfo->height - sourY, 	//���y��ǥ
			m_imageInfo->width - sourX, 			//��±���
			sourY,									//��³���
			sourX,									//�̹���x��ǥ
			0,										//�̹���y��ǥ
			m_imageInfo->width - sourX,				//�̹���xũ��
			sourY);									//�̹���yũ��
	}
	if (sourX != 0 && sourY != 0)
	{
		//����ִ� �̵��� ��ŭ ��ǥ�̵� �� �׷� �ֱ�
		render(hdc,
			destX + m_imageInfo->width - sourX,		//���x��ǥ
			destY + m_imageInfo->height - sourY, 	//���y��ǥ
			sourX,									//��±���
			sourY,									//��³���
			0,										//�̹���x��ǥ
			0,										//�̹���y��ǥ
			sourX,									//�̹���xũ��
			sourY);									//�̹���yũ��
	}
}

void BmpImg::loopAlphaRender(HDC hdc, BYTE alpha, int destX, int destY, int sourX, int sourY)
{
	//��ǥ�� ������ʰ� �� ����ֱ�
	if (sourX < 0) sourX = m_imageInfo->width + (sourX % m_imageInfo->width);
	if (sourY < 0) sourY = m_imageInfo->height + (sourY % m_imageInfo->height);
	if (sourX > m_imageInfo->width) sourX = (sourX % m_imageInfo->width);
	if (sourY > m_imageInfo->height) sourY = (sourY % m_imageInfo->height);

	//�̹��� ũ�⿡ �°� ���� ����
	alphaRender(hdc,
		alpha,
		destX,							//���x��ǥ
		destY,							//���y��ǥ
		m_imageInfo->width - sourX, 	//��±���
		m_imageInfo->height - sourY,	//��³���
		sourX, 							//�̹���x��ǥ
		sourY,							//�̹���y��ǥ
		m_imageInfo->width - sourX,		//�̹���xũ��
		m_imageInfo->height - sourY);	//�̹���yũ��
	if (sourX != 0)
	{
		//����ִ� �̵��� ��ŭ ��ǥ�̵� �� �׷� �ֱ�
		alphaRender(hdc,
			alpha,
			destX + m_imageInfo->width - sourX,//���x��ǥ
			destY, 							  //���y��ǥ
			sourX, 							  //��±���
			m_imageInfo->height - sourY,	  //��³���
			0,								  //�̹���x��ǥ
			sourY,							  //�̹���y��ǥ
			sourX,							  //�̹���xũ��
			m_imageInfo->height - sourY);	 //�̹���yũ��
	}
	if (sourY != 0)
	{
		//����ִ� �̵��� ��ŭ ��ǥ�̵� �� �׷� �ֱ�
		alphaRender(hdc,
			alpha,
			destX,									//���x��ǥ
			destY + m_imageInfo->height - sourY, 	//���y��ǥ
			m_imageInfo->width - sourX, 			//��±���
			sourY,									//��³���
			sourX,									//�̹���x��ǥǥ
			0,										//�̹���y��ǥ
			m_imageInfo->width - sourX,				//�̹���xũ��
			sourY);									//�̹���yũ��
	}
	if (sourX != 0 && sourY != 0)
	{
		//����ִ� �̵��� ��ŭ ��ǥ�̵� �� �׷� �ֱ�
		alphaRender(hdc,
			alpha,
			destX + m_imageInfo->width - sourX,		//���x��ǥ
			destY + m_imageInfo->height - sourY, 	//���y��ǥ
			sourX,									//��±���
			sourY,									//��³���
			0,										//�̹���x��ǥ
			0,										//�̹���y��ǥ
			sourX,									//�̹���xũ��
			sourY);									//�̹���yũ��
	}
}

void BmpImg::rotateRender(HDC hdc, float radian, bool reverse, float multiple, int destX, int destY)
{
	POINT imagePoint[3];
	POINT tempPoint[3];
	FPOINT center;
	float imageAngle[3];
	float radius;

	if (reverse == false)
	{
		imagePoint[0] = { 0,0 };
		imagePoint[1] = { m_imageInfo->width,0 };
		imagePoint[2] = { 0,m_imageInfo->height };
	}
	else
	{
		imagePoint[0] = { m_imageInfo->width , 0 };
		imagePoint[1] = { 0,0 };
		imagePoint[2] = { m_imageInfo->width , m_imageInfo->height };
	}

	center = { m_imageInfo->width / 2.0f,m_imageInfo->height / 2.0f };

	radius = GetDistance(0, 0, center.x, center.y)*multiple;
	for (int i = 0; i < 3; i++)
	{
		imageAngle[i] = GetRadian(center.x, center.y, (float)imagePoint[i].x, (float)imagePoint[i].y);
	}
	center.x *= multiple;
	center.y *= multiple;

	//���� ��ǥ�������� �Ÿ��� ����Ʈ���� �Ǹ� �̹����� �߸��Ƿ�
	//radius ���� ��ǥ�� ����Ʈ���� ���ش�
	for (int i = 0; i < 3; i++)
	{

		tempPoint[i].x = static_cast<int>(round(radius + cosf(radian + imageAngle[i])*radius));
		tempPoint[i].y = static_cast<int>(round(radius - sinf(radian + imageAngle[i])*radius));
	}

	//1. 2dc�� ������ �������� ä���ش�
	HBRUSH brush = CreateSolidBrush(m_transColor);
	HBRUSH obrush = static_cast<HBRUSH>(SelectObject(m_imageInfo2->hMemDC, brush));
	PatBlt(m_imageInfo2->hMemDC, 0, 0, WINSIZEX, WINSIZEY, PATCOPY);
	SelectObject(m_imageInfo2->hMemDC, obrush);
	DeleteObject(brush);

	//2.  ����ؾ� �� �̹����� dc2�� �׷��ش�(��￩��)
	PlgBlt(
		m_imageInfo2->hMemDC,
		tempPoint,
		m_imageInfo->hMemDC,
		0,
		0,
		m_imageInfo->width,
		m_imageInfo->height,
		NULL,
		NULL,
		NULL
	);
	//3. 2�� dc���� �׷��� dc�� ���� �����ϰ� �׷��ش�

	//��� ��ġ���� �ٽ� �־����� radius���� ������ ������ǥ�� �̵������ش�
	GdiTransparentBlt(
		hdc,				//����� ����� DC
		static_cast<int>(round(destX - radius + center.x)),				//����� ��ǥ ������ x
		static_cast<int>(round(destY - radius + center.y)),				//����� ��ǥ ������ y
		static_cast<int>(round(2.0f*radius)),			//����� �̹��� ����ũ��
		static_cast<int>(round(2.0f*radius)),			//����� �̹��� ����ũ��
		m_imageInfo2->hMemDC,	//����� ��� DC
		0, 0,		//���� �������� xy
		static_cast<int>(round(2.0f*radius)),			//���� ���� ����ũ��
		static_cast<int>(round(2.0f*radius)),			//���� ���� ����Ű��
		m_transColor);
}

void BmpImg::frameRotateRender(HDC hdc, float radian, bool reverse, float multiple, int frameX, int frameY, int destX, int destY)
{
	//������ ���� ��ġ
	int sourX = m_imageInfo->FrameWidth* frameX;
	int sourY = m_imageInfo->FrameHeight* frameY;



	POINT imagePoint[3];
	POINT tempPoint[3];
	FPOINT center;
	float imageAngle[3];
	float radius;

	if (reverse == false)
	{
		imagePoint[0] = { 0,0 };
		imagePoint[1] = { m_imageInfo->FrameWidth,0 };
		imagePoint[2] = { 0,m_imageInfo->FrameHeight };
	}
	else
	{
		imagePoint[0] = { m_imageInfo->FrameWidth , 0 };
		imagePoint[1] = { 0,0 };
		imagePoint[2] = { m_imageInfo->FrameWidth , m_imageInfo->FrameHeight };
	}

	center = { m_imageInfo->FrameWidth / 2.0f,m_imageInfo->FrameHeight / 2.0f };

	radius = GetDistance(0, 0, center.x, center.y)*multiple;
	for (int i = 0; i < 3; i++)
	{
		imageAngle[i] = GetRadian(center.x, center.y, (float)imagePoint[i].x, (float)imagePoint[i].y);
	}
	center.x *= multiple;
	center.y *= multiple;

	//���� ��ǥ�������� �Ÿ��� ����Ʈ���� �Ǹ� �̹����� �߸��Ƿ�
	//radius ���� ��ǥ�� ����Ʈ���� ���ش�
	for (int i = 0; i < 3; i++)
	{

		tempPoint[i].x = static_cast<int>(round(radius + cosf(radian + imageAngle[i])*radius));
		tempPoint[i].y = static_cast<int>(round(radius - sinf(radian + imageAngle[i])*radius));
	}

	//1. 2dc�� ������ �������� ä���ش�
	HBRUSH brush = CreateSolidBrush(m_transColor);
	HBRUSH obrush = static_cast<HBRUSH>(SelectObject(m_imageInfo2->hMemDC, brush));
	PatBlt(m_imageInfo2->hMemDC, 0, 0, WINSIZEX, WINSIZEY, PATCOPY);
	SelectObject(m_imageInfo2->hMemDC, obrush);
	DeleteObject(brush);
	//2.  ����ؾ� �� �̹����� ���忡 �׷��ش�(��￩��)
	PlgBlt(
		m_imageInfo2->hMemDC,
		tempPoint,
		m_imageInfo->hMemDC,
		sourX,
		sourY,
		m_imageInfo->FrameWidth,
		m_imageInfo->FrameHeight,
		NULL,
		NULL,
		NULL
	);
	//3. 2�� dc���� �׷��� dc�� ���� �����ϰ� �׷��ش�

	//��� ��ġ���� �ٽ� �־����� radius ���� ������ ������ǥ�� �̵������ش�
	GdiTransparentBlt(
		hdc,				//����� ����� DC
		static_cast<int>(round(destX - radius + center.x)),				//����� ��ǥ ������ x
		static_cast<int>(round(destY - radius + center.y)),				//����� ��ǥ ������ y
		static_cast<int>(round(2.0f * radius)),			//����� �̹��� ����ũ��
		static_cast<int>(round(2.0f * radius)),			//����� �̹��� ����ũ��
		m_imageInfo2->hMemDC,	//����� ��� DC
		0, 0,		//���� �������� xy
		static_cast<int>(round(2.0f * radius)),			//���� ���� ����ũ��
		static_cast<int>(round(2.0f * radius)),			//���� ���� ����Ű��
		m_transColor);
}
