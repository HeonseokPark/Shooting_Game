#pragma once
#include "GameNode.h"
#include "PlayerInfo.h"
#include "Boss.h"
#include "Bullet.h"
#include "Scene.h"

class MainGame : public GameNode
{
private:
	PlayerInfo* m_Player;
	Boss1* m_Boss1;
	Bullet* Bullet_Manager;
	Scene* m_Scene;
	int a, b;

	int Loop;


public:
	MainGame();
	~MainGame() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};

