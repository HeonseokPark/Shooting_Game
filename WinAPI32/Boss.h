#pragma once
#include "GameNode.h"
#include "Bullet.h"

struct Boss
{
	FCIRCLE cc;		//�浹����
	float	speed;	//���ǵ�
	//UINT	FrameX;	//�÷��̾� X ������
	FPOINT	pos;	//�߽� ��ǥ
	bool P_isCollision; //Player�� �浹�Ǿ�����
	bool B_isCollision;	//Player�� Bullet�� �浹�Ǿ�����

};

class Boss1 : public GameNode
{
	


public:
	Boss1();
	~Boss1() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};


class Boss2 : public GameNode
{



public:
	Boss2();
	~Boss2() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};


class Boss3 : public GameNode
{



public:
	Boss3();
	~Boss3() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};
