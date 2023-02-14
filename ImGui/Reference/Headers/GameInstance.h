#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Timer_Manager.h"
#include "PipeLine.h"
#include "Frustum.h"

#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Font_Manager.h"
#include "Target_Manager.h"
#include "Sound_Manager.h"
#include "Picking.h"
#include "PhysX.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.Engine */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void Tick_Engine(_float fTimeDelta);

	void Clear(_uint iLevelIndex);


public: /* For.Graphic_Device */	
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

public: /* For.Input_Device */
	_char Get_DIKState(_uchar eKeyID);
	_char Get_DIMKeyState(DIMK eMouseKeyID);
	_long Get_DIMMoveState(DIMM eMouseMoveID);
	_bool Key_Down(_uchar eKeyID);
	_bool Key_Up(_uchar eKeyID);
	_bool Key_Pressing(_uchar eKeyID);
	_bool Mouse_Down(DIMK eMouseKeyID);
	_bool Mouse_Up(DIMK eMouseKeyID);
	_bool Mouse_Pressing(DIMK eMouseKeyID);

public: /* For.Timer_Manager */
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float Get_TimeDelta(const _tchar* pTimerTag);
	void Update_TimeDelta(const _tchar* pTimerTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(unsigned int iLevelIndex, class CLevel* pNewLevel);
	class CLevel* Get_CurrentLevel();
	_uint Get_CurrentLevelIndex();
	_uint Get_PastLevelIndex();
	_uint Get_DestinationLevelIndex();
	void Set_DestinationLevel(_uint LevelIndex);

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT Add_GameObject_Out(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, OUT CGameObject*& pGameObjectOut, void* pArg = nullptr);
	HRESULT Add_GameObjectLoad(const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, const _tchar* VIBUfferTag, void* pArg = nullptr);

	class CGameObject* Clone_GameObject(const _tchar* pPrototypeTag, void* pArg = nullptr);
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	class CGameObject* Get_Object(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex = 0);
	list<class CGameObject*>* Get_ObjectList(_uint iSceneID, const _tchar * pLayerTag);
	void Clear_Layer(_uint iLevelIndex, const _tchar* LayerTag);
	HRESULT Out_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, class CGameObject* pGameObject);
	HRESULT ReAdd_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, class CGameObject* pGameObject);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
	HRESULT Check_Prototype(_int iLevelIndex, const _tchar * pPrototypeTag); // �߰�

public: /* For.PipeLine */
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformMatrix);
	_matrix Get_TransformMatrix(CPipeLine::TRANSFORMSTATE  eState);
	_float4x4 Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE  eState);
	_float4x4 Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATE  eState);
	_float4x4 Get_TransformFloat4x4_Inverse(CPipeLine::TRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4_Inverse_TP(CPipeLine::TRANSFORMSTATE eState);
	_float4 Get_CamPosition();
	_float4x4 Get_CamWorldMatrix();

public: /* For.Light_Manager */
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	void Set_LightDesc(_uint iIndex, LIGHTDESC* pLightDesc);
	void Clear_AllLight();
	void Clear_Light(_uint iIndex);
	_uint Get_LightSize();
	HRESULT	Set_ShadowLightView(_float4 vEye, _float4 vAt);
	_float4x4	Get_ShadowLightView();

public: /* For.Font_Manager */
	HRESULT Add_Fonts(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pText, _fvector vPos, _fvector vColor, _float fScale = 1.f);

public: /* For. Sound Manager */
	void PlaySounds(TCHAR* pSoundKey, const _uint& eID, const float& fVolume);
	void PlayBGM(TCHAR * pSoundKey, const float& fVolume);
	void StopSound(const _uint& eID);
	void StopAll();
	void SetChannelVolume(const _uint& eID, const float& fVolume);
	int  VolumeUp(const _uint& eID, const _float& _vol);
	int  VolumeDown(const _uint& eID, const _float& _vol);
	int  Pause(const _uint& eID);

public: /* For Picking */
	_vector Get_RayPos();
	_vector Get_RayDir();

public: /* For.Frustum */
	_bool isIn_WorldFrustum(_fvector vPosition, _float fRange = 0.f);
	_bool isIn_BattleWorldFrustum(_fvector vPosition, _float fRange = 0.f);

public: /* For.Target_Manager */
	ID3D11ShaderResourceView* Get_BackBufferCopySRV();
	HRESULT Bind_RenderTarget_SRV(const _tchar* pTargetTag, class CShader* pShader, const char* pConstantName);

public:
	static void Release_Engine();

private:
	CGraphic_Device*				m_pGraphic_Device = nullptr;
	CInput_Device*					m_pInput_Device = nullptr;
	CLevel_Manager*					m_pLevel_Manager = nullptr;
	CObject_Manager*				m_pObject_Manager = nullptr;
	CTimer_Manager*					m_pTimer_Manager = nullptr;
	CComponent_Manager*				m_pComponent_Manager = nullptr;
	CPipeLine*						m_pPipeLine = nullptr;
	CLight_Manager*					m_pLight_Manager = nullptr;
	CFont_Manager*					m_pFont_Manager = nullptr;
	CFrustum*						m_pFrustum = nullptr;
	CTarget_Manager*				m_pTarget_Manager = nullptr;
	CSound_Manager*					m_pSound_Manager = nullptr;
	CPicking*						m_pPicking = nullptr;
	CPhysX*							m_pPhysX = nullptr;
public:
	virtual void Free() override;
};

END