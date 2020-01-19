#pragma once
class GameNode
{
public:
	//S_OK
	//���������� ȣ�� �� ���
	
	//E_FAIL
	//�Լ��� ȣ���ϴ� �߿� ������ ���� ���
	//���� �����Լ�
	virtual HRESULT init() = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	LRESULT MainProc(HWND hWnd, UINT message,
		WPARAM wParam, LPARAM lParam);


	GameNode();
	virtual ~GameNode();
};

