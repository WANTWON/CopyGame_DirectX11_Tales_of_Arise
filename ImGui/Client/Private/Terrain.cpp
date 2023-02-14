#include "stdafx.h"
#include "..\Public\Terrain.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "PickingMgr.h"
#include "Imgui_Manager.h"
#include <Windows.h>

CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBaseObj(pDevice, pContext)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Create_FirstFilterTexture()))
		return E_FAIL;

	if (m_eDebugtype == DEBUG_SOILD)
	{
		CTerrain_Manager::Get_Instance()->Add_DebugTerrain(this);
	}

	CPickingMgr::Get_Instance()->Add_PickingGroup(this);

	m_eObjectID = OBJ_BACKGROUND;

	return S_OK;
}

HRESULT CTerrain::Initialize_Load(const _tchar * VIBufferTag, void * pArg)
{
	CTerrain_Manager::TERRAINDESC TerrainDesc;
	if (pArg != nullptr)
	{
		ZeroMemory(&TerrainDesc, sizeof(CTerrain_Manager::TERRAINDESC));
		memcpy(&TerrainDesc, pArg, sizeof(CTerrain_Manager::TERRAINDESC));
		m_eDebugtype = (TERRAIN_DEBUG_TYPE)TerrainDesc.m_eDebugTerrain;
	}

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"), (CComponent**)&m_pTextureCom[TYPE_DIFFUSE])))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, VIBufferTag, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fRotationPerSec = 1.f;
	TransformDesc.fSpeedPerSec = 3.f;

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Brush */
	if (FAILED(__super::Add_Components(TEXT("Com_Brush"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"), (CComponent**)&m_pTextureCom[TYPE_BRUSH])))
		return E_FAIL;

	_vector vInitPosition = XMVectorSet(TerrainDesc.TerrainDesc.m_iPositionX, TerrainDesc.TerrainDesc.m_fHeight, TerrainDesc.TerrainDesc.m_iPositionZ, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vInitPosition);

	if (FAILED(Create_FirstFilterTexture()))
		return E_FAIL;

	CPickingMgr::Get_Instance()->Add_PickingGroup(this);
	m_eObjectID = OBJ_BACKGROUND;

	return S_OK;
}

int CTerrain::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_eDebugtype == DEBUG_SOILD)
	{
		CTerrain_Manager::TERRAINDESC TerrainDesc = CTerrain_Manager::Get_Instance()->Get_TerrainDesc();
		_vector vPosition = XMVectorSet(TerrainDesc.TerrainDesc.m_iPositionX, TerrainDesc.TerrainDesc.m_fHeight, TerrainDesc.TerrainDesc.m_iPositionZ, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bDebugShow = CTerrain_Manager::Get_Instance()->Get_TerrainShow();
	}

	return OBJ_NOEVENT;
}

void CTerrain::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


	if (nullptr != m_pRendererCom && nullptr != m_pRendererCom && CImgui_Manager::Get_Instance()->Get_ShowOnlyNavi() == false)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	if (CImgui_Manager::Get_Instance()->Get_UpdateTerrain())
	{
		Load_FilterTexture(CImgui_Manager::Get_Instance()->Get_BmpPath());
		CImgui_Manager::Get_Instance()->Set_UpdateTerrain(false);
	}
		

}

HRESULT CTerrain::Render()
{
	if (m_eDebugtype == DEBUG_SOILD && !m_bDebugShow)
		return S_OK;

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	return S_OK;
}

_bool CTerrain::Picking(_float3 * PickingPoint)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//if (m_eDebugtype != DEBUG_SOILD)
	//{

	//	RELEASE_INSTANCE(CGameInstance);
	//	return false;
	//}
		

	if (m_pVIBufferCom->Picking(m_pTransformCom, PickingPoint) == true)
	{
		m_vMousePickPos = *PickingPoint;
		CTerrain_Manager::Get_Instance()->Set_PickingWorldPos(m_vMousePickPos);
		RELEASE_INSTANCE(CGameInstance);
		return true;
	}
	else
	{
		ZeroMemory(&m_vMousePickPos, sizeof(_float3));
	}
		

	RELEASE_INSTANCE(CGameInstance);

	return false;
}

void CTerrain::PickingTrue()
{
	CImgui_Manager::PICKING_TYPE ePickingtype = CImgui_Manager::Get_Instance()->Get_PickingType();

	switch (ePickingtype)
	{
	case Client::CImgui_Manager::PICKING_TERRAIN_TRANSFORM:
		Set_Picked();
		break;
	case Client::CImgui_Manager::PICKING_TERRAIN_SHAPE:
		if (m_eDebugtype != DEBUG_SOILD)
			Set_Terrain_Shape();
		break;
	case Client::CImgui_Manager::PICKING_TERRAIN_BRUSH:
		if (m_eDebugtype != DEBUG_SOILD)
			Create_FilterTexture(CImgui_Manager::Get_Instance()->Get_BmpPath());
		break;
	default:
		break;
	}
}

HRESULT CTerrain::Ready_Components(void* pArg)
{
	CTerrain_Manager::TERRAINDESC TerrainDesc;
	if (pArg != nullptr)
	{
		ZeroMemory(&TerrainDesc, sizeof(CTerrain_Manager::TERRAINDESC));
		memcpy(&TerrainDesc, pArg, sizeof(CTerrain_Manager::TERRAINDESC));
		m_eDebugtype = (TERRAIN_DEBUG_TYPE)TerrainDesc.m_eDebugTerrain;
	}
	

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"), (CComponent**)&m_pTextureCom[TYPE_DIFFUSE])))
		return E_FAIL;

	if (pArg != nullptr)
	{
		/* For.Com_VIBuffer */
		if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"), (CComponent**)&m_pVIBufferCom, &TerrainDesc.TerrainDesc)))
			return E_FAIL;
	}
	else
	{
		/* For.Com_VIBuffer */
		if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_Height"), (CComponent**)&m_pVIBufferCom)))
			return E_FAIL;
	}
	

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fRotationPerSec = 1.f;
	TransformDesc.fSpeedPerSec = 3.f;

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	_vector vInitPosition = XMVectorSet(TerrainDesc.TerrainDesc.m_iPositionX, TerrainDesc.TerrainDesc.m_fHeight, TerrainDesc.TerrainDesc.m_iPositionZ, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vInitPosition);


	/* For.Com_Brush */
	if (FAILED(__super::Add_Components(TEXT("Com_Brush"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"), (CComponent**)&m_pTextureCom[TYPE_BRUSH])))
		return E_FAIL;

	///* For.Com_Filter */
	//if (FAILED(__super::Add_Components(TEXT("Com_Filter"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Filter"), (CComponent**)&m_pTextureCom[TYPE_FILTER])))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::SetUp_ShaderID()
{
	m_bWireFrame = CTerrain_Manager::Get_Instance()->Get_TerrainDesc().m_bShowWireFrame;

	switch (m_eDebugtype)
	{
	case Client::CTerrain::DEBUG_SOILD:
		if (m_bWireFrame)
			m_eShaderID = WIRE;
		else
			m_eShaderID = DEFAULT;
		break;
	case Client::CTerrain::DEBUG_WIRE:
		m_eShaderID = WIRE;
		break;
	case Client::CTerrain::DEBUG_NONE:
		if (m_bWireFrame)
			m_eShaderID = WIRE;
		else if (m_bPicked)
			m_eShaderID = PICKED;
		else
			m_eShaderID = DEFAULT;
		break;
	default:
		break;
	}
	
	return S_OK;
}

HRESULT CTerrain::SetUp_ShaderResources()
{

	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	ID3D11ShaderResourceView*		pSRVs[] = {
		m_pTextureCom[TYPE_DIFFUSE]->Get_SRV(0),
		m_pTextureCom[TYPE_DIFFUSE]->Get_SRV(1),
		m_pTextureCom[TYPE_DIFFUSE]->Get_SRV(2),
		m_pTextureCom[TYPE_DIFFUSE]->Get_SRV(3)
	};

	if (FAILED(m_pShaderCom->Set_ShaderResourceViewArray("g_DiffuseTexture", pSRVs, 4)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_BrushTexture", m_pTextureCom[TYPE_BRUSH]->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vBrushPos", &m_vMousePickPos, sizeof(_float4))))
		return E_FAIL;

	_float fBrushRadius = CTerrain_Manager::Get_Instance()->Get_TerrainShapeDesc().fRadius;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBrushRange", &fBrushRadius, sizeof(_float))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_FilterTexture", m_pFilterTexture)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CTerrain::Set_Terrain_Shape()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_0))
	{
		CPickingMgr::Get_Instance()->Set_PickedObj(nullptr);

		CTerrain_Manager::TERRAINSHAPEDESC TerrainShapeDesc = CTerrain_Manager::Get_Instance()->Get_TerrainShapeDesc();

		_float fHegith = TerrainShapeDesc.fHeight;
		_float fRad = TerrainShapeDesc.fRadius;
		_float fSharp = TerrainShapeDesc.fSharp;

		m_pVIBufferCom->Set_Terrain_Shape(fHegith, fRad, fSharp, m_vMousePickPos, 1.f);
	}
	
	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_MINUS))
	{
		CPickingMgr::Get_Instance()->Set_PickedObj(nullptr);

		CTerrain_Manager::TERRAINSHAPEDESC TerrainShapeDesc = CTerrain_Manager::Get_Instance()->Get_TerrainShapeDesc();

		_float fHegith = TerrainShapeDesc.fHeight;
		_float fRad = TerrainShapeDesc.fRadius;
		_float fSharp = TerrainShapeDesc.fSharp;

		m_pVIBufferCom->Set_Terrain_UpDown(fHegith, fRad, fSharp, m_vMousePickPos, 1.f);
	}

	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_EQUALS))
	{
		CPickingMgr::Get_Instance()->Set_PickedObj(nullptr);

		CTerrain_Manager::TERRAINSHAPEDESC TerrainShapeDesc = CTerrain_Manager::Get_Instance()->Get_TerrainShapeDesc();

		_float fHegith = TerrainShapeDesc.fHeight;
		_float fRad = TerrainShapeDesc.fRadius;
		_float fSharp = TerrainShapeDesc.fSharp;

		m_pVIBufferCom->Set_Terrain_UpDown(-fHegith, fRad, fSharp, m_vMousePickPos, 1.f);
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CTerrain::Set_Picked()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Mouse_Down(DIMK::DIMK_LBUTTON))
	{
		if (m_bPicked)
			CPickingMgr::Get_Instance()->Set_PickedObj(nullptr);
		else
			CPickingMgr::Get_Instance()->Set_PickedObj(this);
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CTerrain::Save_Terrain(HANDLE hFile, _ulong* dwByte)
{
	m_pVIBufferCom->Save_VertexPosition(hFile, dwByte);
}

HRESULT CTerrain::Create_FilterTexture(_tchar* bmpPath)
{

	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_9))
	{
		ID3D11Texture2D*					pTexture2D = nullptr;
		D3D11_TEXTURE2D_DESC		TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		TextureDesc.Width = 256;
		TextureDesc.Height = 256;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.SampleDesc.Count = 1;

		TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
		TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		TextureDesc.MiscFlags = 0;

		_float3 vMousePos = m_vMousePickPos;
		_float fRange = CTerrain_Manager::Get_Instance()->Get_TerrainShapeDesc().fRadius;
		_float fStrength = CTerrain_Manager::Get_Instance()->Get_TerrainShapeDesc().fSharp;

	//	_int iNumTerrainX = m_pVIBufferCom->Get_TerrainDesc().m_iVerticeNumX;
	//	_int iNumTerrainZ = m_pVIBufferCom->Get_TerrainDesc().m_iVerticeNumZ;

		_float3 ConevertPos = m_vMousePickPos;
		ConevertPos.y = 0.f; //_float3(m_vMousePickPos.x * 256 / iNumTerrainX, 0.f, m_vMousePickPos.z * 256 / iNumTerrainZ);
		_uint iConvertRange = (_uint)fRange;// (fRange * 256) / iNumTerrainX;

		_int StartX = (_int)max( ConevertPos.x - iConvertRange, 0);
		
		_int StartY = (_int)max(ConevertPos.z - iConvertRange, 0);
		
		_int EndX = (_int)min( ConevertPos.x + iConvertRange, 256);
		_int EndY = (_int)min( ConevertPos.z + iConvertRange ,256);

		for (_int i = StartY; i < EndY; ++i)
		{
			for (_int j = StartX; j < EndX; ++j)
			{
				_int		iIndex = i * TextureDesc.Width + j;

				_float3 vPixelPos = _float3(j, 0.f, i);
				_vector fDis = XMLoadFloat3(&ConevertPos) - XMLoadFloat3(&vPixelPos);
				_float fLen = XMVectorGetX(XMVector3Length(fDis));
				_float fAlpha =  (fLen / _float(iConvertRange));

				if (fAlpha < 0.7f)
					int a = 0;

				if (fAlpha > 1.f)
					fAlpha = 1.f;
				_uint iPixelValue = _uint((1 - fAlpha) * fStrength);

				_float3 vCurrentPixelColor = pPixelColor[iIndex];
				if (vCurrentPixelColor.x != 0.f)
					int a = 0;
				_float3 pNewPixelColor = _float3(0.f,0.f,0.f);

				_int iBrushType = CImgui_Manager::Get_Instance()->Get_BrushType();
				switch (iBrushType)
				{
				case CImgui_Manager::RED:
					pNewPixelColor = _float3((_float)iPixelValue, 0.f, 0.f);
					break;
				case CImgui_Manager::GREEN:
					pNewPixelColor = _float3( 0.f, (_float)iPixelValue, 0.f);
					break;
				case CImgui_Manager::BLUE:
					pNewPixelColor = _float3( 0.f, 0.f, (_float)iPixelValue);
					break;
				}
				
				_float3 fBlendColor = _float3(
					vCurrentPixelColor.x + pNewPixelColor.x,
					vCurrentPixelColor.y + pNewPixelColor.y,
					vCurrentPixelColor.z + pNewPixelColor.z);

				if (fBlendColor.x >= 255)
					fBlendColor.x = 255;
				if (fBlendColor.y >= 255)
					fBlendColor.y = 255;
				if (fBlendColor.z >= 255)
					fBlendColor.z = 255;
				
				if (fBlendColor.x == 0.f && fBlendColor.y == 0.f&& fBlendColor.z == 0.f)
					continue;

				pPixelColor[iIndex] = fBlendColor;
				pPixel[iIndex] = D3DCOLOR_ARGB(0, (_uint)fBlendColor.x, (_uint)fBlendColor.y, (_uint)fBlendColor.z);
			}
		}

		D3D11_SUBRESOURCE_DATA			SubResourceData;
		ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

		SubResourceData.pSysMem = pPixel;
		SubResourceData.SysMemPitch = 256 * 4;

		if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &pTexture2D)))
			goto FAIL;

		/*D3D11_MAP_WRITE_NO_OVERWRITE : 기존에 있던 값을 유지한상태로 메모리의 주소를 얻어오낟ㄷ. */
		/*D3D11_MAP_WRITE_DISCARD : 기존에 있던 값은 날리고 메모리 주소를 얻어오낟. */

		D3D11_MAPPED_SUBRESOURCE			SubResource;
		ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		if (FAILED(m_pContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource)))
			goto FAIL;

		memcpy(SubResource.pData, pPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

		//SetFileAttributes(TEXT("../../../Bin/Resources/Textures/Terrain/Newfilter.bmp"), FILE_ATTRIBUTE_NORMAL);
		HANDLE hFile = CreateFile(bmpPath, GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			int a = 0;//goto FAIL;

		BITMAPFILEHEADER bmfh;
		bmfh.bfType = 0x4d42; // "BM"
		bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (256 * 256 * 4);
		bmfh.bfReserved1 = 0;
		bmfh.bfReserved2 = 0;
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		DWORD dwBytesWritten;
		WriteFile(hFile, &bmfh, sizeof(bmfh), &dwBytesWritten, NULL);

		BITMAPINFOHEADER bmih;
		bmih.biSize = sizeof(bmih);
		bmih.biWidth = 256;
		bmih.biHeight = 256;
		bmih.biPlanes = 1;
		bmih.biBitCount = 32; //비트 수
		bmih.biCompression = BI_RGB; //압축타입
		bmih.biSizeImage = 0;
		bmih.biXPelsPerMeter = 0;
		bmih.biYPelsPerMeter = 0;
		bmih.biClrUsed = 0;
		bmih.biClrImportant = 0;
		WriteFile(hFile, &bmih, sizeof(bmih), &dwBytesWritten, NULL);

		WriteFile(hFile, pPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height, &dwBytesWritten, NULL);

		CloseHandle(hFile);

		m_pContext->Unmap(pTexture2D, 0);

	

		if (m_pFilterTexture != nullptr)
		{
			Safe_Release(m_pFilterTexture);
		}
		if (FAILED(m_pDevice->CreateShaderResourceView(pTexture2D, nullptr, &m_pFilterTexture)))
			goto FAIL;
		
		Safe_Release(pTexture2D);
		return S_OK;

	FAIL:
		Safe_Release(pTexture2D);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTerrain::Load_FilterTexture(_tchar * bmpPath)
{
	ID3D11Texture2D*					pTexture2D = nullptr;
	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 256;
	TextureDesc.Height = 256;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	HANDLE hFile = CreateFile(bmpPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		int a = 0;//goto FAIL;

	BITMAPFILEHEADER bmfh;
	DWORD dwBytesRead;
	ReadFile(hFile, &bmfh, sizeof(bmfh), &dwBytesRead, NULL);

	BITMAPINFOHEADER bmih;
	ReadFile(hFile, &bmih, sizeof(bmih), &dwBytesRead, NULL);
	ReadFile(hFile, pPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height, &dwBytesRead, NULL);

	D3D11_SUBRESOURCE_DATA			SubResourceData;
	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	SubResourceData.pSysMem = pPixel;
	SubResourceData.SysMemPitch = 256 * 4;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &pTexture2D)))
		goto FAIL;

	/*D3D11_MAP_WRITE_NO_OVERWRITE : 기존에 있던 값을 유지한상태로 메모리의 주소를 얻어오낟ㄷ. */
	/*D3D11_MAP_WRITE_DISCARD : 기존에 있던 값은 날리고 메모리 주소를 얻어오낟. */

	D3D11_MAPPED_SUBRESOURCE			SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	if (FAILED(m_pContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource)))
		goto FAIL;

	memcpy(SubResource.pData, pPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	CloseHandle(hFile);


	for (_uint i = 0; i < TextureDesc.Height; ++i)
	{
		for (_uint j = 0; j < TextureDesc.Width; ++j)
		{
			_uint		iIndex = i * TextureDesc.Width + j;
			_uint red = pPixel[iIndex] & 0x00FF0000;
			red = red >> 16;
			_uint green = pPixel[iIndex] & 0x0000FF00;
			green = green >> 8;
			_uint blue = pPixel[iIndex] & 0x000000FF;
			blue = blue >> 0;
			pPixelColor[iIndex] = _float3(red, green, blue);
		}
	}

	m_pContext->Unmap(pTexture2D, 0);


	if (m_pFilterTexture != nullptr)
	{
		Safe_Release(m_pFilterTexture);
	}
	if (FAILED(m_pDevice->CreateShaderResourceView(pTexture2D, nullptr, &m_pFilterTexture)))
		goto FAIL;

	Safe_Release(pTexture2D);
	return S_OK;

FAIL:
	Safe_Release(pTexture2D);
	return E_FAIL;
}

HRESULT CTerrain::Create_FirstFilterTexture()
{
	if (!m_bFirst)
	{
		m_bFirst = true;
		ID3D11Texture2D*					pTexture2D = nullptr;
		D3D11_TEXTURE2D_DESC		TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		TextureDesc.Width = 256;
		TextureDesc.Height = 256;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.SampleDesc.Count = 1;

		TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
		TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		TextureDesc.MiscFlags = 0;

		pPixel = new _uint[TextureDesc.Width * 	TextureDesc.Height];
		pPixelColor = new _float3[TextureDesc.Width * TextureDesc.Height];
		

		for (_uint i = 0; i < TextureDesc.Height; ++i)
		{
			for (_uint j = 0; j < TextureDesc.Width; ++j)
			{
				_uint		iIndex = i * TextureDesc.Width + j;
				pPixelColor[iIndex] = _float3(0, 0, 0 );
				pPixel[iIndex] = D3DCOLOR_ARGB(255, (_uint)pPixelColor[iIndex].x, (_uint)pPixelColor[iIndex].y, (_uint)pPixelColor[iIndex].z);
			}
		}

		D3D11_SUBRESOURCE_DATA			SubResourceData;
		ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

		SubResourceData.pSysMem = pPixel;
		SubResourceData.SysMemPitch = 256 * 4;

		if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &pTexture2D)))
			goto FAIL;

		D3D11_MAPPED_SUBRESOURCE			SubResource;
		ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		if (FAILED(m_pContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource)))
			goto FAIL;

		memcpy(SubResource.pData, pPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);


	//	SetFileAttributes(TEXT("../../../Bin/Resources/Textures/Terrain/Newfilter.bmp"), FILE_ATTRIBUTE_NORMAL);
		HANDLE hFile = CreateFile(TEXT("../../../Bin/Resources/Textures/Terrain/Newfilter.bmp"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		//if (hFile == INVALID_HANDLE_VALUE)
		//	goto FAIL;

		BITMAPFILEHEADER bmfh;
		bmfh.bfType = 0x4d42; // "BM"
		bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (256 * 256 * 4);
		bmfh.bfReserved1 = 0;
		bmfh.bfReserved2 = 0;
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		DWORD dwBytesWritten;
		WriteFile(hFile, &bmfh, sizeof(bmfh), &dwBytesWritten, NULL);

		BITMAPINFOHEADER bmih;
		bmih.biSize = sizeof(bmih);
		bmih.biWidth = 256;
		bmih.biHeight = 256;
		bmih.biPlanes = 1;
		bmih.biBitCount = 32;
		bmih.biCompression = BI_RGB;
		bmih.biSizeImage = 0;
		bmih.biXPelsPerMeter = 0;
		bmih.biYPelsPerMeter = 0;
		bmih.biClrUsed = 0;
		bmih.biClrImportant = 0;
		WriteFile(hFile, &bmih, sizeof(bmih), &dwBytesWritten, NULL);

		WriteFile(hFile, pPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height, &dwBytesWritten, NULL);

		CloseHandle(hFile);



		m_pContext->Unmap(pTexture2D, 0);



		//if (FAILED(DirectX::SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../../../Bin/Resources/Textures/Terrain/Newfilter.dds"))))
		//	goto FAIL;

		if (FAILED(m_pDevice->CreateShaderResourceView(pTexture2D, nullptr, &m_pFilterTexture)))
			goto FAIL;

		//	Safe_Delete_Array(pPixel);
		Safe_Release(pTexture2D);
		return S_OK;

	FAIL:
		Safe_Release(pTexture2D);
		return S_OK;
	}


	return S_OK;
}


CTerrain * CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain*	pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone(void* pArg)
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

	Safe_Delete_Array(pPixel);
	Safe_Delete_Array(pPixelColor);

	Safe_Release(m_pFilterTexture);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

	for (_uint i = 0; i < TYPE_END; ++i)
		Safe_Release(m_pTextureCom[i]);
}
