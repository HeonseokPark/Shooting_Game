#pragma once
#include "GameNode.h"

struct tagPlayer
{
	FCIRCLE cc;		//�浹����
	float	speed;	//���ǵ�
	//UINT	FrameX;	//�÷��̾� X ������
	FPOINT	pos;	//�߽� ��ǥ
	bool isCollision; //Bullet or Enemy�� �浹 �Ǿ����� �ƴ���

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

