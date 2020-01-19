#pragma once
#include "GameNode.h"
#include "Bullet.h"

struct Boss
{
	FCIRCLE cc;		//충돌범위
	float	speed;	//스피드
	//UINT	FrameX;	//플레이어 X 프레임
	FPOINT	pos;	//중심 좌표
	bool P_isCollision; //Player와 충돌되었는지
	bool B_isCollision;	//Player의 Bullet과 충돌되었는지

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
