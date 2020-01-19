#include "stdafx.h"
#include "MainGame.h"


MainGame::MainGame()
	:Bullet_Manager(nullptr), m_Scene(nullptr)
{
}


MainGame::~MainGame()
{
	//�̱��� ����
	KEYMANAGER->ReleaseSingleton();
	TIMEMANAGER->ReleaseSingleton();
	//�̹��� �Ŵ��� ���� �����ǿ� ��� �̹��� ��ü ����
	IMGMANAGER->deleteAll();
	IMGMANAGER->ReleaseSingleton();
}

HRESULT MainGame::init()
{	
	//������(�ü��)�� ���۵Ǿ �帥 �ð��� 1/1000�� ������
	//DWORD���� �����ϴ� �Լ���. dword->unsigned long
	srand(GetTickCount());
	//////////////////////////////////////////

	//�̹��� �߰�
	//IMGMANAGER->addImage("Player", _T("images/Player.bmp"), 80, 80, true);
	IMGMANAGER->addImage("Player", _T("images/Player2.bmp"), 60, 60, true);
	IMGMANAGER->addImage("E_Bullet1", _T("images/E_Bullet1.bmp"), 15, 15, true);
	IMGMANAGER->addImage("E_Bullet2", _T("images/E_Bullet2.bmp"), 15, 15, true);
	IMGMANAGER->addImage("P_Bullet", _T("images/PlayerBullet.bmp"), 15, 15, true);
	IMGMANAGER->addImage("MainMenu", _T("images/MainMenu.bmp"), WINSIZEX, WINSIZEY);
	//IMGMANAGER->addImage("Start", _T("images/startbutton.bmp"), 126, 42, true);
	IMGMANAGER->addImage("Start", _T("images/start.bmp"), 190, 64, true);
	IMGMANAGER->addImage("Exit", _T("images/exit.bmp"), 187, 64, true);
	IMGMANAGER->addImage("BackGround", _T("images/cloud.bmp"), WINSIZEX, WINSIZEY);
	IMGMANAGER->addImage("Boss2", _T("images/Boss_JS.bmp"), 80, 80, true);
	IMGMANAGER->addImage("Boss", _T("images/boss.bmp"), 1440, 100, 18, 1, true);
	IMGMANAGER->addImage("Number", _T("images/number.bmp"), 300, 30, true);
	



////���� �߰�
//SOUNDMANAGER->addSound("Shoot", "sound/tear_fire.wav", false);
//SOUNDMANAGER->addSound("ShootDelete", "sound/tear_destroy.wav", false);
//SOUNDMANAGER->addSound("BGM", "sound/background1.wav", true);
//SOUNDMANAGER->addSound("NewMap", "sound/next_level.wav", false);
//SOUNDMANAGER->addSound("Door", "sound/door_open.wav", false);

	m_Player = new PlayerInfo;
	m_Player->init();

	Bullet_Manager = new Bullet;
	Bullet_Manager->init();
	Bullet_Manager->MaxBullet = 100;
	m_Scene = new Scene;
	m_Scene->init();

	//////////////////////////////////////////
	return S_OK;
}

void MainGame::release()
{
	//////////////////////////////////////////
	m_Player->release();
	SAFE_DELETE(m_Player);

	Bullet_Manager->release();
	SAFE_DELETE(Bullet_Manager);

	m_Scene->release();
	SAFE_DELETE(m_Scene);


	//////////////////////////////////////////
}

void MainGame::update()
{
	//////////////////////////////////////////

	Loop -= 1;

	if (m_Scene->m_isGameStart == false)
	{
		m_Scene->update();
	}


	if (m_Scene->m_isGameStart == true)
	{
		m_Player->update();
		Bullet_Manager->update();
	}

	if (m_Scene->m_isGameExit == true)
	{
		PostQuitMessage(0);
	}

	cout << "x : " << g_ptMouse.x << " / y : " << g_ptMouse.y << endl;
	//////////////////////////////////////////
}

void MainGame::render()
{
	//������ ����ϴ� �Լ�(dc�� ���� �Ǿ��ִ� �귯�ø� �������� ���. WHITENESS �� ���� �׳� �Ͼ�� ���)
	PatBlt(g_MemDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//////////////////////////////////////////

	if (m_Scene->m_isGameStart == true)
	{

		
		Bullet_Manager->render();
		m_Player->render();
		

	}
	if (m_Scene->m_isGameStart == false)
	{
		m_Scene->render();
	}



	//////////////////////////////////////////
	TIMEMANAGER->render();
	//������۸�
	BitBlt(IMGMANAGER->getHDC(), 0, 0, WINSIZEX, WINSIZEY,
		g_MemDC, 0, 0, SRCCOPY);

}