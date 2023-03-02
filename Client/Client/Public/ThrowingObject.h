#pragma once

#include "InteractObject.h"

BEGIN(Client)
class CThrowingObject :	public CInteractObject
{
public:
	enum TYPE { TYPE_APPLE, TYPE_BREAD, TYPE_PINEAPPLE, TYPE_POTATO, TYPE_MANGO, TYPE_REDONION, TYPE_END};

public:
	typedef struct tagThrowDesc
	{
		TYPE eType;
		_float fTime;
		NONANIMDESC tNonDesc;
	}THROWDESC;

protected:
	CThrowingObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CThrowingObject(const CThrowingObject& rhs);
	virtual ~CThrowingObject() = default;

public:
	TYPE Get_Type(void) { return m_tThrowDesc.eType; }

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
	void	Dead_Effect();

private:
	CNavigation*			m_pNavigationCom = nullptr;

private:
	_vector m_vStartPos;
	_vector m_vTargetPos;
	_float	m_fTime = 0.f;
	_int	m_iTurnAxis;
	_vector m_vTurnAxis;
	_bool	m_bDown = false;

	THROWDESC m_tThrowDesc;

public:
	static CThrowingObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};
END