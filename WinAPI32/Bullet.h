#pragma once
#include "GameNode.h"
#include "Boss.h"
#include "PlayerInfo.h"


enum BULLET_NUM
{
	CIRCLE_BULLET = 1,
	SPIRAL_BULLET

};

//Bullet ����ü
struct ENEMY_BULLET
{
	UINT ColorType;
	FCIRCLE cc;
	FPOINT pos;		//��ǥ
	float Angle;	//����
	float Speed;	//�ӵ�
	float AngleRate; //���ӵ�
	float SpeedRate; //���ӵ�
	float RotateTime;
};

struct PLAYER_BULLET
{
	UINT bulletNum;
	FCIRCLE cc;
	FPOINT pos;		//��ǥ
	float Angle;	//����
	float Speed;	//�ӵ�
	float AngleRate; //���ӵ�
	float SpeedRate; //���ӵ�
	float RotateTime;
};

class Bullet : public GameNode, public PlayerInfo
{	
public:
	vector<ENEMY_BULLET*> g_vBullet;
	vector<ENEMY_BULLET*>::iterator g_viBullet;

	vector<ENEMY_BULLET*> g_v2Bullet;
	vector<ENEMY_BULLET*>::iterator g_vi2Bullet;

	vector<ENEMY_BULLET*> g_v3Bullet;
	vector<ENEMY_BULLET*>::iterator g_vi3Bullet;

	vector<PLAYER_BULLET*> g_vPBullet;
	vector<PLAYER_BULLET*>::iterator g_viPBullet;

	FCIRCLE BOSS_CC;		//���� �浹����
	FRECT Boss_rc;
	float m_ShotDelay; //Bullet �߻� ������
	float m_RandomDelay;
	float m_SpiralDelay;
	float m_PlayerShotDelay; //�÷��̾��� Bullet �߻� ������
	UINT MaxBullet;
	bool _isPlayerAlive;

	float m_BulletTime;

	UINT m_BulletNum;

	int Loop;

	FRECT Map;
	
	float m_FrameSpeed;
	float m_time;

	int m_BossFrameX;
	int m_BossFrameY;
	int m_BossFrameWidth;
	int m_BossFrameHeight;

public:
	Bullet();
	~Bullet() override;
	void Circle_Bullet(float AngleRate, float Speed, float SpeedRate);
	void PlayerBullet();
	void Reverse_Circle_Bullet(float AngleRate, float Speed, float SpeedRate);
	void N_Way_Bullet(int N, float AngleRate, float Speed);
	void Spiral_Bullet(float AngleRate, float Speed);
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
	//AngleRate = ���ӵ�, SpeedRate = ���ӵ�

};

