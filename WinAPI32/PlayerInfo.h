#pragma once
#include "GameNode.h"

struct tagPlayer
{
	FCIRCLE cc;		//충돌범위
	float	speed;	//스피드
	//UINT	FrameX;	//플레이어 X 프레임
	FPOINT	pos;	//중심 좌표
	bool isCollision; //Bullet or Enemy와 충돌 되었는지 아닌지

};

class PlayerInfo : public GameNode
{
public:
	tagPlayer _Player;
	FRECT Map;

public:
	PlayerInfo();
	~PlayerInfo() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};

