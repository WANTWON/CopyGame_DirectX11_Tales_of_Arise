#pragma once

#include "NonAnim.h"



BEGIN(Client)

class CDgnKey final : public CNonAnim
{
public:
	enum KEY_TYPE { SMALL_KEY, TAILCAVE_KEY };

	typedef struct KeyTag
	{
		KEY_TYPE eType = SMALL_KEY;
		_float3 vPosition = _float3(0.f, 0.f, 0.f);

	}DGNKEYDESC;


private:
	CDgnKey(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDgnKey(const CDgnKey& rhs);
	virtual ~CDgnKey() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	void Make_GetItemEffect();
	virtual HRESULT SetUp_ShaderResources() override; 

private:
	DGNKEYDESC				m_eKeyDesc;
	_bool					m_bGet = false;
	_bool					m_bMakeEffect = false;
	_bool					m_bFirst = false;
	CTexture*				m_pGlowTexture = nullptr;


private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CDgnKey* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END