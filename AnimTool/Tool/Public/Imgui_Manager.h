#pragma once

#include "Client_Defines.h"
#include "Base.h"


class CImgui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImgui_Manager)
public:
	CImgui_Manager();
	virtual ~CImgui_Manager() = default;


public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void Render();
	void ShowGui();

public:
	bool Get_WantCaptureMouse(void) { return ImGui::GetIO().WantCaptureMouse; }

public:
	_bool m_isLoad = false;
	_bool m_isSaveComplete = false;

	vector<char*> m_AnimObj;
	vector<char*> m_NonAnimObj;
	vector<char*> m_WeaponObj;

	_int m_iLoadModel = -1;
	_int m_iPreModel = -1;

	_int m_iBoneChoice = -1;
	_int m_iAnimationChoice = -1;

	_float m_fBoneRotation[3] = { 0.f, 0.f, 0.f };
	_float m_fBoneScale[3] = { 0.f, 0.f, 0.f };
	_float m_fBonePosition[3] = { 0.f, 0.f, 0.f };

	_float m_fAnimationDuration = 0.f;
	_float m_fAnimationTime = 0.f;

	_bool m_isAnimationPlay = false;
	_bool m_isAnimationEdit = false;

	_int m_iTickChoice = -1;
	_int m_iTimeChoice = -1;

	_float m_fAnimationTickTime = 0.f;
	_float m_fAnimationTick = 0.f;

	_bool m_isWeaponWindow = false;
	_int m_iWeaponChoice = -1;

	_bool m_isEventWindow = false;
	_float m_fEventStartTime = 0.f;
	_float m_fEventEndTime = 0.f;
	_int m_iEventType = 0;
	_int m_iEventChoice = -1;



	_int m_iMode;
	_int m_iMapChoice;
	_int m_iType;
	_int m_iChoice;
	_int m_iNonChoice;
	_int m_iFixChoice;
	_int m_iLightChoice;
	_int m_iLightFixChoice;

	_float m_fPlaceRotation[3];
	_float m_fPlaceScale[3] = { 1.f, 1.f, 1.f };

	_float m_fChangeRotation[3];
	_float m_fChangeScale[3] = { 1.f, 1.f, 1.f };
	_float m_fChangePosition[3];

	_int m_iLightMode;

	_float m_fLightPosition[3];
	_float m_fLightChangePosition[3];
	_float m_fRange = 0.f;
	_float m_fChangeRange = 0.f;

	_float m_fLightDirection[3];
	_float m_fLightChangeDirection[3];

	_float m_fLightDiffuse[3];
	_float m_fLightAmbient[3];
	_float m_fLightSpecular[3];

	_float m_fLightChangeDiffuse[3];
	_float m_fLightChangeAmbient[3];
	_float m_fLightChangeSpecular[3];

	vector<char*> m_FloorObj;
	vector<char*> m_MovableObj;
	vector<char*> m_NonMovableObj;
	vector<char*> m_RoomObj;

	vector<_float3> m_CellPoints;
	_bool m_isNavigation;
	_int m_iNaviChoice;
	_float m_fNaviPosition1[3];
	_float m_fNaviPosition2[3];
	_float m_fNaviPosition3[3];
	_bool m_isSort;
	_bool m_isCreateNavi = false;
	_int m_iPreChoice = -1;
	_int m_iCurChoice = -1;

	_bool m_isMapBinary[MAP_END] = { false, };
	_bool m_isNonBinary[NON_END] = { false, };

	_int m_iRoomChoice = -1;
	_float m_fColliderScale[3];

	_float m_fDoorColliderScale[3];
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

public:
	virtual void Free() override;
};

