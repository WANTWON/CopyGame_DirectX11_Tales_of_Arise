#pragma once

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum DIRID { DIR_STRAIGHT, DIR_BACKWARD, DIR_LEFT, DIR_RIGHT, DIR_STRAIGHT_LEFT, DIR_STRAIGHT_RIGHT, DIR_BACKWARD_LEFT, DIR_BACKWARD_RIGHT, DIR_END };
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_SNOWFIELD, LEVEL_BATTLE, LEVEL_END };
	enum SOUND_ID { SOUND_BGM, SOUND_SYSTEM, SOUND_VOICE, SOUND_FOOT, SOUND_WEAPON, SOUND_EFFECT, SOUND_EFFECT_SION, SOUND_EFFECT_ALPHEN, SOUND_OBJECT, SOUND_END };
	
	/* Shader Enums */
	enum SHADER_RECT { SHADER_RECT_DEFAULT, SHADER_RECT_UI, SHADER_RECT_EFFECT, SHADER_RECT_GLOW };
	enum SHADER_ANIMID { SHADER_ANIMDEFAULT, SHADER_ANIMSHADOW, SHADER_ANIM_DISSOLVE, SHADER_ANIM_GLOW, SHADER_ANIM_GLOW_DISSOLVE };
	enum SHADER_NONANIMID { SHADER_NONANIMDEFAULT, SHADER_NONANIM_SHADOW, SHADER_NONANIM_WATER, SHADER_NONANIM_DISSOLVE, SHADER_NONANIM_EFFECT, SHADER_NONANIM_GLOW, SHADER_NONANIM_DISTORT };
	enum SHADER_EFFECT { SHADER_DEFAULT, SHADER_EFFECT, SHADER_GLOW, SHADER_DISTORTION, SHADER_END };

	enum THREAD_ID { CLONE_PLAYER , CLONE_PLAYER2,  CLONE_NPC, CLONE_MONSTER1, CLONE_MONSTER2, CLONE_MONSTER3, CLONE_NONANIM, THREAD_END };

	enum OBJECT_ID {PLAYER, MONSTER, NPC, OBJECT_END};
	enum PLAYER_MODE { ACTIVE, AI_MODE, UNVISIBLE };

	enum ITEM_NAME { ITEMNAME_LEMONJELLY, ITEMNAME_HWANGJELLY , ITEMNAME_GRAPEJELLY, ITEMNAME_PINEAPPLEJELLY, ITEMNAME_OMEGAELIXIR,
		ITEMNAME_PANASIABOTTLE , ITEMNAME_LIFEBOTTLE, ITEMNAME_REDSAGE , ITEMNAME_POTATO, ITEMNAME_TOMATO,ITEMNAME_APPLE,
		ITEMNAME_LETTUCE,ITEMNAME_HERB , ITEMNAME_SMALLHERB , ITEMNAME_DULLSWORD, ITEMNAME_ARSORSWORD,ITEMNAME_PYOGOMUSHROOM,
		ITEMNAME_SONG2MUSHROOM , ITEMNAME_SALTROCK, ITEMNAME_END };
	enum ITEM_TYPE { ITEMTYPE_JELLY , ITEMTYPE_MATERIAL ,ITEMTYPE_SWORD, ITEMTYPE_ARMOR, ITEMTYPE_ACCESSORIES, ITEMTYPE_ACCMATERIAL,
		ITEMTYPE_GRAIN , ITEMTYPE_LEAVES, ITEMTYPE_VEGITABLE, ITEMTYPE_MEAT, ITEMTYPE_FRUIT, ITEMTYPE_ELSE,
		ITEMTYPE_CONDIMENT, ITEMTYPE_FISH, ITEMTYPE_RECIPE, ITEMTYPE_GUN, ITEMTYPE_BOOK, ITEMTYPE_GAUNTLET,
		ITEMTYPE_KEY , ITEMTYPE_POTION , ITEMTYPE_END };

	enum QUESTITEM_NAME{QUEST_LETTUCE , QUEST_HERB , QUEST_END};

	//enum MONSTER_ID { ICE_WOLF, HAWK, BERSERKER, SLIME, MONSTER_END };

	enum MONSTER_ID { ICE_WOLF, HAWK, BERSERKER, SLIME, RINWELL, ASTRAL_DOUBT, MONSTER_END };


	typedef struct ObjectInfo
	{
		float fMaxHp;
		float fCurrentHp;
		float fMaxMp;
		float fCurrentMp;
		int iDamage;
		float fCurrentBoostGuage;
		int iCurrentExp;
		int iMaxExp;
		int iLevel;
		float fCurrentOverlimitGauge = 100.f;

	}OBJINFO;

	typedef struct iteminfo
	{
		ITEM_NAME eitemname;
		ITEM_TYPE eitemtype;
		int icount = 0;
		

	}ITEMINFO;

	/*For Anim State*/
	enum STATETYPE
	{
		STATETYPE_START,
		STATETYPE_MAIN,
		STATETYPE_END,
		STATETYPE_DEFAULT
	};

#define MAXDISTANCE 999999.f
#define GET_PLUSMINUS (rand() %2 == 0? 1 : -1)
}

#include <process.h>
#include <time.h>

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;
extern bool		g_bUIMade;
extern float	g_fSoundVolume;

using namespace Client;