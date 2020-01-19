#include "stdafx.h"
#include "KeyManager.h"


KeyManager::KeyManager()
	:PrevKey(NULL)
{
	ZeroMemory(KeyDown, sizeof(KeyDown));
}


KeyManager::~KeyManager()
{
}

bool KeyManager::isKeyDown(int KeyCode)
{
	//0x0000  0
	//0x0001  1
	//0x8001  10000001
	//0x8000  10000000
	//비트연산자로 값을 비교했을때 0이 아닌 조건은 들어와진다
	if (GetAsyncKeyState(KeyCode) & 0x8000)
	{
		if (!KeyDown[KeyCode])
		{
			KeyDown[KeyCode] = true;
			return true;
		}

	}
	else KeyDown[KeyCode] = false;

	return false;
}

bool KeyManager::StayKeyDown(int KeyCode)
{
	if (GetAsyncKeyState(KeyCode) & 0x8000)
	{
		PrevKey = KeyCode;
		return true;
	}


	return false;
}

bool KeyManager::isKeyUp(int KeyCode)
{
	if (!StayKeyDown(KeyCode))
	{
		if (PrevKey == KeyCode)
		{
			PrevKey = NULL;
			return true;
		}

	}
	return false;
}

bool KeyManager::ToggleKeyDown(int KeyCode)
{
	if (GetKeyState(KeyCode) & 0x0001)
	{
		return true;
	}
	return false;
}
