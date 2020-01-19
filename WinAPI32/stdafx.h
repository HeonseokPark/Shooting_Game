#pragma once

//디버그,문자집합 매크로
#if defined UNICODE && defined _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#elif defined _MBCS && defined _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif 


#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
// #include <stdlib.h>
// #include <stdio.h>
#include <tchar.h>

// C++ 런타임 헤더파일
#include <iostream>

//자주 사용하는 STL
#include <string>
#include <vector>
//#include <list>
#include <map>
#include <queue>
using namespace std;


//문자집합 매크로
#ifdef _UNICODE
#define _tstring wstring
#define _tcout wcout
#else
#define _tstring string
#define _tcout cout
#endif


//=====================================
//	헤더 추가
//=====================================
#include "MyFunc.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "BmpImg.h"
#include "ImageManager.h"
#include "SoundManager.h"
#include "ObjectPool.h"

//=====================================
//	싱글톤 추가
//=====================================
#define KEYMANAGER KeyManager::GetInstance()
#define TIMEMANAGER TimeManager::GetInstance()
#define g_ETime TimeManager::GetInstance()->GetElapsedTime()
#define g_WTime TimeManager::GetInstance()->GetWorldTime()
#define IMGMANAGER ImageManager::GetInstance()
#define g_MemDC ImageManager::GetInstance()->getMemDC()
#define SOUNDMANAGER SoundManager::GetInstance()
#define OBJECTPOOL(Type) ObjectPool<Type>::GetInstance()




//=====================================
//	윈도우 초기화 세팅
//=====================================
#define WINNAME _T("TanMak Shooting")
#define WINSTARX 0
#define WINSTARY 100
#define WINSIZEX 1280
#define WINSIZEY 720
#define FIXFRAME 300.0f


//===================================================
//	## 매크로 함수 ## (메인게임의 릴리즈 부분에서 사용)
//===================================================
#define SAFE_DELETE(p) {if(p){delete(p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[](p); (p) = nullptr;}

//===================================================
//	## 전역변수 ## 
//===================================================
extern HINSTANCE	g_hInst;
extern HWND			g_hwnd;
extern FPOINT		g_ptMouse;
extern FPOINT		g_ptCam;