#include "stdafx.h"
#include "..\Public\Terrain.h"

#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void * pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CTerrain::Initialize_Load(const _tchar * VIBufferTag, void * pArg)
{

	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, VIBufferTag, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	switch (iLevel)
	{
	case Client::LEVEL_STATIC:
		break;
	case Client::LEVEL_LOADING:
		break;
	case Client::LEVEL_LOGO:
		break;
	case Client::LEVEL_SNOWFIELD:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_SNOWFIELD, TEXT("Prototype_Component_Texture_Terrain_SnowBattle"), (CComponent**)&m_pTextureCom[TYPE_DIFFUSE])))
			return E_FAIL;

		/* For.Com_Brush */
		if (FAILED(__super::Add_Components(TEXT("Com_Brush"), LEVEL_SNOWFIELD, TEXT("Prototype_Component_Texture_Brush"), (CComponent**)&m_pTextureCom[TYPE_BRUSH])))
			return E_FAIL;

		/* For.Com_Filter */
		if (FAILED(__super::Add_Components(TEXT("Com_Filter"), LEVEL_SNOWFIELD, TEXT("Prototype_Component_Texture_BattleZoneFilter"), (CComponent**)&m_pTextureCom[TYPE_FILTER])))
			return E_FAIL;

		/* For.Com_Navigation */
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;

		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_NormalTexture"), LEVEL_SNOWFIELD, TEXT("Prototype_Component_Texture_TerrainNormal_SnowBattle"), (CComponent**)&m_pNormaltexture[TYPE_DIFFUSE])))
			return E_FAIL;
		break;
	case Client::LEVEL_BATTLE:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_BATTLE, TEXT("Prototype_Component_Texture_Terrain_SnowBattle"), (CComponent**)&m_pTextureCom[TYPE_DIFFUSE])))
			return E_FAIL;

		/* For.Com_Brush */
		if (FAILED(__super::Add_Components(TEXT("Com_Brush"), LEVEL_BATTLE, TEXT("Prototype_Component_Texture_Brush"), (CComponent**)&m_pTextureCom[TYPE_BRUSH])))
			return E_FAIL;

		/* For.Com_Filter */
		if (FAILED(__super::Add_Components(TEXT("Com_Filter"), LEVEL_BATTLE, TEXT("Prototype_Component_Texture_BattleZoneFilter"), (CComponent**)&m_pTextureCom[TYPE_FILTER])))
			return E_FAIL;

		/* For.Com_Navigation */
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;

		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_NormalTexture"), LEVEL_BATTLE, TEXT("Prototype_Component_Texture_TerrainNormal_SnowBattle"), (CComponent**)&m_pNormaltexture[TYPE_DIFFUSE])))
			return E_FAIL;
		break;
	default:
		break;
	}

	


	return S_OK;
}

int CTerrain::Tick(_float fTimeDelta)
{

	return OBJ_NOEVENT;
}

void CTerrain::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return ;
	//m_pVIBufferCom->Culling(m_pTransformCom);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
#ifdef _DEBUG		
		m_pRendererCom->Add_Debug(m_pNavigationCom);
#endif
	}
}

HRESULT CTerrain::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;


	m_pShaderCom->Begin(0);
	
	m_pVIBufferCom->Render();


	return S_OK;
}


HRESULT CTerrain::Ready_Components(void *pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_BATTLE, TEXT("Prototype_Component_Texture_Terrain"), (CComponent**)&m_pTextureCom[TYPE_DIFFUSE])))
		return E_FAIL;

	/* For.Com_Brush */
	if (FAILED(__super::Add_Components(TEXT("Com_Brush"), LEVEL_BATTLE, TEXT("Prototype_Component_Texture_Brush"), (CComponent**)&m_pTextureCom[TYPE_BRUSH])))
		return E_FAIL;

	/* For.Com_Filter */
	if (FAILED(__super::Add_Components(TEXT("Com_Filter"), LEVEL_BATTLE, TEXT("Prototype_Component_Texture_BattleZoneFilter"), (CComponent**)&m_pTextureCom[TYPE_FILTER])))
		return E_FAIL;	

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Navigation */
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation"), (CComponent**)&m_pNavigationCom)))
		return E_FAIL;
	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_NormalTexture"), LEVEL_BATTLE, TEXT("Prototype_Component_Texture_TerrainNormal"), (CComponent**)&m_pNormaltexture[TYPE_DIFFUSE])))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	ID3D11ShaderResourceView*		pSRVs[] = {
		m_pTextureCom[TYPE_DIFFUSE]->Get_SRV(0), 
		m_pTextureCom[TYPE_DIFFUSE]->Get_SRV(1),
		m_pTextureCom[TYPE_DIFFUSE]->Get_SRV(2),
		m_pTextureCom[TYPE_DIFFUSE]->Get_SRV(3),
	};

	ID3D11ShaderResourceView*		pNormalSRVs[] = {
		m_pNormaltexture[TYPE_DIFFUSE]->Get_SRV(0),
		m_pNormaltexture[TYPE_DIFFUSE]->Get_SRV(1),
		m_pNormaltexture[TYPE_DIFFUSE]->Get_SRV(2),
		m_pNormaltexture[TYPE_DIFFUSE]->Get_SRV(3),
	};

	if (FAILED(m_pShaderCom->Set_ShaderResourceViewArray("g_DiffuseTexture", pSRVs, 4)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceViewArray("g_NormalTexture", pNormalSRVs, 4)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_BrushTexture", m_pTextureCom[TYPE_BRUSH]->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_FilterTexture", m_pTextureCom[TYPE_FILTER]->Get_SRV(0))))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_FilterTexture", m_pFilterTexture)))
		//return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Create_FilterTexture()
{
	ID3D11Texture2D*			pTexture2D = nullptr;

	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 128;
	TextureDesc.Height = 128;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	_uint*			pPixel = new _uint[TextureDesc.Width * 	TextureDesc.Height];

	for (_uint i = 0; i < TextureDesc.Height; ++i)
	{
		for (_uint j = 0; j < TextureDesc.Width; ++j)
		{
			_uint		iIndex = i * TextureDesc.Width + j;

			if(j < 64)
				pPixel[iIndex] = D3DCOLOR_ARGB(255, 255, 255, 255);
			else
				pPixel[iIndex] = D3DCOLOR_ARGB(255, 0, 0, 0);
		}
	}

	D3D11_SUBRESOURCE_DATA			SubResourceData;
	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	SubResourceData.pSysMem = pPixel;
	SubResourceData.SysMemPitch = 128 * 4;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &pTexture2D)))
		return E_FAIL;

	
	/*D3D11_MAP_WRITE_NO_OVERWRITE : 기존에 있던 값을 유지한상태로 메모리의 주소를 얻어오낟ㄷ. */
	/*D3D11_MAP_WRITE_DISCARD : 기존에 있던 값은 날리고 메모리 주소를 얻어오낟. */

	D3D11_MAPPED_SUBRESOURCE			SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	if (FAILED(m_pContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource)))
		return E_FAIL;

	//pPixel[0] = D3DCOLOR_ARGB(255, 255, 255, 0);

	memcpy(SubResource.pData, pPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	m_pContext->Unmap(pTexture2D, 0);

	if (FAILED(DirectX::SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../../../Bin/Resources/Textures/Terrain/Newfilter.dds"))))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateShaderResourceView(pTexture2D, nullptr, &m_pFilterTexture)))
		return E_FAIL;

	Safe_Delete_Array(pPixel);
	Safe_Release(pTexture2D);

	return S_OK;
}

CTerrain * CTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTerrain*	pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone_Load(const _tchar * VIBufferTag, void * pArg)
{
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize_Load(VIBufferTag, pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pFilterTexture);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);

	for (_uint i = 0; i < TYPE_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
		Safe_Release(m_pNormaltexture[i]);
	}
		

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
