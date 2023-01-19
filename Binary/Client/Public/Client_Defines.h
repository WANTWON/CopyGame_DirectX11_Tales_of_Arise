#pragma once

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_GAMEPLAY, LEVEL_END };

	enum NONOBJECT { NON_CHAIR, NON_DESK, NON_BLOODBOOK,
		NON_CABINET, NON_COIN, NON_DRIVER, NON_FIRE, NON_FUSE1,
		NON_FUSE2, NON_FUSE3, NON_MASTERKEY, NON_MAGIC, NON_PAPER,
		NON_POISON, NON_MAP, NON_SOYBEAN, NON_SPRAY, NON_TABLET,
		NON_FRIDAY,	NON_THURSDAY, NON_END };

	enum MAP { MAP_MAIN01, MAP_END };
}

#include <process.h>

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Client;