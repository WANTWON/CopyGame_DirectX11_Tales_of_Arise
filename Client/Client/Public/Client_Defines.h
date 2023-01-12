#pragma once

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };

	enum SOUND_ID { SOUND_BGM, SOUND_SYSTEM, SOUND_VOICE, SOUND_FOOT, SOUND_WEAPON, SOUND_EFFECT, SOUND_OBJECT, SOUND_END };

}

#include <process.h>

// Client_Defines.h

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;

using namespace Client;