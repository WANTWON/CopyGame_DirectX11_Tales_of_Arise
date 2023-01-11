#pragma once
#include "NonAnim.h"


BEGIN(Client)

class CCrane final : public CNonAnim
{
public:

private:
	CCrane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCrane(const CCrane& rhs);
	virtual ~CCrane() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

	void Input_Dir(DIRINPUT eDIr);
	void Setting_Default();
	_bool Get_Finished() { return m_bFinished; }

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	_float		m_fVelocity = 0.f;
	_float		m_fMoveSoundTime = 0.f;
	_bool		m_IsMoved[DIR_END] = { false };
	_bool		m_bMove = false;
	DIRINPUT	m_eInputDir = DIR_END;
	_bool		m_bUp = false; 
	_bool		m_bFinished = false;
	CBaseObj*	m_pCollisionObj = nullptr;

public:
	static CCrane* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END