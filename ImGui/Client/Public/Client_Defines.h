#pragma once
#pragma warning (disable : 4996)
namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };

	enum OBJID { OBJ_BACKGROUND, OBJ_MONSTER, OBJ_BLOCK, OBJ_INTERATIVE, OBJ_UNINTERATIVE, OBJ_END };
	enum CONSTRUCT_ID { CONSTRUCT_HOUSE, CONSTRUCT_TREE, CONSTRUCT_END};
	enum SHADER_ID { SHADER_DEFAULT, SHADER_WIREFRAME, SHADER_PICKED, SHADER_SYMBOL};

	enum MODEL { MODEL_ALPHEN, MODEL_SION, MODEL_RINWELL, MODEL_LAW, MODEL_ICEWOLF, MODEL_BERSERKER, MODEL_HAWK, MODEL_ASTRAL_DOUBT, MODEL_TREASURE, MODEL_END };
	

}


#include <process.h>
#include <commdlg.h>

// Client_Defines.h

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;

using namespace Client;
using namespace std;

#define MAX_NUM 9999

