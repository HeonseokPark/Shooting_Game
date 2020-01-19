#include "stdafx.h"
#include "PlayerInfo.h"


PlayerInfo::PlayerInfo()
{
	ZeroMemory(&_Player, sizeof(_Player));
}


PlayerInfo::~PlayerInfo()
{
}

HRESULT PlayerInfo::init()
{
	_Player.pos.x = 420.0f;
	_Player.pos.y = 520.0f;

	_Player.isCollision = false;
	_Player.speed = 120.0f;



	return S_OK;
}

void PlayerInfo::release()
{
}

void PlayerInfo::update()
{
	Map = RectMake(880.0f, 0.0f, 400.0f, WINSIZEY);
	_Player.cc = CircleMake(_Player.pos.x + 30.0f, _Player.pos.y + 35.0f, 5.0f);

	if (KEYMANAGER->StayKeyDown(VK_LEFT))
	{
		_Player.pos.x -= _Player.speed * g_ETime;
	}
	if (KEYMANAGER->StayKeyDown(VK_RIGHT))
	{
		_Player.pos.x += _Player.speed * g_ETime;
	}
	if (KEYMANAGER->StayKeyDown(VK_UP))
	{
		_Player.pos.y -= _Player.speed * g_ETime;
	}
	if (KEYMANAGER->StayKeyDown(VK_DOWN))
	{
		_Player.pos.y += _Player.speed * g_ETime;
	}


	//맵과 캐릭터가 화면 밖으로 나가지 않도록 보정

	if ((_Player.cc.x - 40.0f) <= 0.0f)
	{
		_Player.pos.x += 5.0f;
		_Player.cc.x += 5.0f;
	}
	if ((_Player.cc.x + 40.0f) >= Map.left)
	{
		_Player.pos.x -= 5.0f;
		_Player.cc.x -= 5.0f;
	}

	if ((_Player.cc.y - 35.0f) <= 0.0f)
	{
		_Player.pos.y += 5.0f;
		_Player.cc.y += 5.0f;
	}
	if ((_Player.cc.y + 35.0f) >= WINSIZEY)
	{
		_Player.pos.y -= 5.0f;
		_Player.cc.y -= 5.0f;
	}

}

void PlayerInfo::render()
{
	

	/*IMGMANAGER->render("Player", g_MemDC, _Player.pos.x, _Player.pos.y);*/
	//DrawfCircle(g_MemDC, _Player.cc);
}
