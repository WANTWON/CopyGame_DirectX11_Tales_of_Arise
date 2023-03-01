#pragma once

#include "InteractObject.h"

BEGIN(Client)
class CShootingObject :	public CInteractObject
{
public:
	enum TYPE { TYPE_A, TYPE_B, TYPE_C, TYPE_D, TYPE_E, TYPE_F, TYPE_END};

public:
	typedef struct tagThrowDesc
	{
		TYPE eType;
		_float fVelocity;
		_bool m_bGoRight = true;
		_int m_iScore = 0;
		NONANIMDESC tNonDesc;
	}SHOOTINGDEC;

protected:
	CShootingObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShootingObject(const CShootingObject& rhs);
	virtual ~CShootingObject() = default;

public:
	TYPE Get_Type(void) { return m_tShootingDesc.eType; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_EdgeDetection() override;

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	virtual HRESULT SetUp_ShaderID() override;

private:
	CNavigation*			m_pNavigationCom = nullptr;

private:
	_vector m_vStartPos;
	_vector m_vTargetPos;
	_float	m_fTime = 0.f;
	_int	m_iTurnAxis;
	_vector m_vTurnAxis;
	_bool	m_bDown = false;

	_bool  m_bhit = false;

	_vector m_vDir;
	SHOOTINGDEC m_tShootingDesc;

	_float m_fDeadtimer = 0.f;

public:
	static CShootingObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};
END