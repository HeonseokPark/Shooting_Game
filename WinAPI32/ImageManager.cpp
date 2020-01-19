#include "stdafx.h"
#include "ImageManager.h"


ImageManager::ImageManager()
{
	m_BackDC = new BmpImg;
	m_BackDC->init(WINSIZEX, WINSIZEY);
	m_FrontDC = GetDC(g_hwnd);
	//�̹��� �ø��� ���� ����
	SetStretchBltMode(m_BackDC->getMemDC(), HALFTONE);

}


ImageManager::~ImageManager()
{
	m_BackDC->release();
	SAFE_DELETE(m_BackDC);
	//DC ����
	ReleaseDC(g_hwnd, m_FrontDC);
}

BmpImg * ImageManager::addImage(string strKey, const _TCHAR * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	//�߰��Ϸ��� Ű������ �̹����� �����ϴ��� Ȯ��
	BmpImg* img = findImage(strKey);

	//�߰��Ϸ��� �̹����� �̹� �����Ѵٸ� ���� ������ �ʰ� ����
	if (img) return img;

	//���� ���� �ʴ´ٸ�
	img = new BmpImg;
	//�̹��� �ʱ�ȭ ����� �ȵǸ� nullptr����(E_FAILE ���� S_OK 0�� ��ȯ)
	if (FAILED(img->init(fileName, width, height, isTrans, transColor)))
	{
		img->release();
		SAFE_DELETE(img);
		return nullptr;
	}
	//������ �̹����� ������ ���� ��Ͽ� �߰��Ѵ�.
	m_ImgMap.insert(make_pair(strKey, img));

	return img;
}

BmpImg * ImageManager::addImage(string strKey, const _TCHAR * fileName, int width, int height, int MaxFrameX, int MaxFrameY, bool isTrans, COLORREF transColor)
{
	//�߰��Ϸ��� Ű������ �̹����� �����ϴ��� Ȯ��
	BmpImg* img = findImage(strKey);

	//�߰��Ϸ��� �̹����� �̹� �����Ѵٸ� ���� ������ �ʰ� ����
	if (img) return img;

	//���� ���� �ʴ´ٸ�
	img = new BmpImg;
	//�̹��� �ʱ�ȭ ����� �ȵǸ� nullptr����(E_FAILE ���� S_OK 0�� ��ȯ)
	if (FAILED(img->init(fileName, width, height, MaxFrameX, MaxFrameY,isTrans, transColor)))
	{
		img->release();
		SAFE_DELETE(img);
		return nullptr;
	}
	//������ �̹����� ������ ���� ��Ͽ� �߰��Ѵ�.
	m_ImgMap.insert(make_pair(strKey, img));

	return img;
}

BmpImg * ImageManager::findImage(string strKey)
{
	//�ش� Ű������ Ž���ϴµ� Ű���� ������ �� �������� end�� ��ȯ�ȴ�.
	ImgIter iter = m_ImgMap.find(strKey);
	
	//�˻��� Ű�� ã�Ҵٸ�
	if (iter != m_ImgMap.end())
	{
		return iter->second;
	}
	//��ã���� nullptr ��ȯ
	return nullptr;
}

bool ImageManager::deleteImage(string strKey)
{
	//�ش�Ű�� �˻�
	ImgIter iter = m_ImgMap.find(strKey);

	//�˻��� Ű�� ã�Ҵٸ�
	if (iter != m_ImgMap.end())
	{
		//�̹��� ����� ������ �� �����Ҵ� ����
		iter->second->release();
		SAFE_DELETE(iter->second);
		//�ʿ��� �߰� ����
		m_ImgMap.erase(iter);
		return true;
	}

	return false;
}

bool ImageManager::deleteAll()
{
	ImgIter iter = m_ImgMap.begin();
	for (iter; iter != m_ImgMap.end();)
	{
		if (iter->second != nullptr)
		{
			iter->second->release();
			SAFE_DELETE(iter->second);
			iter = m_ImgMap.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	//�� ��ü ����
	m_ImgMap.clear();

	return true;
}

void ImageManager::render(string strKey, HDC hdc)
{
	//Ű������ ã�� �̹����� ����
	BmpImg* img = findImage(strKey);
	if (img) img->render(hdc);
}

void ImageManager::render(string strKey, HDC hdc, float destX, float DestY)
{
	//Ű������ ã�� �̹����� ����
	BmpImg* img = findImage(strKey);
	if (img) img->render(hdc,static_cast<int>(round(destX)), static_cast<int>(round(DestY)));
}

void ImageManager::render(string strKey, HDC hdc, float destX, float destY, int destWidth, int destHeight, int sourX, int sourY, int sourWidth, int sourHeight)
{
	//Ű������ ã�� �̹����� ����
	BmpImg* img = findImage(strKey);
	if (img) img->render(hdc, static_cast<int>(round(destX)), static_cast<int>(round(destY)), destWidth, destHeight, sourX, sourY, sourWidth, sourHeight);
}

void ImageManager::alphaRender(string strKey, HDC hdc, BYTE alpha)
{
	//Ű������ ã�� �̹����� ����
	BmpImg* img = findImage(strKey);
	if (img) img->alphaRender(hdc, alpha);
}

void ImageManager::alphaRender(string strKey, HDC hdc, BYTE alpha, float destX, float destY)
{
	//Ű������ ã�� �̹����� ����
	BmpImg* img = findImage(strKey);
	if (img) img->alphaRender(hdc, alpha, static_cast<int>(round(destX)), static_cast<int>(round(destY)));
}

void ImageManager::alphaRender(string strKey, HDC hdc, BYTE alpha, float destX, float destY, int destWidth, int destHeight, int sourX, int sourY, int sourWidth, int sourHeight)
{
	//Ű������ ã�� �̹����� ����
	BmpImg* img = findImage(strKey);
	if (img) img->alphaRender(hdc, alpha, static_cast<int>(round(destX)), static_cast<int>(round(destY)), destWidth, destHeight, sourX, sourY, sourWidth, sourHeight);
}

void ImageManager::frameRender(string strKey, HDC hdc, int frameX, int frameY, float destX, float destY, int destWidth, int destHeight)
{
	//Ű������ ã�� �̹����� ����
	BmpImg* img = findImage(strKey);
	if (img) img->frameRender(hdc, frameX, frameY, static_cast<int>(round(destX)), static_cast<int>(round(destY)), destWidth, destHeight);
}

void ImageManager::frameAlphaRender(string strKey, HDC hdc, BYTE alpha, int frameX, int frameY, float destX, float destY, int destWidth, int destHeight)
{
	//Ű������ ã�� �̹����� ����
	BmpImg* img = findImage(strKey);
	if (img) img->frameAlphaRender(hdc, alpha, frameX, frameY, static_cast<int>(round(destX)), static_cast<int>(round(destY)), destWidth, destHeight);
}

void ImageManager::loopRender(string strKey, HDC hdc, float destX, float destY, int sourX, int sourY)
{
	//�̹����� ã�� �� ����
	BmpImg* img = findImage(strKey);
	if (img) img->loopRender(hdc, static_cast<int>(round(destX)), static_cast<int>(round(destY)), sourX, sourY);

}

void ImageManager::loopAlphaRender(string strKey, HDC hdc, BYTE alpha, float destX, float destY, int sourX, int sourY)
{
	//�̹����� ã�� �� ����
	BmpImg* img = findImage(strKey);
	if (img) img->loopAlphaRender(hdc, alpha, static_cast<int>(round(destX)), static_cast<int>(round(destY)), sourX, sourY);
}

void ImageManager::rotateRender(string strKey, HDC hdc, float radian, bool reverse, float multiple, float destX, float destY)
{
	//�̹����� ã�� �� ����
	BmpImg* img = findImage(strKey);
	if (img) img->rotateRender(hdc, radian, reverse, multiple, static_cast<int>(round(destX)), static_cast<int>(round(destY)));
}

void ImageManager::frameRotateRender(string strKey, HDC hdc, float radian, bool reverse, float multiple, int frameX, int frameY, float destX, float destY)
{
	//�̹����� ã�� �� ����
	BmpImg* img = findImage(strKey);
	if (img) img->frameRotateRender(hdc, radian, reverse, multiple, frameX, frameY, static_cast<int>(round(destX)), static_cast<int>(round(destY)));
}
