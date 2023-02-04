#include "stdafx.h"
#include "..\Public\Bullet.h"


CBullet::CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBaseObj(pDevice, pContext)
{
}

CBullet::CBullet(const CBullet & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet::Initialize(void * pArg)
{
	memcpy(&m_BulletDesc, pArg, sizeof(BULLETDESC));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if(m_BulletDesc.eBulletType == PLAYER)
		CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, this);
	else if (m_BulletDesc.eBulletType == MONSTER)
		CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, this);

}

void CBullet::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;

	if (m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	//CB
	if (m_BulletDesc.eBulletType == PLAYER)
	{
		//if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, )
	}
	else
	{

	}


}

HRESULT CBullet::Render()
{
	return S_OK;
}

HRESULT CBullet::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CBullet::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f * m_BulletDesc.fVelocity;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet::SetUp_ShaderResources()
{
	return S_OK;
}

HRESULT CBullet::SetUp_ShaderID()
{
	return S_OK;
}



void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
