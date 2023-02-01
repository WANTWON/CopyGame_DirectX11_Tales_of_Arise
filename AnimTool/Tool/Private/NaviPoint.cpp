#include "stdafx.h"
#include "..\Public\NaviPoint.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"

CNaviPoint::CNaviPoint(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CNaviPoint::CNaviPoint(const CNaviPoint & rhs)
	: CGameObject(rhs)
{
}

void CNaviPoint::Change_Cell(void)
{
	if (nullptr != m_pVIBufferCom)
		Safe_Release(m_pVIBufferCom);

	CImgui_Manager* pImgui = GET_INSTANCE(CImgui_Manager);

	m_vPoints[0] = _float3(pImgui->m_fNaviPosition1[0], pImgui->m_fNaviPosition1[1], pImgui->m_fNaviPosition1[2]);
	m_vPoints[1] = _float3(pImgui->m_fNaviPosition2[0], pImgui->m_fNaviPosition2[1], pImgui->m_fNaviPosition2[2]);
	m_vPoints[2] = _float3(pImgui->m_fNaviPosition3[0], pImgui->m_fNaviPosition3[1], pImgui->m_fNaviPosition3[2]);
	
	RELEASE_INSTANCE(CImgui_Manager);

	m_vOriginal[0] = _float3(m_vPoints[0].x, m_vOriginal[0].y, m_vPoints[0].z);
	m_vOriginal[1] = _float3(m_vPoints[1].x, m_vOriginal[1].y, m_vPoints[1].z);
	m_vOriginal[2] = _float3(m_vPoints[2].x, m_vOriginal[2].y, m_vPoints[2].z);

	m_pVIBufferCom = CVIBuffer_Navigation::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBufferCom)
	{
		ERR_MSG(TEXT("Failed to Change Cell"));
		return;
	}
}

HRESULT CNaviPoint::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CNaviPoint::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_vPoints, pArg, sizeof(_float3) * 3);
	memcpy(&m_vOriginal, pArg, sizeof(_float3) * 3);

	for (_int i = 0; i < 3; ++i)
		m_vPoints[i].y += 0.05f;

	_vector vLineAB = XMVector3Normalize(XMLoadFloat3(&m_vPoints[1]) - XMLoadFloat3(&m_vPoints[0]));
	_vector vLineAC = XMVector3Normalize(XMLoadFloat3(&m_vPoints[2]) - XMLoadFloat3(&m_vPoints[0]));

	XMStoreFloat3(&m_vNormal, XMVector3Cross(vLineAB, vLineAC));
	
	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	return S_OK;
}

void CNaviPoint::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CImgui_Manager* pImgui = GET_INSTANCE(CImgui_Manager);

	if (pImgui->m_isSort)
		Sort_Points();
	

	RELEASE_INSTANCE(CImgui_Manager);
}

void CNaviPoint::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CNaviPoint::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	
	if (nullptr == m_pRendererCom || nullptr == m_pVIBufferCom)
		return E_FAIL;
	
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	
	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CNaviPoint::Ready_Components(void)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	m_pVIBufferCom = CVIBuffer_Navigation::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNaviPoint::SetUp_ShaderResources(void)
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	RELEASE_INSTANCE(CGameInstance);

	if (m_isChoice)
		m_pShaderCom->Set_RawValue("g_vColor", &_float4(1.f, 1.f, 0.f, 1.f), sizeof(_float4));
	else
	{
		if (m_vNormal.y < 0.f)
			m_pShaderCom->Set_RawValue("g_vColor", &_float4(1.f, 0.f, 0.f, 1.f), sizeof(_float4));

		else
			m_pShaderCom->Set_RawValue("g_vColor", &_float4(0.f, 1.f, 0.f, 1.f), sizeof(_float4));
	}
	
	return S_OK;
}

void CNaviPoint::Sort_Points(void)
{
	if (0.f > m_vNormal.y)
	{
		_float3 vTemp = m_vPoints[1];
		m_vPoints[1] = m_vPoints[2];
		m_vPoints[2] = vTemp;

		vTemp = m_vOriginal[1];
		m_vOriginal[1] = m_vOriginal[2];
		m_vOriginal[2] = vTemp;
	}

	_vector vLineAB = XMVector3Normalize(XMLoadFloat3(&m_vPoints[1]) - XMLoadFloat3(&m_vPoints[0]));
	_vector vLineAC = XMVector3Normalize(XMLoadFloat3(&m_vPoints[2]) - XMLoadFloat3(&m_vPoints[0]));
	
	XMStoreFloat3(&m_vNormal, XMVector3Cross(vLineAB, vLineAC));
}

CNaviPoint * CNaviPoint::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CNaviPoint* pInstance = new CNaviPoint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CNaviPoint"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CNaviPoint::Clone(void * pArg)
{
	CNaviPoint* pInstance = new CNaviPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CNaviPoint"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNaviPoint::Free(void)
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
