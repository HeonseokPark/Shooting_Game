#include "stdafx.h"
#include "TimeManager.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
//timeGetTime 함수를 사용하기 위한 라이브러리 추가
//윈도우가 시작되고 난 뒤의 시간을 리턴

TimeManager::TimeManager()
{
	// 참고 http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=735

	//클럭은 전력이 꺼지고 켜지고 반복되는 주파수를 클럭 주파수라 하는데 줄여서 클럭 이라 함

	/*컴퓨터의 메인보드에는 하나의 고해상도 타이머가 존재한다. 이 타이머는 정확하고 일정한 주파수
	(즉, 1초에 각 타이머의 성능에 맞는 진동수)를 갖기 때문에 측정하고자 하는 코드들의 시작과 끝에서
		CPU 클럭수를 얻어 그 차로 수행시간을 얻을 수 있다.*/

	//QueryPerformanceFrequency : 고해상도 타이머의 주파수(1초당 진동수)를 매개변수가 얻는 함수
	//지원 하지 않으면 0을 반환함
	//QueryPerformanceCounter : 고해상도 타이머의 현재 CPU의 클럭 수를 매개변수가 얻는 함수
	//지원 하지 않으면 0을 반환함


	//고성능 타이머 지원여부를 체크한다.
	//고성능 타이머를 지원하면 초당 마이크로세턴드까지 값이 들어간다.
	//(1/1000000)
	//시간 측정
	__int64 _periodFrequency;
	//									포인터형변환,집합체
	if (QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_periodFrequency)))
	{
		//64비트 정수를 사용할 수 있도록 정의한 이유는
		//마이크로세컨드를 (1/1000000)초 까지 측정하여 좀더 정확한 시간값을 저장하기 위함
		_isHardware = true;
		//초기화 될때 마지막시간을(클럭단위) 먼저 초기화 해줌
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_lastTime));

		//초의 단위가 어떻게 되는지 초당 진동수를 나누어 초기화
		_timeScale = 1.0f / _periodFrequency;
	}

	else
	{
		_isHardware = false;

		//timeGetTime:윈도우(운영체제)가 시작되어서 지금까지 흐른 시간을 
		//1 / 1000 초(milliseconds) 단위로 DWORD형을 리턴하는 함수다.

		//초기화 될때 마지막시간을(밀리세컨드단위) 먼저 초기화 해줌
		_lastTime = timeGetTime();
		//밀리세컨드 크기로 계산이라는걸 초기화
		_timeScale = 0.001f;
	}

	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPStimeElapsed = 0.0f;
	_worldTime = 0;
}


void TimeManager::SetFrame(float lock)
{
	//고성능 타이머를 지원한다면
	if (_isHardware)
	{
		//현재시간을(클럭단위) 먼저 초기화 해줌
		QueryPerformanceCounter((reinterpret_cast<LARGE_INTEGER*>(&_curTime)));
	}
	else
	{
		//지원하지 않으면 time 함수를 이용. 밀리세컨드(1/1000)
		_curTime = timeGetTime();
	}

	//마지막 시간과 현재 시간의 한 프레임당 경과량
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	//이곳의 반복문 때문에 경과시간이 맞춰짐.
	//이곳에서 반복문을 돌다가 조건을 맞추면 업데이트와 렌더로 가게됨
	if (lock > 0.0f)
	{
		//고정 프레임의 시간을 만족할때까지 루프
		while (_timeElapsed < (1.0f / lock))
		{
			if (_isHardware) QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_curTime));
			else _curTime = timeGetTime();

			//마지막 시간과 현재 시간의 경과량
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	_lastTime = _curTime;				//마지막 시간을 기록
	_FPSFrameCount++;					//자동으로 프레임 카운트 증가
	_FPStimeElapsed += _timeElapsed;	//초당 프레임 시간 경과량 증가
	_worldTime += _timeElapsed;			//전체 시간 경과량 증가

	//프레임 초기화를 1초마다 진행
	if (_FPStimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPStimeElapsed = 0.0f;
	}

}

void TimeManager::render()
{
	//글꼴 뒤에 하얀배경 지우기
	SetBkMode(g_MemDC, TRANSPARENT);

	_TCHAR str[256];
	#ifdef _DEBUG
		//FPS
		wsprintf(str, _T("FPS: %d"), TIMEMANAGER->GetFrameRate());
		TextOut(g_MemDC, 0, 0, str, _tcslen(str));
	
		//전체 경과 시간
		_stprintf_s(str, _T("WorldTime: %f"), TIMEMANAGER->GetWorldTime());
		TextOut(g_MemDC, 0, 20, str, _tcslen(str));
	
		//한 프레임당 경과 시간
		_stprintf_s(str, _T("SecondPerFrameTime: %f"), TIMEMANAGER->GetElapsedTime());
		TextOut(g_MemDC, 0, 40, str, _tcslen(str));
	#else
		//FPS
		wsprintf(str, _T("FPS: %d"), TIMEMANAGER->GetFrameRate());
		TextOut(g_MemDC, 0, 0, str, _tcslen(str));
	#endif // !_DEBUG
}
