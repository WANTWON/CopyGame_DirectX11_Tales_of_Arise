#pragma once

#include "NonAnim.h"


BEGIN(Client)

class CPrizeItem final : public CNonAnim
{
public:
	enum TYPE { SMALL_KEY, COMPASS, MAP, FEATHER, BOSS_KEY, HEART_RECOVERY,  RUBY_GREEN, CELLO ,
				ARROW, DOGFOOD, HEART_CONTAINER, MAGIC_ROD, BOW, NECKLACE, YOSHIDOLL, MAGICPOWDER, 
				RUBYPURPLE, TAIL_KEY, DRUM, HORN , BELL, TELEPHONE, ITEM_END };

	enum INTERACTTYPE { DEFAULT, PRIZE, CARRYABLE, TELL };
	enum MSGTEX_GETITEM { MSG_SMALL_KEY, MSG_COMPOSS, MSG_DGN_MAP, MSG_FEATHER, MSG_BOSS_KEY, MSG_MSG_HEART, MSG_MGS_RUBY, MSG_CELLO, MSG_ARROW,
		MSG_DOGFOOD, MSG_HEART_CONTAINER, MSG_MAGIC_ROD, MSG_BOW, MSG_NECKLACE, MSG_YOSHIDOLL, MSG_MAGICPOWDER, MSG_RUBYPURPLE,
		MSG_TAIL_KEY, MSG_DRUM, MSG_HORN, MSG_BELL, MSG_MARIN, MSG_END };



	typedef struct KeyTag
	{
		TYPE eType = SMALL_KEY;
		_float3 vPosition = _float3(0.f, 0.f, 0.f);
		INTERACTTYPE eInteractType = DEFAULT;
		
	}ITEMDESC;

	
private:
	CPrizeItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPrizeItem(const CPrizeItem& rhs);
	virtual ~CPrizeItem() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	void LateTick_PrizeModeItem(_float fTimeDelta);
	void LateTick_DefaultModeItem(_float fTimeDelta);
	void LateTick_CarryableModeItem(_float fTimeDelta);
	void LateTick_TelephoneModeItem(_float fTimeDelta);
	void Setting_Get_Item();
	void Setting_TelephoneMessage();
	void Make_GetItemEffect();
	virtual HRESULT SetUp_ShaderResources() override;

private:
	ITEMDESC				m_ItemDesc;
	_bool					m_bGet = false;
	_bool					m_bMakeEffect = false;
	_float4x4				m_CombinedWorldMatrix;

	_float					m_fHeight= 0.f;
	_float					m_fMoveTime = 0.f;
	_bool					m_bTimeMax = false;

	CTexture*				m_pGlowTexture = nullptr;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CPrizeItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END