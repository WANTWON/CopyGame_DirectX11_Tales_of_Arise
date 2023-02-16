#include "stdafx.h"
#include "..\Public\BaseObj.h"


CBaseObj::CBaseObj(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBaseObj::CBaseObj(const CBaseObj & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBaseObj::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CBaseObj::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CBaseObj::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bDead)
		return OBJ_DEAD;


	return OBJ_NOEVENT;
}

void CBaseObj::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CBaseObj::Render()
{
	return S_OK;
}



_vector CBaseObj::Get_Position()
{
	if (m_pTransformCom == nullptr)
		return _vector();


	return m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
}

void CBaseObj::Set_State(CTransform::STATE eState, _fvector vState)
{
	if (m_pTransformCom == nullptr)
		return;

	m_pTransformCom->Set_State(eState, vState);
}

void CBaseObj::Set_Scale(_float3 vScale)
{
	m_vScale = vScale;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_vScale.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_vScale.y);
	m_pTransformCom->Set_Scale(CTransform::STATE_LOOK, m_vScale.z);

}

_vector CBaseObj::Get_TransformState(CTransform::STATE eState)
{
	if (m_pTransformCom == nullptr)
		return _vector();

	if (eState == CTransform::STATE_TRANSLATION)
		return XMVectorSetW(m_pTransformCom->Get_State(eState), 1.f);

	return m_pTransformCom->Get_State(eState);
}

void CBaseObj::Free()
{
	__super::Free();
}