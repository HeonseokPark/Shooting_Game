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

	//��ư�� ���ȴ��� Ȯ��
	bool isKeyDown(int KeyCode);
	//��ư�� ��� �������� �ִ��� Ȯ��
	bool StayKeyDown(int KeyCode);
	//��ư�� ���������� Ȯ��
	bool isKeyUp(int KeyCode);
	//��ư�� ó�� �ѹ� ���ȴ��� Ȯ��
	bool ToggleKeyDown(int KeyCode);

};

