// WinAPI32.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "MainGame.h"


HINSTANCE	g_hInst;
HWND		g_hwnd;
FPOINT		g_ptMouse = { 0.0f , 0.0f };
FPOINT		g_ptCam = { 0.0f , 0.0f };

MainGame*	Mg;

//윈도우 프로시져 : 메세지를 운영체제에 전달 강제로 운영체제가 호출해.
//hWnd : 어느 윈도우에서 발생한 메세지 인지 구분
//message : 메세지 구분 번호
//wParam : 마우스 버튼의 상태 키보드 조합키의 상태 전달
//lParam : 마우스 클릭 좌표 전달
//callback 어떤 이벤트가 발생했을때 나에게 알려달라고 하는것


/*

데이터형	의미
BYTE	unsigned char형
WORD	unsigned short형
DWORD	unsigned long형
LONG	long과 동일하다.
LPSTR	char *와 동일하다.
BOOL	정수형이며 TRUE, FALSE 중 한 값을 가진다.

*/

enum DIRECTION
{
	DIRECTION_LT,
	DIRECTION_RT,
	DIRECTION_LB,
	DIRECTION_RB
};


RECT rc;
DIRECTION direction;
bool isTouch = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Mg->MainProc(hWnd, message, wParam, lParam);
}
void SetWindowSize(int x, int y, int width, int height)
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;

	//실제 윈도우 크기 조정
	AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU, false);
	//위 RECT 정보로 윈도우 사이즈 셋팅
	SetWindowPos(g_hwnd, NULL, x, y, (rc.right - rc.left), 
		(rc.bottom - rc.top), SWP_NOZORDER | SWP_NOMOVE);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	WNDCLASS	wc;	//window infomation struct
	ZeroMemory(&wc, sizeof(WNDCLASS));


	//비트연산자   111
	//윈도우 클래스의 스타일을 지정
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//메세지를 처리하는 함수를 지정(프로시져)
	wc.lpfnWndProc = static_cast<WNDPROC>(WndProc);
	//윈도우 클래스 에서 사용하고자 하는 여분의 메모리 양을 바이트 단위로 지정
	wc.cbClsExtra = 0;
	//cbClsExtra 유사하되 개별 윈도우에서 사용하고자 하는 여분의 메모리양을 지정
	wc.cbWndExtra = 0;
	//윈도우 클래스를 등록한 응용 프로그램의 인스턴스 핸들
	wc.hInstance = hInstance;
	//타이틀바의 좌상단에 보여줄 아이콘 윈도우가 최소화 되었을때 보여줄 아이콘을 지정
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//클래스 커서를 지정
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//윈도우 작업영역에 칠할 배경 브러시
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	//등록하고자 하는 윈도우의 이름
	wc.lpszClassName = WINNAME;
	//클래스로부터 만들어진 윈도우가 사용할 메뉴 지장
	wc.lpszMenuName = nullptr;

	//운영체제의 레지스터에 이 프로그램을 등록한다.
	if (RegisterClass(&wc) == false)
	{
		return FALSE;
	}

	//핸들 인스턴스 저장
	g_hInst = hInstance;
	g_hwnd = CreateWindow(
		WINNAME,				//윈도우 클래스의 이름
		WINNAME,				//윈도우 타이틀바 이름
		WS_OVERLAPPEDWINDOW,	//윈도우 스타일
		WINSTARX,					    //창의 시작 x좌표
		WINSTARY,						//창의 시작 y좌표
		WINSIZEX,					//창의 가로크기
		WINSIZEY,					//창의 세로크기
		nullptr,				//부모 윈도우
		nullptr,				//메뉴 핸들
		hInstance,				//인스턴스 지정
		hInstance
	);

	if (!g_hwnd)
	{
		return FALSE;
	}

	Mg = new MainGame;
	if (FAILED(Mg->init()))
	{
		return FALSE;
	}

	//화면 작업 사이즈 영역 계산
	SetWindowSize(WINSTARX, WINSTARY, WINSIZEX, WINSIZEY);

	//생성한 프로그램을 디스플레이의 최상위로 올린다.
	ShowWindow(g_hwnd, nCmdShow);

	//메인 윈도우 갱신
	UpdateWindow(g_hwnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		else
		{
			TIMEMANAGER->SetFrame(FIXFRAME);
			Mg->update();
			Mg->render();
		}
	}

	Mg->release();
	SAFE_DELETE(Mg);


	DestroyWindow(g_hwnd);
	UnregisterClass(WINNAME, hInstance);

	return static_cast<int>(msg.wParam);
}