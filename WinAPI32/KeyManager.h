#pragma once
#include "SingletonBase.h"

class KeyManager :public SingletonBase<KeyManager>
{
private:
	//0~255
	bool KeyDown[256];
	BYTE PrevKey;

public:
	KeyManager();
	~KeyManager();

	//버튼이 눌렸는지 확인
	bool isKeyDown(int KeyCode);
	//버튼이 계속 눌려지고 있는지 확인
	bool StayKeyDown(int KeyCode);
	//버튼이 떼어졌는지 확인
	bool isKeyUp(int KeyCode);
	//버튼이 처음 한번 눌렸는지 확인
	bool ToggleKeyDown(int KeyCode);

};

