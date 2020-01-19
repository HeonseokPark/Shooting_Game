#include "stdafx.h"
#include "Bullet.h"



Bullet::Bullet()
{
	_isPlayerAlive = true;
}

Bullet::~Bullet()
{
}

void Bullet::Circle_Bullet(float AngleRate, float Speed, float SpeedRate)
{
	for (g_viBullet = g_vBullet.begin(); g_viBullet != g_vBullet.end();)
	{
		
		//(*g_viBullet)->pos.x = CC_BOX.x;
		//(*g_viBullet)->pos.y = CC_BOX.y;
	
		//각도가 2파이 값을 넘으면 빼준다. 그리하여 유효 범위를 유지할 수 있게 해준다.
		if ((*g_viBullet)->Angle >= 360)
		{
			(*g_viBullet)->Angle -= 360;
		}
		(*g_viBullet)->RotateTime += g_ETime;
	

		//Speed = (*g_viBullet)->Speed;
		//Speed *= g_ETime;
		(*g_viBullet)->ColorType = 1;
		(*g_viBullet)->cc.x += Speed * cosf(((*g_viBullet)->Angle) * TORADIAN);
		(*g_viBullet)->cc.y -= Speed * sinf(((*g_viBullet)->Angle) * TORADIAN);
		(*g_viBullet)->Angle += ((*g_viBullet)->AngleRate  * g_ETime);
		if (IntersectCircle((*g_viBullet)->cc, _Player.cc))
		{
			OBJECTPOOL(ENEMY_BULLET)->PushObject((*g_viBullet));
			g_viBullet = g_vBullet.erase(g_viBullet);
			continue;
			//cout << "충돌" << endl;
		}

		if ((*g_viBullet)->cc.x <= 0.0f || (*g_viBullet)->cc.x >= Map.left || (*g_viBullet)->cc.y <= 0.0f || (*g_viBullet)->cc.y >= WINSIZEY)
		{
			OBJECTPOOL(ENEMY_BULLET)->PushObject((*g_viBullet));
			g_viBullet = g_vBullet.erase(g_viBullet);
			continue;
		}

		g_viBullet++;
	

	}

	if (m_ShotDelay >= 0.5f)
	{
		if (MaxBullet * 10 > g_vBullet.size())
		{
			for (int i = 0; i < 30; i++)
			{
				g_vBullet.emplace_back(OBJECTPOOL(ENEMY_BULLET)->GetObObject());
				g_vBullet[g_vBullet.size() - 1]->pos.x = BOSS_CC.x;
				g_vBullet[g_vBullet.size() - 1]->pos.y = BOSS_CC.y;
				g_vBullet[g_vBullet.size() - 1]->AngleRate = AngleRate;
				g_vBullet[g_vBullet.size() - 1]->SpeedRate = SpeedRate;
				g_vBullet[g_vBullet.size() - 1]->RotateTime = 0.0f;
				g_vBullet[g_vBullet.size() - 1]->Angle = -(360 / 30) * i;
	
				//g_vBullet[g_vBullet.size() - 1]->Angle = 90.0f;

				g_vBullet[g_vBullet.size() - 1]->cc = CircleMake(g_vBullet[g_vBullet.size() - 1]->pos.x + cosf(g_vBullet[g_vBullet.size() - 1]->Angle* TORADIAN) ,
					g_vBullet[g_vBullet.size() - 1]->pos.y - sinf(g_vBullet[g_vBullet.size() - 1]->Angle * TORADIAN) , 5.0f);
				//g_vBullet[g_vBullet.size() - 1]->Speed = 200.0f * g_ETime;
			}
		}
		m_ShotDelay = 0.0f;
	}
	
	


}

void Bullet::PlayerBullet()
{
	for (g_viPBullet = g_vPBullet.begin(); g_viPBullet != g_vPBullet.end();)
	{
		(*g_viPBullet)->RotateTime += g_ETime;
		(*g_viPBullet)->cc.x +=	3.0f * cosf(((*g_viPBullet)->Angle) * TORADIAN);
		(*g_viPBullet)->cc.y -= 3.0f * sinf(((*g_viPBullet)->Angle) * TORADIAN);
		(*g_viPBullet)->Angle += ((*g_viPBullet)->AngleRate  * g_ETime);
		if (IntersectCircle(BOSS_CC, (*g_viPBullet)->cc))
		{
			OBJECTPOOL(PLAYER_BULLET)->PushObject((*g_viPBullet));
			g_viPBullet = g_vPBullet.erase(g_viPBullet);
			continue;
			//cout << "충돌" << endl;
		}

		if ((*g_viPBullet)->cc.x <= 0.0f || (*g_viPBullet)->cc.x >= Map.left || (*g_viPBullet)->cc.y <= 0.0f || (*g_viPBullet)->cc.y >= WINSIZEY)
		{
			OBJECTPOOL(PLAYER_BULLET)->PushObject((*g_viPBullet));
			g_viPBullet = g_vPBullet.erase(g_viPBullet);
			continue;
		}

		g_viPBullet++;
	}

	if (m_PlayerShotDelay >= 0.15f)
	{
		
		g_vPBullet.emplace_back(OBJECTPOOL(PLAYER_BULLET)->GetObObject());
		g_vPBullet[g_vPBullet.size() - 1]->pos.x = _Player.cc.x;
		g_vPBullet[g_vPBullet.size() - 1]->pos.y = _Player.cc.y;
		g_vPBullet[g_vPBullet.size() - 1]->AngleRate = 0.0f;
		g_vPBullet[g_vPBullet.size() - 1]->SpeedRate = 0.0f;
		g_vPBullet[g_vPBullet.size() - 1]->RotateTime = 0.0f;
		g_vPBullet[g_vPBullet.size() - 1]->Angle = 90.0f;


		g_vPBullet[g_vPBullet.size() - 1]->cc = CircleMake(g_vPBullet[g_vPBullet.size() - 1]->pos.x + cosf(g_vPBullet[g_vPBullet.size() - 1]->Angle* TORADIAN),
			g_vPBullet[g_vPBullet.size() - 1]->pos.y - sinf(g_vPBullet[g_vPBullet.size() - 1]->Angle * TORADIAN), 4.0f);
	
		
		m_PlayerShotDelay = 0.0f;
	}
	
}

void Bullet::Reverse_Circle_Bullet(float AngleRate, float Speed, float SpeedRate)
{

	for (g_vi2Bullet = g_v2Bullet.begin(); g_vi2Bullet != g_v2Bullet.end();)
	{
		
		//각도가 2파이 값을 넘으면 빼준다. 그리하여 유효 범위를 유지할 수 있게 해준다.
		if ((*g_vi2Bullet)->Angle >= 360)
		{
			(*g_vi2Bullet)->Angle -= 360;
		}
		(*g_vi2Bullet)->RotateTime += g_ETime;


		//Speed = (*g_viBullet)->Speed;
		//Speed *= g_ETime;
		(*g_vi2Bullet)->ColorType = 2;
		(*g_vi2Bullet)->cc.x += Speed * cosf(((*g_vi2Bullet)->Angle) * TORADIAN);
		(*g_vi2Bullet)->cc.y -= Speed * sinf(((*g_vi2Bullet)->Angle) * TORADIAN);
		(*g_vi2Bullet)->Angle -= ((*g_vi2Bullet)->AngleRate  * g_ETime);

		if (IntersectCircle((*g_vi2Bullet)->cc, _Player.cc))
		{
			OBJECTPOOL(ENEMY_BULLET)->PushObject((*g_vi2Bullet));
			g_vi2Bullet = g_v2Bullet.erase(g_vi2Bullet);
			continue;
			//cout << "충돌" << endl;
		}

		if ((*g_vi2Bullet)->cc.x <= 0.0f || (*g_vi2Bullet)->cc.x >= Map.left || (*g_vi2Bullet)->cc.y <= 0.0f || (*g_vi2Bullet)->cc.y >= WINSIZEY)
		{
			OBJECTPOOL(ENEMY_BULLET)->PushObject((*g_vi2Bullet));
			g_vi2Bullet = g_v2Bullet.erase(g_vi2Bullet);
			continue;
		}

		g_vi2Bullet++;


	}

	if (m_RandomDelay >= 0.5f)
	{
		if (MaxBullet * 10 > g_v2Bullet.size())
		{
			for (int i = 0; i < 30; i++)
			{
				g_v2Bullet.emplace_back(OBJECTPOOL(ENEMY_BULLET)->GetObObject());
				g_v2Bullet[g_v2Bullet.size() - 1]->pos.x = BOSS_CC.x;
				g_v2Bullet[g_v2Bullet.size() - 1]->pos.y = BOSS_CC.y;
				g_v2Bullet[g_v2Bullet.size() - 1]->AngleRate = AngleRate;
				g_v2Bullet[g_v2Bullet.size() - 1]->SpeedRate = SpeedRate * g_ETime;
				g_v2Bullet[g_v2Bullet.size() - 1]->RotateTime = 0.0f;
				//g_v2Bullet[g_v2Bullet.size() - 1]->Angle = 270.0f * AngleRate + g_ETime;
				g_v2Bullet[g_v2Bullet.size() - 1]->Angle = -(360 / 30) * i;

				//g_vBullet[g_vBullet.size() - 1]->Angle - (g_vBullet[g_vBullet.size() - 1]->AngleRate *= g_ETime);


				g_v2Bullet[g_v2Bullet.size() - 1]->cc = CircleMake(g_v2Bullet[g_v2Bullet.size() - 1]->pos.x + cosf(g_v2Bullet[g_v2Bullet.size() - 1]->Angle),
					g_v2Bullet[g_v2Bullet.size() - 1]->pos.y - sinf(g_v2Bullet[g_v2Bullet.size() - 1]->Angle), 5.0f);
				//g_vBullet[g_vBullet.size() - 1]->Speed = 200.0f * g_ETime;

				//g_vBullet[g_vBullet.size() - 1]->Angle -= floor(g_vBullet[g_vBullet.size() - 1]->Angle);
			}
	
		}
		m_RandomDelay = 0.0f;
	}


}

void Bullet::N_Way_Bullet(int N, float AngleRate, float Speed)
{
	for (g_viBullet = g_vBullet.begin(); g_viBullet != g_vBullet.end();)
	{
		//각도가 2파이 값을 넘으면 빼준다. 그리하여 유효 범위를 유지할 수 있게 해준다.
		if ((*g_viBullet)->Angle >= 360)
		{
			(*g_viBullet)->Angle -= 360;
		}
		(*g_viBullet)->RotateTime += g_ETime;

		(*g_viBullet)->ColorType = 1;
		(*g_viBullet)->cc.x += Speed * cosf(((*g_viBullet)->Angle) * TORADIAN);
		(*g_viBullet)->cc.y -= Speed * sinf(((*g_viBullet)->Angle) * TORADIAN);
		(*g_viBullet)->Angle += ((*g_viBullet)->AngleRate  * g_ETime);
		if (IntersectCircle((*g_viBullet)->cc, _Player.cc))
		{
			OBJECTPOOL(ENEMY_BULLET)->PushObject((*g_viBullet));
			g_viBullet = g_vBullet.erase(g_viBullet);
			continue;
			//cout << "충돌" << endl;
		}

		if ((*g_viBullet)->cc.x <= 0.0f || (*g_viBullet)->cc.x >= Map.left || (*g_viBullet)->cc.y <= 0.0f || (*g_viBullet)->cc.y >= WINSIZEY)
		{
			OBJECTPOOL(ENEMY_BULLET)->PushObject((*g_viBullet));
			g_viBullet = g_vBullet.erase(g_viBullet);
			continue;
		}

		g_viBullet++;
	}

	if (m_ShotDelay >= 0.5f)
	{
		if (MaxBullet * 10 > g_vBullet.size())
		{
			for (int i = 0; i < N; i++)
			{
				g_vBullet.emplace_back(OBJECTPOOL(ENEMY_BULLET)->GetObObject());
				g_vBullet[g_vBullet.size() - 1]->pos.x = BOSS_CC.x;
				g_vBullet[g_vBullet.size() - 1]->pos.y = BOSS_CC.y;
				g_vBullet[g_vBullet.size() - 1]->AngleRate = AngleRate;
				g_vBullet[g_vBullet.size() - 1]->SpeedRate = 0.0f;
				g_vBullet[g_vBullet.size() - 1]->RotateTime = 0.0f;
				g_vBullet[g_vBullet.size() - 1]->Angle = -(360.0f / 40.0f) * i - 40.0f;

				g_vBullet[g_vBullet.size() - 1]->cc = CircleMake(g_vBullet[g_vBullet.size() - 1]->pos.x + cosf(g_vBullet[g_vBullet.size() - 1]->Angle * TORADIAN),
					g_vBullet[g_vBullet.size() - 1]->pos.y - sinf(g_vBullet[g_vBullet.size() - 1]->Angle * TORADIAN ), 4.0f);
			}
		}
		m_ShotDelay = 0.0f;
	}
}

void Bullet::Spiral_Bullet(float AngleRate, float Speed)
{
	for (g_vi3Bullet = g_v3Bullet.begin(); g_vi3Bullet != g_v3Bullet.end();)
	{

		//각도가 2파이 값을 넘으면 빼준다. 그리하여 유효 범위를 유지할 수 있게 해준다.
		//if ((*g_viBullet)->Angle >= 360)
		//{
		//	(*g_viBullet)->Angle -= 360;
		//}
		(*g_vi3Bullet)->RotateTime += g_ETime;


		//Speed = (*g_viBullet)->Speed;
		//Speed *= g_ETime;
		(*g_vi3Bullet)->cc.x += Speed * cosf(((*g_vi3Bullet)->Angle) + (AngleRate * g_ETime) * TORADIAN);
		(*g_vi3Bullet)->cc.y -= Speed * sinf(((*g_vi3Bullet)->Angle) + (AngleRate * g_ETime) * TORADIAN);
		//(*g_vi2Bullet)->Angle += ((*g_vi2Bullet)->AngleRate  * g_ETime);

		if (IntersectCircle((*g_vi3Bullet)->cc, _Player.cc))
		{
			OBJECTPOOL(ENEMY_BULLET)->PushObject((*g_vi3Bullet));
			g_vi3Bullet = g_v2Bullet.erase(g_vi3Bullet);
			continue;
			//cout << "충돌" << endl;
		}

		if ((*g_vi3Bullet)->cc.x <= 0.0f || (*g_vi3Bullet)->cc.x >= Map.left || (*g_vi3Bullet)->cc.y <= 0.0f || (*g_vi3Bullet)->cc.y >= WINSIZEY)
		{
			OBJECTPOOL(ENEMY_BULLET)->PushObject((*g_vi3Bullet));
			g_vi3Bullet = g_v3Bullet.erase(g_vi3Bullet);
			continue;
		}

		g_vi3Bullet++;


	}

	if (m_SpiralDelay >= 0.2f)
	{
		if (MaxBullet * 10 > g_v3Bullet.size())
		{
			g_v3Bullet.emplace_back(OBJECTPOOL(ENEMY_BULLET)->GetObObject());
			g_v3Bullet[g_v3Bullet.size() - 1]->pos.x = BOSS_CC.x;
			g_v3Bullet[g_v3Bullet.size() - 1]->pos.y = BOSS_CC.y;
			g_v3Bullet[g_v3Bullet.size() - 1]->AngleRate = AngleRate;
			g_v3Bullet[g_v3Bullet.size() - 1]->SpeedRate = 0.0f;
			g_v3Bullet[g_v3Bullet.size() - 1]->RotateTime = 0.0f;
			//g_v2Bullet[g_v2Bullet.size() - 1]->Angle = 270.0f * AngleRate + g_ETime;
			for (int i = 0; i < 4; i++)
			{
				//g_v3Bullet[g_v3Bullet.size() - 1]->Angle = 
			}
			g_v3Bullet[g_v3Bullet.size() - 1]->Angle = 270.0f;

			//g_vBullet[g_vBullet.size() - 1]->Angle - (g_vBullet[g_vBullet.size() - 1]->AngleRate *= g_ETime);


			g_v3Bullet[g_v3Bullet.size() - 1]->cc = CircleMake(g_v3Bullet[g_v3Bullet.size() - 1]->pos.x + cosf(g_v3Bullet[g_v3Bullet.size() - 1]->Angle),
				g_v3Bullet[g_v3Bullet.size() - 1]->pos.y - sinf(g_v3Bullet[g_v3Bullet.size() - 1]->Angle), 4.0f);
			//g_vBullet[g_vBullet.size() - 1]->Speed = 200.0f * g_ETime;

			//g_vBullet[g_vBullet.size() - 1]->Angle -= floor(g_vBullet[g_vBullet.size() - 1]->Angle);
		}
		m_SpiralDelay = 0.0f;
	}

}

HRESULT Bullet::init()
{
	//총알 생성 시 GetObObject
	//g_vBullet.emplace_back(OBJECTPOOL(BULLET)->GetObObject());


	//반환할 때 때 (오브젝트를 Push 해준다)
	//OBJECTPOOL(BULLET)->PushObject(&g_viBullet); //이터레이터 반환

	m_ShotDelay = 0.0f;
	m_SpiralDelay = 0.0f;
	m_RandomDelay = 0.0f;
	m_PlayerShotDelay = 0.0f;
	m_BulletTime = 0.0f;

	m_BulletNum = 0;

	_Player.pos.x = 420.0f;
	_Player.pos.y = 520.0f;
	_Player.speed = 120.0f;

	m_BossFrameX = 0;
	m_BossFrameY = 0;
	m_BossFrameWidth = 80.0f;
	m_BossFrameHeight = 100.0f;
	m_FrameSpeed = 1.0f / 17.0f;
	m_time = 0.0f;

	Map = RectMake(880.0f, 0.0f, 400.0f, WINSIZEY);
	return S_OK;
}

void Bullet::release()
{
}

void Bullet::update()
{
	Loop -= 1;
	m_ShotDelay += g_ETime;
	m_SpiralDelay += g_ETime;
	m_RandomDelay += g_ETime;
	m_PlayerShotDelay += g_ETime;

	m_BulletTime += g_ETime;


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

	BOSS_CC = CircleMake(440.0f, 200.0f, 30.0f);
	//Boss_rc = RectMake(400.0f, 150.0f, 80.0f, 100.0f);


	//Spiral_Bullet(-270.0f, 120.0f * g_ETime);



	switch (m_BulletNum)
	{
	case 1:
		Circle_Bullet(20.0f, 100.0f * g_ETime, 10.0f);
		break;
	case 2:
		Reverse_Circle_Bullet(20.0f, 130.0f * g_ETime, 0.0f);
		break;
	case 3:
		N_Way_Bullet(12, 0.0f, 70.0f * g_ETime);
		break;
	}
	
	if (m_BulletTime >= 5.0f)
	{
		m_BulletNum = GetFromIntTo(1, 3);
		m_BulletTime = 0.0f;
	}

	if (_isPlayerAlive == true)
	{
		PlayerBullet();
	}


	//BULLET과 Player 충돌 처리
	//update 문 밖으로 또 서술한 이유는
	//vBullet 으로 선언된 패턴이 실행될 때 v2Bullet 패턴 실행하고 나온 Bullet과 Player가 충돌이 안되기 때문

	//시간흐름에 따른 프레임 증가
	if (m_time >= m_FrameSpeed)
	{
		m_time -= m_FrameSpeed;
	
		if (m_BossFrameY == 0)
		{
			m_BossFrameX++;
			if (m_BossFrameX == 18)
			{
				m_BossFrameX = 0;
			}
		}
	}
	else
	{
		m_time += g_ETime;
	}


}

void Bullet::render()
{


	IMGMANAGER->loopRender("BackGround", g_MemDC, 0, 0, 880.0f, Loop);
	DrawfRect(g_MemDC, Map);
	
	IMGMANAGER->render("Player", g_MemDC, _Player.pos.x, _Player.pos.y);
	DrawfCircle(g_MemDC, _Player.cc);

	for (g_viPBullet = g_vPBullet.begin(); g_viPBullet != g_vPBullet.end(); ++g_viPBullet)
	{
		if (_isPlayerAlive == true)
		{
			IMGMANAGER->render("P_Bullet", g_MemDC, (*g_viPBullet)->cc.x - 7.0f, (*g_viPBullet)->cc.y - 7.0f);

		}
	}

	for (g_vi2Bullet = g_v2Bullet.begin(); g_vi2Bullet != g_v2Bullet.end(); ++g_vi2Bullet)
	{
		if ((*g_vi2Bullet)->ColorType == 2)
		{
			IMGMANAGER->render("E_Bullet2", g_MemDC, (*g_vi2Bullet)->cc.x - 7.0f, (*g_vi2Bullet)->cc.y - 7.0f);
		}
	}

	for (g_viBullet = g_vBullet.begin(); g_viBullet != g_vBullet.end(); ++g_viBullet)
	{
		if ((*g_viBullet)->ColorType == 1)
		{
			IMGMANAGER->render("E_Bullet1", g_MemDC, (*g_viBullet)->cc.x - 7.0f, (*g_viBullet)->cc.y - 7.0f);
		}
		
		if ((*g_viBullet)->ColorType == 2)
		{
			IMGMANAGER->render("E_Bullet2", g_MemDC, (*g_viBullet)->cc.x - 7.0f, (*g_viBullet)->cc.y - 7.0f);
		}

	}
	for (g_vi3Bullet = g_v3Bullet.begin(); g_vi3Bullet != g_v3Bullet.end(); ++g_vi3Bullet)
	{
		IMGMANAGER->render("E_Bullet1", g_MemDC, (*g_vi3Bullet)->cc.x - 7.0f, (*g_vi3Bullet)->cc.y - 7.0f);
	}

	IMGMANAGER->frameRender("Boss", g_MemDC, m_BossFrameX, m_BossFrameY, BOSS_CC.x - 40.0f, BOSS_CC.y - 55.0f, m_BossFrameWidth, m_BossFrameHeight);
	IMGMANAGER->render("Number", g_MemDC, 935.0f, 116.0f);


	//for (g_vi2Bullet = g_v2Bullet.begin(); g_vi2Bullet != g_v2Bullet.end(); ++g_vi2Bullet)
	//{
	//	IMGMANAGER->render("E_Bullet2", g_MemDC, (*g_vi2Bullet)->cc.x - 7.0f, (*g_vi2Bullet)->cc.y - 7.0f);
	//	//DrawfCircle(g_MemDC, (*g_viBullet)->cc);

	//}


	//IMGMANAGER->render("E_Bullet1", g_MemDC, g_vBullet[g_vBullet.size() - 1]->cc.x, g_vBullet[g_vBullet.size() - 1]->cc.y);
	

}

