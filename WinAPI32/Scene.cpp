#include "stdafx.h"
#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}

HRESULT Scene::init()
{
	m_StartButton = RectMake(WINSIZEX / 2.0f - 95.0f, 500.0f, 190.0f, 64.0f);
	m_ExitButton = RectMake(WINSIZEX / 2.0f - 95.0f, 600.0f, 187.0f, 64.0f);
	m_isGameStart = false;
	m_isGameExit = false;
	return S_OK;
}

void Scene::release()
{
}

void Scene::update()
{
	if (PtInRect(m_StartButton, g_ptMouse))
	{
		if (KEYMANAGER->isKeyUp(VK_LBUTTON))
		{
			m_isGameStart = true;
		}
	}
	if (PtInRect(m_ExitButton, g_ptMouse))
	{
		if (KEYMANAGER->isKeyUp(VK_LBUTTON))
		{
			m_isGameExit = true;
		}
	}
}

void Scene::render()
{
	if (m_isGameStart == false)
	{
		IMGMANAGER->render("MainMenu", g_MemDC);
		IMGMANAGER->render("Start", g_MemDC, WINSIZEX / 2.0f - 95.0f, 500.0f);
		IMGMANAGER->render("Exit", g_MemDC, WINSIZEX / 2.0f - 95.0f, 600.0f);
	}
}
