#pragma once
#include "GameNode.h"

class Scene : public GameNode
{
public:
	Scene();
	~Scene() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;

	bool m_isGameStart;
	bool m_isGameExit;
	FRECT m_StartButton;
	FRECT m_ExitButton;
};

