#pragma once

#include "BaseObj.h"
BEGIN(Engine)
class CModel;
END


BEGIN(Client)

class CCraneButton final : public CBaseObj
{
public:
	enum STATE { RIGHT_OFF, RIGHT_OFF_LP, RIGHT_ON, RIGHT_ON_LP, RIGHTUP_OFF, RIGHTUP_OFF_LP,
		RIGHTUP_ON, RIGHTUP_ON_LP, UP_OFF, UP_OFF_LP, UP_ON, UP_ON_LP};

private:
	CCraneButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCraneButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Change_Animation(_float fTimeDelta);
	void Play_CraneGame(_float fTimeDelta);
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override;

private:
	CModel*					m_pModelCom = nullptr;
	STATE					m_eState = RIGHTUP_OFF_LP;
	STATE					m_ePreState = RIGHT_OFF;
	_bool					m_bFirst = false;
	_bool					m_bPlay = false;
	_bool					m_bSoundOnce = false;
	

public:
	static CCraneButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END