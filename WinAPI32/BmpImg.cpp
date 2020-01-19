#include "stdafx.h"
#include "BmpImg.h"
#pragma comment(lib,"msimg32.lib")//알파블렌드를 사용하기 위해서 라이브러리 추가

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
//비트맵을 출력하기 위해서 메모리DC를 만들어 주는 함수
//화면 DC와 메모리 DC 호환

//CreateCompatibleBitmap
//원본 DC와 호환되는 비트맵을 생성해 주는 함수

//SelectObject
//기본의 비트맵 핸들을 반환하고, 새로운 비트맵을 메모리 DC가 선택하고
//메모리 DC에 비트맵이 그려짐, 화면 DC 아니므로 모니터에 출력 x

//올드 비트맵, 비트맵 생성 이유는
//현재 선택되어 있는 DC에 GDI 오브젝트를 생성,삭제 할 수 없기 때문이다(일종의 안전장치)
//새로 만들고 현재 오브젝트와 바꿔주거나 
//예전 오브젝트와 바꿔 준 후 삭제 해야한다

HRESULT BmpImg::init(int width, int height)
{
	//재초기화 방지용, 이미지 정보에 값이 들어 잇다면 릴리즈 먼저 해줄 것
	if (m_imageInfo != nullptr) release();

	//DC 가져오기(현재 윈도우에 대한 화면DC를 임시로 받아옴)
	HDC hdc = GetDC(g_hwnd);

	//이미지 정보 생성하기
	m_imageInfo = new IMAGE_INFO;
	//비트맵 dc 호환되게 만들기
	m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	//새로 만든 비트맵
	m_imageInfo->hBit = static_cast<HBITMAP>(CreateCompatibleBitmap(hdc, width, height));
	//DC에게서 기존걸 받아오고, 새로 만든걸 줌. 기존정보는 올드 비트맵이라 해놓고 저장
	m_imageInfo->hOBit = static_cast<HBITMAP>(SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit));
	m_imageInfo->width = width;
	m_imageInfo->height = height;

	//이미지 정보 생성하기
	m_imageInfo2 = new IMAGE_INFO;
	//비트맵 DC 호환되는거 만들기
	m_imageInfo2->hMemDC = CreateCompatibleDC(hdc);
	//새로 만든 비트맵
	m_imageInfo2->hBit = static_cast<HBITMAP>(CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY));
	//DC에게서 기존걸 받아오고, 새로 만든걸 줌. 기존정보는 올드 비트맵이라 해놓고 저장
	m_imageInfo2->hOBit = static_cast<HBITMAP>(SelectObject(m_imageInfo2->hMemDC, m_imageInfo2->hBit));
	m_imageInfo2->width = WINSIZEX;
	m_imageInfo2->height = WINSIZEY;
	
	//알파 블렌드 옵션
	m_blendFunc.BlendFlags = 0;
	m_blendFunc.AlphaFormat = 0; // 32비트인경우 AC_SRC_ALPHA 를 지정하고 그외는 0으로 지정
	m_blendFunc.BlendOp = AC_SRC_OVER;



	//파일이름
	m_fileName = nullptr;

	//투명키 컬러 세팅
	m_isTranse = false;
	m_transColor = RGB(0, 0, 0);

	//리소스를 얻어오는데 실패 했을 때
	if (m_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//DC 해제
	ReleaseDC(g_hwnd, hdc);

	return S_OK;
}

HRESULT BmpImg::init(const _TCHAR* fileName, int width, int height, bool isTrans, COLORREF transeColor)
{
	//재초기화 방지용, 이미지 정보에 값이 들어 잇다면 릴리즈 먼저 해줄것
	if (m_imageInfo != nullptr) release();

	//DC 가져오기(현재 윈도우에 대한 화면DC를 임시로 받아옴)
	HDC hdc = GetDC(g_hwnd);

	//이미지 정보 생성하기
	m_imageInfo = new IMAGE_INFO;
	m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	m_imageInfo->hBit = static_cast<HBITMAP>(LoadImage(g_hInst, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE));
	m_imageInfo->hOBit = static_cast<HBITMAP>(SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit));
	m_imageInfo->width = width;
	m_imageInfo->height = height;


	//이미지 정보 생성하기
	m_imageInfo2 = new IMAGE_INFO;
	//비트맵 dc 호환되는거 만들기
	m_imageInfo2->hMemDC = CreateCompatibleDC(hdc);
	//새로 만든 비트맵
	m_imageInfo2->hBit = static_cast<HBITMAP>(CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY));
	//dc에게서 기존걸 받아오고 새로 만든걸 줌 기존정보는 올드 비트맵이라 해놓고 저장
	m_imageInfo2->hOBit = static_cast<HBITMAP>(SelectObject(m_imageInfo2->hMemDC, m_imageInfo2->hBit));
	m_imageInfo2->width = WINSIZEX;
	m_imageInfo2->height = WINSIZEY;

	//알파 블렌드 옵션
	m_blendFunc.BlendFlags = 0;
	m_blendFunc.AlphaFormat = 0; // 32비트인경우 AC_SRC_ALPHA 를 지정하고 그외는 0으로 지정
	m_blendFunc.BlendOp = AC_SRC_OVER;


	//파일 이름
	int len = _tcslen(fileName);
	m_fileName = new _TCHAR[len + 1];
	_tcscpy_s(m_fileName, len + 1, fileName);

	//투명키 컬러 셋팅
	m_isTranse = isTrans;
	m_transColor = transeColor;

	//리소스를 얻어오는데 실패 했을 때
	if (m_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//DC 해제
	ReleaseDC(g_hwnd, hdc);

	return S_OK;
}

HRESULT BmpImg::init(const _TCHAR * fileName, int width, int height, int MaxFrameX, int MaxFrameY, bool isTrans, COLORREF transeColor)
{
	//재초기화 방지용, 이미지 정보에 값이 들어 잇다면 릴리즈 먼저 해줄것
	if (m_imageInfo != nullptr) release();

	//DC 가져오기(현재 윈도우에 대한 화면DC를 임시로 받아옴)
	HDC hdc = GetDC(g_hwnd);

	//이미지 정보 생성하기
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

	//이미지 정보 생성하기
	m_imageInfo2 = new IMAGE_INFO;
	//비트맵 dc 호환되는거 만들기
	m_imageInfo2->hMemDC = CreateCompatibleDC(hdc);
	//새로 만든 비트맵
	m_imageInfo2->hBit = static_cast<HBITMAP>(CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY));
	//dc에게서 기존걸 받아오고 새로 만든걸 줌 기존정보는 올드 비트맵이라 해놓고 저장
	m_imageInfo2->hOBit = static_cast<HBITMAP>(SelectObject(m_imageInfo2->hMemDC, m_imageInfo2->hBit));
	m_imageInfo2->width = WINSIZEX;
	m_imageInfo2->height = WINSIZEY;

	//알파 블렌드 옵션
	m_blendFunc.BlendFlags = 0;
	m_blendFunc.AlphaFormat = 0; // 32비트인경우 AC_SRC_ALPHA 를 지정하고 그외는 0으로 지정
	m_blendFunc.BlendOp = AC_SRC_OVER;


	//파일 이름
	int len = _tcslen(fileName);
	m_fileName = new _TCHAR[len + 1];
	_tcscpy_s(m_fileName, len + 1, fileName);

	//투명키 컬러 셋팅
	m_isTranse = isTrans;
	m_transColor = transeColor;

	//리소스를 얻어오는데 실패 했을때
	if (m_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}

	//DC 해제
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
	//이미지 정보가 남아 있다면 릴리즈 해줄 것!!
	if (m_imageInfo)
	{
		//올드 비트맵 쥐어주기
		SelectObject(m_imageInfo->hMemDC, m_imageInfo->hOBit);
		//만든 비트맵 삭제
		DeleteObject(m_imageInfo->hBit);
		//받아온 dc 삭제
		DeleteDC(m_imageInfo->hMemDC);
		//올드 비트맵 쥐어주기
		SelectObject(m_imageInfo2->hMemDC, m_imageInfo2->hOBit);
		//만든 비트맵 삭제
		DeleteObject(m_imageInfo2->hBit);
		//받아온 dc 삭제
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
	//배경색 없앨 것인지
	if (m_isTranse)
	{
		//GdiTransparentBlt : 비트맵을 불러올 때 특정색상 제외하고 복사해주는 함수
		GdiTransparentBlt(
			hdc,					//복사될 장소의 DC
			0,						//복사될 좌표의 시작점 x
			0,						//복사될 좌표의 시작점 y
			m_imageInfo->width,		//복사될 이미지 가로 크기
			m_imageInfo->height,	//복사될 이미지 세로 크기
			m_imageInfo->hMemDC,	//복사될 대상 DC
			0,						//복사 시작 지점x
			0,						//복사 시작 지점y
			m_imageInfo->width,		//복사 영역 가로 크기
			m_imageInfo->height,	//복사 영역 세로 크기
			m_transColor);			//복사할때 제외할 색상(마젠타)
	}
	//원본 이미지 그대로 출력할 것인지
	else
	{
		//DC간의 영역끼리 고속복사 해주는 함수 SRCCOPY: 소스영역을 대상영역에 복사한다
		BitBlt(hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void BmpImg::render(HDC hdc, int destX, int destY)
{
	//배경색 없앨거냐?
	if (m_isTranse)
	{
		//GdiTransparentBlt : 비트맵을 불러올 때 특정색상 제외하고 복사해주는 함수
		GdiTransparentBlt(
			hdc,					//복사될 장소의 DC
			destX,					//복사될 좌표의 시작점x
			destY,					//복사될 좌표의 시작점y
			m_imageInfo->width,		//복사될 이미지 가로크기
			m_imageInfo->height,	//복사될 이미지 세로크기
			m_imageInfo->hMemDC,	//복사될 대상 DC
			0,						//복사 시작지점x
			0,						//복사 시작지점y
			m_imageInfo->width,		//복사 영역 가로크기
			m_imageInfo->height,	//복사 영역 세로크기
			m_transColor);			//복사할때 제외할 색상(마젠타)
	}
	//원본 이미지 그대로 출력할건가?
	else
	{
		//DC간의 영역끼리 고속복사 해주는 함수 SRCCOPY: 소스영역을 대상영역에 복사한다
		BitBlt(hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void BmpImg::render(HDC hdc, int destX, int destY, int destWidth, int destHeight, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (sourWidth == 0)sourWidth = m_imageInfo->width - sourX;
	if (sourHeight == 0)sourHeight = m_imageInfo->height - sourY;
	//배경색 없앨거냐?
	if (m_isTranse)
	{
		//GdiTransparentBlt : 비트맵을 불러올 때 특정색상 제외하고 복사해주는 함수
		GdiTransparentBlt(
			hdc,					//복사될 장소의 DC
			destX,					//복사될 좌표의 시작점x
			destY,					//복사될 좌표의 시작점y
			destWidth,				//복사될 이미지 가로크기
			destHeight,				//복사될 이미지 세로크기
			m_imageInfo->hMemDC,	//복사될 대상 DC
			sourX,					//복사 시작지점x
			sourY,					//복사 시작지점y
			sourWidth,				//복사 영역 가로크기
			sourHeight,				//복사 영역 세로크기
			m_transColor);			//복사할때 제외할 색상(마젠타)
	}
	//원본 이미지 그대로 출력할건가?
	else
	{
		//bitblt에서 늘이기 줄이기 기능 추가된 녀석
		StretchBlt(hdc, destX, destY, destWidth, destHeight,
			m_imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight, SRCCOPY);
	}
}

void BmpImg::alphaRender(HDC hdc, BYTE alpha)
{
	//알파값 초기화
	m_blendFunc.SourceConstantAlpha = alpha;


	//배경색 없앨건가?
	if (m_isTranse)
	{
		//1. 고속복사 g_MemDc-> 2번 메모리 dc
		BitBlt(m_imageInfo2->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			hdc, 0, 0, SRCCOPY);
		//2. 출력해야 될 이미지를 2번 메모리 dc에 투명색 제외하고 그려준다
		GdiTransparentBlt(m_imageInfo2->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_transColor);
		//3. 2번에 있던 dc 를 우리가 보는 화면에 옮겨준다
		AlphaBlend(hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo2->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_blendFunc);


	}
	//원본 이미지 그대로 출력할건가?
	else
	{
		//DC간의 영역끼리 고속복사 해주는 함수 SRCCOPY: 소스영역을 대상영역에 복사한다
		AlphaBlend(hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,m_blendFunc);
	}


}

void BmpImg::alphaRender(HDC hdc, BYTE alpha, int destX, int destY)
{
	//알파값 초기화
	m_blendFunc.SourceConstantAlpha = alpha;


	//배경색 없앨건가?
	if (m_isTranse)
	{
		
		//1. 고속복사 g_MemDc-> 2번 메모리 dc
		BitBlt(m_imageInfo2->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			hdc, destX, destY, SRCCOPY);
		//2. 출력해야 될 이미지를 2번 메모리 dc에 투명색 제외하고 그려준다
		GdiTransparentBlt(m_imageInfo2->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_transColor);
		//3. 2번에 있던 dc 를 우리가 보는 화면에 옮겨준다
		AlphaBlend(hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo2->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_blendFunc);


	}
	//원본 이미지 그대로 출력할건가?
	else
	{
		//DC간의 영역끼리 고속복사 해주는 함수 SRCCOPY: 소스영역을 대상영역에 복사한다
		AlphaBlend(hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
			m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_blendFunc);
	}



}

void BmpImg::alphaRender(HDC hdc, BYTE alpha, int destX, int destY, int destWidth, int destHeight, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (sourWidth == 0)sourWidth = m_imageInfo->width - sourX;
	if (sourHeight == 0)sourHeight = m_imageInfo->height - sourY;
	//알파값 초기화
	m_blendFunc.SourceConstantAlpha = alpha;


	//배경색 없앨건가?
	if (m_isTranse)
	{
		////1. 고속복사 g_MemDc-> 2번 메모리 dc
		BitBlt(m_imageInfo2->hMemDC, 0, 0, m_imageInfo2->width, m_imageInfo2->height,
			hdc, destX, destY, SRCCOPY);
		//2. 출력해야 될 이미지를 2번 메모리 dc에 투명색 제외하고 그려준다
		GdiTransparentBlt(m_imageInfo2->hMemDC, 0, 0, destWidth, destHeight,
			m_imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight, m_transColor);
		//3. 2번에 있던 dc 를 우리가 보는 화면에 옮겨준다
		AlphaBlend(hdc, destX, destY, destWidth, destHeight,
			m_imageInfo2->hMemDC, 0, 0, destWidth, destHeight, m_blendFunc);


	}
	//원본 이미지 그대로 출력할건가?
	else
	{
		//DC간의 영역끼리 고속복사 해주는 함수 SRCCOPY: 소스영역을 대상영역에 복사한다
		AlphaBlend(hdc, destX, destY, destWidth, destHeight,
			m_imageInfo->hMemDC,sourX, sourY, sourWidth, sourHeight, m_blendFunc);
	}
}

void BmpImg::frameRender(HDC hdc, int frameX, int frameY, int destX, int destY, int destWidth, int destHeight)
{
	//프레임 시작 위치
	int sourX = m_imageInfo->FrameWidth * frameX;
	int sourY = m_imageInfo->FrameHeight * frameY;


	//배경색 없앨것인가?
	if (m_isTranse)
	{
		//GdiTransparentBlt : 비트맵을 불러올 때 특정색상 제외하고 복사해주는 함수
		GdiTransparentBlt(
			hdc,					//복사될 장소의 DC
			destX,					//복사될 좌표의 시작점 x
			destY,					//복사될 좌표의 시작점 y
			destWidth,				//복사될 이미지 가로 크기
			destHeight,				//복사될 이미지 세로 크기
			m_imageInfo->hMemDC,	//복사될 대상 DC
			sourX,					//복사 시작지점x
			sourY,					//복사 시작지점y
			m_imageInfo->FrameWidth,				//복사 영역 가로 크기
			m_imageInfo->FrameHeight,				//복사 영역 세로 크기
			m_transColor);			//복사할 때 제외할 색상(마젠타)
	}
	//원본 이미지 그대로 출력할 것인지
	else
	{
		//bitblt에서 늘이기 줄이기 기능 추가된 놈
		StretchBlt(hdc, destX, destY, destWidth, destHeight,
			m_imageInfo->hMemDC, sourX, sourY, m_imageInfo->FrameWidth, m_imageInfo->FrameHeight, SRCCOPY);
	}
}

void BmpImg::frameAlphaRender(HDC hdc, BYTE alpha, int frameX, int frameY, int destX, int destY, int destWidth, int destHeight)
{
	//프레임 시작 위치
	int sourX = m_imageInfo->FrameWidth * frameX;
	int sourY = m_imageInfo->FrameHeight * frameY;
	//알파값 초기화
	m_blendFunc.SourceConstantAlpha = alpha;


	//배경색 없앨건가?
	if (m_isTranse)
	{
		////1. 고속복사 g_MemDc-> 2번 메모리 dc
		BitBlt(m_imageInfo2->hMemDC, 0, 0, m_imageInfo2->width, m_imageInfo2->height,
			hdc, destX, destY, SRCCOPY);
		//2. 출력해야 될 이미지를 2번 메모리 dc에 투명색제외하고 그려준다
		GdiTransparentBlt(m_imageInfo2->hMemDC, 0, 0, destWidth, destHeight,
			m_imageInfo->hMemDC, sourX, sourY, m_imageInfo->FrameWidth, m_imageInfo->FrameHeight, m_transColor);
		//3. 2번에 있던 dc 를 우리가 보는 화면에 옮겨준다
		AlphaBlend(hdc, destX, destY, destWidth, destHeight,
			m_imageInfo2->hMemDC, 0, 0, destWidth, destHeight, m_blendFunc);


	}
	//원본 이미지 그대로 출력할건가?
	else
	{
		//DC간의 영역끼리 고속복사 해주는 함수 SRCCOPY: 소스영역을 대상영역에 복사한다
		AlphaBlend(hdc, destX, destY, destWidth, destHeight,
			m_imageInfo->hMemDC, sourX, sourY, m_imageInfo->FrameWidth, m_imageInfo->FrameHeight, m_blendFunc);
	}
}

void BmpImg::loopRender(HDC hdc, int destX, int destY, int sourX, int sourY)
{
	//좌표값 벗어나지않게 값 잡아주기
	if (sourX < 0) sourX = m_imageInfo->width + (sourX % m_imageInfo->width);
	if (sourY < 0) sourY = m_imageInfo->height + (sourY % m_imageInfo->height);
	if (sourX > m_imageInfo->width) sourX = (sourX % m_imageInfo->width);
	if (sourY > m_imageInfo->height) sourY = (sourY % m_imageInfo->height);

	//이미지 크기에 맞게 먼저 렌더
	render(hdc,
		destX,							//출력x좌표
		destY,							//출력y좌표
		m_imageInfo->width - sourX, 	//출력길이
		m_imageInfo->height - sourY,	//출력높이
		sourX, 							//이미지x좌표
		sourY,							//이미지y좌표
		m_imageInfo->width - sourX,		//이미지x크기
		m_imageInfo->height - sourY);	//이미지y크기
	if (sourX != 0)
	{
		//비어있는 이동값 만큼 좌표이동 후 그려 주기
		render(hdc,
			destX + m_imageInfo->width - sourX,//출력x좌표
			destY, 							  //출력y좌표
			sourX, 							  //출력길이
			m_imageInfo->height - sourY,	  //출력높이
			0,								  //이미지x좌표
			sourY,							  //이미지y좌표
			sourX,							  //이미지x크기
			m_imageInfo->height - sourY);	 //이미지y크기
	}
	if (sourY != 0)
	{
		//비어있는 이동값 만큼 좌표이동 후 그려 주기
		render(hdc,
			destX,									//출력x좌표
			destY + m_imageInfo->height - sourY, 	//출력y좌표
			m_imageInfo->width - sourX, 			//출력길이
			sourY,									//출력높이
			sourX,									//이미지x좌표
			0,										//이미지y좌표
			m_imageInfo->width - sourX,				//이미지x크기
			sourY);									//이미지y크기
	}
	if (sourX != 0 && sourY != 0)
	{
		//비어있는 이동값 만큼 좌표이동 후 그려 주기
		render(hdc,
			destX + m_imageInfo->width - sourX,		//출력x좌표
			destY + m_imageInfo->height - sourY, 	//출력y좌표
			sourX,									//출력길이
			sourY,									//출력높이
			0,										//이미지x좌표
			0,										//이미지y좌표
			sourX,									//이미지x크기
			sourY);									//이미지y크기
	}
}

void BmpImg::loopAlphaRender(HDC hdc, BYTE alpha, int destX, int destY, int sourX, int sourY)
{
	//좌표값 벗어나지않게 값 잡아주기
	if (sourX < 0) sourX = m_imageInfo->width + (sourX % m_imageInfo->width);
	if (sourY < 0) sourY = m_imageInfo->height + (sourY % m_imageInfo->height);
	if (sourX > m_imageInfo->width) sourX = (sourX % m_imageInfo->width);
	if (sourY > m_imageInfo->height) sourY = (sourY % m_imageInfo->height);

	//이미지 크기에 맞게 먼저 렌더
	alphaRender(hdc,
		alpha,
		destX,							//출력x좌표
		destY,							//출력y좌표
		m_imageInfo->width - sourX, 	//출력길이
		m_imageInfo->height - sourY,	//출력높이
		sourX, 							//이미지x좌표
		sourY,							//이미지y좌표
		m_imageInfo->width - sourX,		//이미지x크기
		m_imageInfo->height - sourY);	//이미지y크기
	if (sourX != 0)
	{
		//비어있는 이동값 만큼 좌표이동 후 그려 주기
		alphaRender(hdc,
			alpha,
			destX + m_imageInfo->width - sourX,//출력x좌표
			destY, 							  //출력y좌표
			sourX, 							  //출력길이
			m_imageInfo->height - sourY,	  //출력높이
			0,								  //이미지x좌표
			sourY,							  //이미지y좌표
			sourX,							  //이미지x크기
			m_imageInfo->height - sourY);	 //이미지y크기
	}
	if (sourY != 0)
	{
		//비어있는 이동값 만큼 좌표이동 후 그려 주기
		alphaRender(hdc,
			alpha,
			destX,									//출력x좌표
			destY + m_imageInfo->height - sourY, 	//출력y좌표
			m_imageInfo->width - sourX, 			//출력길이
			sourY,									//출력높이
			sourX,									//이미지x좌표표
			0,										//이미지y좌표
			m_imageInfo->width - sourX,				//이미지x크기
			sourY);									//이미지y크기
	}
	if (sourX != 0 && sourY != 0)
	{
		//비어있는 이동값 만큼 좌표이동 후 그려 주기
		alphaRender(hdc,
			alpha,
			destX + m_imageInfo->width - sourX,		//출력x좌표
			destY + m_imageInfo->height - sourY, 	//출력y좌표
			sourX,									//출력길이
			sourY,									//출력높이
			0,										//이미지x좌표
			0,										//이미지y좌표
			sourX,									//이미지x크기
			sourY);									//이미지y크기
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

	//중점 좌표기준으로 거리를 떨어트리게 되면 이미지가 잘리므로
	//radius 기준 좌표로 떨어트리게 해준다
	for (int i = 0; i < 3; i++)
	{

		tempPoint[i].x = static_cast<int>(round(radius + cosf(radian + imageAngle[i])*radius));
		tempPoint[i].y = static_cast<int>(round(radius - sinf(radian + imageAngle[i])*radius));
	}

	//1. 2dc에 제외할 색상으로 채워준다
	HBRUSH brush = CreateSolidBrush(m_transColor);
	HBRUSH obrush = static_cast<HBRUSH>(SelectObject(m_imageInfo2->hMemDC, brush));
	PatBlt(m_imageInfo2->hMemDC, 0, 0, WINSIZEX, WINSIZEY, PATCOPY);
	SelectObject(m_imageInfo2->hMemDC, obrush);
	DeleteObject(brush);

	//2.  출력해야 될 이미지를 dc2에 그려준다(기울여서)
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
	//3. 2번 dc에서 그려줄 dc로 배경색 제외하고 그려준다

	//출력 위치값은 다시 멀어졌던 radius값을 빼준후 센터좌표로 이동시켜준다
	GdiTransparentBlt(
		hdc,				//복사될 장소의 DC
		static_cast<int>(round(destX - radius + center.x)),				//복사될 좌표 시작점 x
		static_cast<int>(round(destY - radius + center.y)),				//복사될 좌표 시작점 y
		static_cast<int>(round(2.0f*radius)),			//복사될 이미지 가로크기
		static_cast<int>(round(2.0f*radius)),			//복사될 이미지 세로크기
		m_imageInfo2->hMemDC,	//복사될 대상 DC
		0, 0,		//복사 시작지점 xy
		static_cast<int>(round(2.0f*radius)),			//복사 영역 가로크기
		static_cast<int>(round(2.0f*radius)),			//복사 영역 세로키기
		m_transColor);
}

void BmpImg::frameRotateRender(HDC hdc, float radian, bool reverse, float multiple, int frameX, int frameY, int destX, int destY)
{
	//프레임 시작 위치
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

	//중점 좌표기준으로 거리를 떨어트리게 되면 이미지가 잘리므로
	//radius 기준 좌표로 떨어트리게 해준다
	for (int i = 0; i < 3; i++)
	{

		tempPoint[i].x = static_cast<int>(round(radius + cosf(radian + imageAngle[i])*radius));
		tempPoint[i].y = static_cast<int>(round(radius - sinf(radian + imageAngle[i])*radius));
	}

	//1. 2dc에 제외할 색상으로 채워준다
	HBRUSH brush = CreateSolidBrush(m_transColor);
	HBRUSH obrush = static_cast<HBRUSH>(SelectObject(m_imageInfo2->hMemDC, brush));
	PatBlt(m_imageInfo2->hMemDC, 0, 0, WINSIZEX, WINSIZEY, PATCOPY);
	SelectObject(m_imageInfo2->hMemDC, obrush);
	DeleteObject(brush);
	//2.  출력해야 될 이미지를 블렌드에 그려준다(기울여서)
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
	//3. 2번 dc에서 그려줄 dc로 배경색 제외하고 그려준다

	//출력 위치값은 다시 멀어졌던 radius 값을 빼준후 센터좌표로 이동시켜준다
	GdiTransparentBlt(
		hdc,				//복사될 장소의 DC
		static_cast<int>(round(destX - radius + center.x)),				//복사될 좌표 시작점 x
		static_cast<int>(round(destY - radius + center.y)),				//복사될 좌표 시작점 y
		static_cast<int>(round(2.0f * radius)),			//복사될 이미지 가로크기
		static_cast<int>(round(2.0f * radius)),			//복사될 이미지 세로크기
		m_imageInfo2->hMemDC,	//복사될 대상 DC
		0, 0,		//복사 시작지점 xy
		static_cast<int>(round(2.0f * radius)),			//복사 영역 가로크기
		static_cast<int>(round(2.0f * radius)),			//복사 영역 세로키기
		m_transColor);
}
