#pragma once
#include "SingletonBase.h"

class TimeManager : public SingletonBase <TimeManager>
{
private:
	//FPS:초당 프레임 수(frames per second)
	bool	_isHardware;			//고성능 타이머를 지원하는가?
	__int64 _curTime;				//현재시간
	__int64 _lastTime;				//마지막시간

	float _FPStimeElapsed;			//FPS 마지막시간과 현재 시간의 경과량
	float _timeScale;				//시간 단위 
	DWORD _FPSFrameCount;			//FPS 카운트

	//리턴값
	DWORD _frameRate;				//FPS
	float _timeElapsed;				//한 프레임당 경과량
	float _worldTime;				//전체 시간 경과량

public:
	TimeManager();
	~TimeManager() {};

	//초기화 하지 않으면 프레임은 60 프레임으로 고정
	void SetFrame(float lock = 60.0f);

	//한 프레임 경과시간 가져오기 1/60
	float GetElapsedTime() const { return _timeElapsed; }
	//전체 시간 경과량 가져오기
	float GetWorldTime() const { return _worldTime; }
	//현재 FPS 가져오기
	unsigned long GetFrameRate() { return _frameRate; }

	void render();

};

