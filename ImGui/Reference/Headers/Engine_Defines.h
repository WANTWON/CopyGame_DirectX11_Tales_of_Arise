#pragma once

#pragma warning (disable : 4996)
#pragma warning (disable : 4099)
#pragma warning (disable : 4005)
#pragma warning (disable : 4251)
#pragma warning (disable : 4819)

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <Effects11/d3dx11effect.h>
#include <DirectXCollision.h>

#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/ScreenGrab.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/Effects.h>
#include <directXTK/SpriteFont.h>
#include <directXTK/SpriteBatch.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <d3dcompiler.h>


using namespace DirectX;


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

enum DIMK { DIMK_LBUTTON, DIMK_RBUTTON, DIMK_WHEEL, DIMK_END };
enum DIMM { DIMM_X, DIMM_Y, DIMM_WHEEL, DIMM_END };

#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <typeinfo.h>
#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <math.h>
using namespace std;


#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"

enum EVENTTYPE { EVENT_SOUND, EVENT_EFFECT, EVENT_COLLIDER, EVENT_STATE, EVENT_END };

/* for PhysX */
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxConfig.h>

#define PX_RELEASE(x)	if(x != nullptr)	{ x->release(); x = nullptr;	}
#define PVD_HOST "127.0.0.1"

using namespace physx;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

#define OBJ_POOL 2
#define OBJ_DEAD 1
#define OBJ_NOEVENT 0

using namespace Engine;
using namespace TriangleTests;
