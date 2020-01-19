#include "stdafx.h"
#include "ImageManager.h"


ImageManager::ImageManager()
{
	m_BackDC = new BmpImg;
	m_BackDC->init(WINSIZEX, WINSIZEY);
	m_FrontDC = GetDC(g_hwnd);
	//이미지 늘릴때 깨짐 방지
	SetStretchBltMode(m_BackDC->getMemDC(), HALFTONE);

}


ImageManager::~ImageManager()
{
	m_BackDC->release();
	SAFE_DELETE(m_BackDC);
	//DC 해제
	ReleaseDC(g_hwnd, m_FrontDC);
}

BmpImg * ImageManager::addImage(string strKey, const _TCHAR * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	//추가하려는 키값으로 이미지가 존재하는지 확인
	BmpImg* img = findImage(strKey);

	//추가하려는 이미지가 이미 존재한다면 새로 만들지 않고 리턴
	if (img) return img;

	//존재 하지 않는다면
	img = new BmpImg;
	//이미지 초기화 제대로 안되면 nullptr리턴(E_FAILE 음수 S_OK 0이 반환)
	if (FAILED(img->init(fileName, width, height, isTrans, transColor)))
	{
		img->release();
		SAFE_DELETE(img);
		return nullptr;
	}
	//생성된 이미지를 맵으로 만든 목록에 추가한다.
	m_ImgMap.insert(make_pair(strKey, img));

	return img;
}

BmpImg * ImageManager::addImage(string strKey, const _TCHAR * fileName, int width, int height, int MaxFrameX, int MaxFrameY, bool isTrans, COLORREF transColor)
{
	//추가하려는 키값으로 이미지가 존재하는지 확인
	BmpImg* img = findImage(strKey);

	//추가하려는 이미지가 이미 존재한다면 새로 만들지 않고 리턴
	if (img) return img;

	//존재 하지 않는다면
	img = new BmpImg;
	//이미지 초기화 제대로 안되면 nullptr리턴(E_FAILE 음수 S_OK 0이 반환)
	if (FAILED(img->init(fileName, width, height, MaxFrameX, MaxFrameY,isTrans, transColor)))
	{
		img->release();
		SAFE_DELETE(img);
		return nullptr;
	}
	//생성된 이미지를 맵으로 만든 목록에 추가한다.
	m_ImgMap.insert(make_pair(strKey, img));

	return img;
}

BmpImg * ImageManager::findImage(string strKey)
{
	//해당 키값으로 탐색하는데 키값이 없으면 맨 마지막인 end가 반환된다.
	ImgIter iter = m_ImgMap.find(strKey);
	
	//검색한 키를 찾았다면
	if (iter != m_ImgMap.end())
	{
		return iter->second;
	}
	//못찾으면 nullptr 반환
	return nullptr;
}

bool ImageManager::deleteImage(string strKey)
{
	//해당키를 검색
	ImgIter iter = m_ImgMap.find(strKey);

	//검색한 키를 찾았다면
	if (iter != m_ImgMap.end())
	{
		//이미지 만든것 릴리즈 후 동적할당 해제
		iter->second->release();
		SAFE_DELETE(iter->second);
		//맵에서 중간 삭제
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
	//맵 전체 삭제
	m_ImgMap.clear();

	return true;
}

void ImageManager::render(string strKey, HDC hdc)
{
	//키값으로 찾은 이미지를 렌더
	BmpImg* img = findImage(strKey);
	if (img) img->render(hdc);
}

void ImageManager::render(string strKey, HDC hdc, float destX, float DestY)
{
	//키값으로 찾은 이미지를 렌더
	BmpImg* img = findImage(strKey);
	if (img) img->render(hdc,static_cast<int>(round(destX)), static_cast<int>(round(DestY)));
}

void ImageManager::render(string strKey, HDC hdc, float destX, float destY, int destWidth, int destHeight, int sourX, int sourY, int sourWidth, int sourHeight)
{
	//키값으로 찾은 이미지를 렌더
	BmpImg* img = findImage(strKey);
	if (img) img->render(hdc, static_cast<int>(round(destX)), static_cast<int>(round(destY)), destWidth, destHeight, sourX, sourY, sourWidth, sourHeight);
}

void ImageManager::alphaRender(string strKey, HDC hdc, BYTE alpha)
{
	//키값으로 찾은 이미지를 렌더
	BmpImg* img = findImage(strKey);
	if (img) img->alphaRender(hdc, alpha);
}

void ImageManager::alphaRender(string strKey, HDC hdc, BYTE alpha, float destX, float destY)
{
	//키값으로 찾은 이미지를 렌더
	BmpImg* img = findImage(strKey);
	if (img) img->alphaRender(hdc, alpha, static_cast<int>(round(destX)), static_cast<int>(round(destY)));
}

void ImageManager::alphaRender(string strKey, HDC hdc, BYTE alpha, float destX, float destY, int destWidth, int destHeight, int sourX, int sourY, int sourWidth, int sourHeight)
{
	//키값으로 찾은 이미지를 렌더
	BmpImg* img = findImage(strKey);
	if (img) img->alphaRender(hdc, alpha, static_cast<int>(round(destX)), static_cast<int>(round(destY)), destWidth, destHeight, sourX, sourY, sourWidth, sourHeight);
}

void ImageManager::frameRender(string strKey, HDC hdc, int frameX, int frameY, float destX, float destY, int destWidth, int destHeight)
{
	//키값으로 찾은 이미지를 렌더
	BmpImg* img = findImage(strKey);
	if (img) img->frameRender(hdc, frameX, frameY, static_cast<int>(round(destX)), static_cast<int>(round(destY)), destWidth, destHeight);
}

void ImageManager::frameAlphaRender(string strKey, HDC hdc, BYTE alpha, int frameX, int frameY, float destX, float destY, int destWidth, int destHeight)
{
	//키값으로 찾은 이미지를 렌더
	BmpImg* img = findImage(strKey);
	if (img) img->frameAlphaRender(hdc, alpha, frameX, frameY, static_cast<int>(round(destX)), static_cast<int>(round(destY)), destWidth, destHeight);
}

void ImageManager::loopRender(string strKey, HDC hdc, float destX, float destY, int sourX, int sourY)
{
	//이미지를 찾은 후 렌더
	BmpImg* img = findImage(strKey);
	if (img) img->loopRender(hdc, static_cast<int>(round(destX)), static_cast<int>(round(destY)), sourX, sourY);

}

void ImageManager::loopAlphaRender(string strKey, HDC hdc, BYTE alpha, float destX, float destY, int sourX, int sourY)
{
	//이미지를 찾은 후 렌더
	BmpImg* img = findImage(strKey);
	if (img) img->loopAlphaRender(hdc, alpha, static_cast<int>(round(destX)), static_cast<int>(round(destY)), sourX, sourY);
}

void ImageManager::rotateRender(string strKey, HDC hdc, float radian, bool reverse, float multiple, float destX, float destY)
{
	//이미지를 찾은 후 렌더
	BmpImg* img = findImage(strKey);
	if (img) img->rotateRender(hdc, radian, reverse, multiple, static_cast<int>(round(destX)), static_cast<int>(round(destY)));
}

void ImageManager::frameRotateRender(string strKey, HDC hdc, float radian, bool reverse, float multiple, int frameX, int frameY, float destX, float destY)
{
	//이미지를 찾은 후 렌더
	BmpImg* img = findImage(strKey);
	if (img) img->frameRotateRender(hdc, radian, reverse, multiple, frameX, frameY, static_cast<int>(round(destX)), static_cast<int>(round(destY)));
}
