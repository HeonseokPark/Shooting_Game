#include "stdafx.h"
#include "GameNode.h"


LRESULT GameNode::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		g_ptMouse.x = static_cast<float>(LOWORD(lParam));
		g_ptMouse.y = static_cast<float>(HIWORD(lParam));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	}
	return (DefWindowProc(hWnd, message, wParam, lParam));
}

GameNode::GameNode()
{
}


GameNode::~GameNode()
{
}
