#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "Level_Manager.h"
#include "Terrain_Manager.h"
#include "ModelManager.h"
#include "NonAnim.h"
#include "Navigation_Manager.h"
#include "Effect_Manager.h"
#include "Camera_Manager.h"
#include "TreasureBox.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CImgui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImgui_Manager)

public:
	enum PICKING_TYPE { PICKING_TERRAIN_TRANSFORM,
		PICKING_TERRAIN_SHAPE, PICKING_TERRAIN_BRUSH,
	PICKING_MODEL,};

	enum COLOR { RED, GREEN, BLUE};

private:
	CImgui_Manager();
	virtual ~CImgui_Manager() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void Render();
	void Tick_Imgui();

public:
	PICKING_TYPE Get_PickingType() {return (PICKING_TYPE)m_PickingType;}
	_bool Get_CameraPicking() { return m_bMakeCamera; }
	_int Get_BrushType() { return m_eBrushType; }
public:
	/* For Debug*/
	void ShowSimpleMousePos(bool* p_open);
	void ShowPickedObj();
	void Show_PopupBox();

	/* For Terrain Tool */
	void Set_Terrain_Map();
	void Set_Terrain_Shape();
	void Save_Terrain();
	void Load_Terrain();
	void Set_HeightMap();
	void Set_Brush();

	/* For Model Tool */
	void Set_Object_Map();
	void Set_LayerTag();
	void Set_Macro();
	void Show_ModelList();
	void Show_CurrentModelList();
	void BrowseForFolder();

	void Set_TrasureBox();
	void Save_TrasureBox();
	void Load_TrasureBox();

	/* For Navigation Tool */
	void Set_Navigation();
	void Save_Navigation();
	void Load_Navigation();
	_bool Get_ShowOnlyNavi() { return m_bShowOnlyNavi; }

	/* For Camera Tool */
	void Set_Camera();
	void Save_Camera();
	void Load_Camera();

	/* For Light Tool */
	void Set_Light();
	void Save_Light();
	void Load_Light();

	/* For Effect Tool */
	void Draw_EffectModals();

	void Read_EffectsData();
	void Read_Textures(_tchar* pFolderPath);
	void Read_Meshes(_tchar* pFolderPath);

	void Set_Effect();
	void Save_Effect();
	void Load_Effect();

public:
	void Create_Model(const _tchar* pPrototypeTag, const _tchar* pLayerTag, _bool bCreatePrototype = false);
	void Add_PrototypeTag(_tchar* TempTag) { m_TempLayerTags.push_back(TempTag); }
	void Add_PrototypeTag(char* TempTag) { m_TempCharTags.push_back(TempTag); }
	
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	_bool m_bShowSimpleMousePos = false;
	_bool m_bShowPickedObject = false;
	_bool m_bShow_app_style_editor = false;
	_bool m_bSave = false;
	_bool m_bLoad = false;
	LEVEL m_iCurrentLevel = LEVEL_GAMEPLAY;

private:
	/* For Terrain */
	CTerrain_Manager*						m_pTerrain_Manager = nullptr;
	CTerrain_Manager::TERRAINDESC			m_pTerrainDesc;
	CTerrain_Manager::TERRAINSHAPEDESC		m_TerrainShapeDesc;
	_bool									m_bTerrain_Show = true;
	_int									m_eBrushType = RED;


	/* For Object */
	CModelManager*							m_pModel_Manager = nullptr;
	NONANIMDESC					m_InitDesc;
	_tchar									m_pFilePath[MAX_PATH] = L"../../../Bin/Resources/Meshes/";
	vector<string>							m_stLayerTags;
	vector<const _tchar*>					m_TempLayerTags;
	vector<const _tchar*>					m_TerrainTags;
	vector<char*>							m_TempCharTags;
	_int									m_iCreatedSelected = 0;
	_int									m_iSeletecLayerNum = 0;
	_int									m_iBmpTerrainNum = 0;
	_float									m_fDist = 1.f;
	_float3									m_vfOffset = _float3(0.f, 0.f, 0.f);

	//TreasureBox
	CTreasureBox::BOXTAG					m_BoxDesc;
	_int									m_iTreasureIndex = 0;
	_float3									m_fTreasureBoxPos = _float3(0.f, 0.f, 0.f);
	_int									m_ItemType = 0;
	_bool									m_Visible = false;

	/* For Picking */
	_float3									m_vPickedObjPos = _float3(1.f, 1.f, 1.f);
	_float3									m_vPickedObjScale = _float3(1.f, 1.f, 1.f);
	_int									m_PickingType = 0;
	_float3									m_vPickedRotAxis = _float3(0.f, 1.f, 0.f);
	_float									m_fRotAngle = 0.f;

	/*For Navigation*/
	CNavigation_Manager*					m_pNavigation_Manager = nullptr;
	_bool									m_bNaviPicking = false;
	_int									m_iCellIndex = 0;
	_int									m_iCellType = 0;
	_float3									m_fClickPoint = _float3(0.f, 0.f, 0.f);
	_bool									m_bShowOnlyNavi = false;

	/*For Camera*/
	CCamera_Manager*						m_pCamera_Manager = nullptr;
	_bool									m_bMakeCamera = false;
	_int									m_iCameraIndex = 0;
	_float3									m_fCamPosition = _float3(0.f, 0.f, 0.f);

	/*For Light */
	_bool									m_bMakeLight = false;
	_int									m_iLightIndex = 0;
	_int									m_iLightType = 0;
	LIGHTDESC								m_LightDesc;

	/*
	* For Effect
	*/
	enum TRANSFORM_TYPE { TRANS_SCALE, TRANS_ROTATION, TRANS_END };

	CEffect_Manager*						m_pEffectManager = nullptr;
	vector<string>							m_SavedEffects;
	_int									m_iSavedEffect;
	string									m_sCurrentEffect;
	
	/* Effect Resources (Texture, Meshes) */
	list<string>							m_TextureNames;
	list<string>							m_MeshNames;
	string									m_sSelectedTexture;
	string									m_sSelectedMesh;
	string									m_sSelectedEffect;
	CEffect*								m_pSelectedEffect = nullptr;
	
	/* Effect Settings */
	_bool									m_bIsPlaying = false;
	TRANSFORM_TYPE							m_eEffectTransformation = TRANS_SCALE;
	class CTransform*						m_pEffectTransform = nullptr;
	_float									m_fX = 1.f, m_fY = 1.f, m_fZ = 1.f;
	_bool									m_bBillboard = true;

	_float									m_fCurveX = 1.f, m_fCurveY = 1.f, m_fCurveZ = 1.f, m_fCurveTime = 1.f;

public:
	virtual void Free() override;
};
END