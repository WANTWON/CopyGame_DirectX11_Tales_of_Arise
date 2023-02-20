#include "stdafx.h"
#include "..\Public\UI_LOCKON.h"

#include "GameInstance.h"
#include "Player.h"
#include "BattleManager.h"
#include "Monster.h"
#include "Effect.h"

CUI_LOCKON::CUI_LOCKON(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_LOCKON::CUI_LOCKON(const CUI_LOCKON & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_LOCKON::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_LOCKON::Initialize(void * pArg)
{


	//Set_Scale({ 0.35f, 0.35f, 1.f });
	m_fSize.x = 32.f;
	m_fSize.y = 32.f;
	m_fPosition.x = 600.f;
	m_fPosition.y = 600.f;
	///*m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f;
	//m_fPosition.y = g_iWinSizeY - m_fSize.y * 0.5f;

	m_fcurrentmp = 0.f;
	/*if (FAILED(__super::Initialize(pArg)))
	return E_FAIL;*/
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1.f, 0.f, 0.f, 1.f));

	//m_vRight = _float4(0.5f, 0.f, 0.f, 0.f);
	//m_vUp = _float4(0.f, 0.5f, 0.f, 0.f);
	/*pVertices->vLook = _float4(0.f, 0.f, 0.5f, 0.f);*/

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Set_LockOn(this);

	return S_OK;
}

int CUI_LOCKON::Tick(_float fTimeDelta)
{
	
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;

	if (m_bDead || CBattleManager::Get_Instance()->Get_LackonMonster() == nullptr)
	{
		CUI_Manager::Get_Instance()->Set_LockOn(nullptr);
		return OBJ_DEAD;
	}
		
	speed = 0.1f;
	//time = 0.f;
	time += fTimeDelta;


	if (time > 0.6f)
	{
		//scaler = 0.1f;
		time = 0.f;
		//return OBJ_DEAD;
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_5))
	{
		m_fcurrentmp += 5.f;
	}
	if (CGameInstance::Get_Instance()->Key_Up(DIK_6))
	{
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_ScreenFadeEffect"), LEVEL_BATTLE, TEXT("ETETET"))))
			return OBJ_NOEVENT;

		m_fcurrentmp -= 1.f;
	}
	//dynamic_cast<CPlayer*>(pGameObject)->Get_ProjPosition().x - 85.f;

	if (CBattleManager::Get_Instance()->Get_LackonMonster() == nullptr)
		return OBJ_NOEVENT;

	


	if(nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
	m_fcurrentmp = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_Stats().m_fLockonSmashGuage;

	if (m_fcurrentmp >= 4.f)
		m_bStrikeon = true;
	else
	{
		m_bRenderDiamond = true;
		m_bStrikefonton = false;
		m_bStrikeon = false;
		m_fAlphaDiamond = 1.f;
		m_fDiamondShooter = 1.f;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		m_fTimeOffset += 0.1f;
		if (m_fTimeOffset >= 1.f)
			m_fTimeOffset = 1.f;
		CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_60"), m_fTimeOffset);
		if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_Zoom(false);
		RELEASE_INSTANCE(CGameInstance);
	}
	

	if (m_bStrikeon)        // f
	{
		
		m_fGlowtimer += fTimeDelta*30.f;
		m_bStrikefonton = true;
		if(CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_Zoom(true);
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		m_fTimeOffset -= 0.1f;
		if (m_fTimeOffset <= 0.1f)
			m_fTimeOffset = 0.1f;		
		CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_60"), m_fTimeOffset);
		RELEASE_INSTANCE(CGameInstance);
		
	}

	if (m_bStrikefonton && m_bStrikeSmallonetime)    // first strike font smaller
	{
		m_bStrikeScaler -= 1.f;
		m_fLockonScaler -= 1.f;
		m_fDiamondShooter += 1.f;
		m_fAlphaDiamond -= 0.1f;
		if (m_bStrikeScaler <= 1.f)
		{
			m_fAlphaDiamond = 0.f;
			m_bStrikeScaler = 1.f;
			m_fLockonScaler = 1.f;
			m_bStrikefonton = false;
			m_bStrikeSmallonetime = false;
			m_bRenderDiamond = false;
		}


	}

	if (m_bStrikeon && !m_bStrikeSmallonetime) //jansang
	{
		m_bStrikeFakeScaler += 0.015f;
		if (m_bStrikeFakeScaler >= 1.2f)
			m_bStrikeFakeScaler = 1.f;

	}
	//m_pTransformCom->Turn({ 0.f,1.f,0.f,1.f }, 45.f);

	_float2 lockonPos = CBattleManager::Get_Instance()->Get_LackonMonster()->Get_ProjPosition();

	m_fPosition.x = lockonPos.x;//550.f;
	m_fPosition.y = lockonPos.y - 50.f; //600.f;
	m_fSize.x = 48.f * m_fDiamondShooter;
	m_fSize.y = 48.f * m_fDiamondShooter;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (m_bStrikeon && !m_bEffectSpawned)
	{
		CBaseObj* pObj = CBattleManager::Get_Instance()->Get_LackonMonster();
		if (pObj)
		{
			_float4 vPosition;
			XMStoreFloat4(&vPosition, pObj->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + XMVectorSet(0.f, 1.f, 0.f, 0.f));

			_matrix mWorldMatrix = XMMatrixTranslation(vPosition.x, vPosition.y, vPosition.z);

			CEffect::PlayEffectAtLocation(TEXT("Skill_Break_Shockwave_1.dat"), mWorldMatrix);

			m_bEffectSpawned = true;

			
		}
	}

	return OBJ_NOEVENT;
}

void CUI_LOCKON::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;

	if (CBattleManager::Get_Instance()->Get_LackonMonster() == nullptr)
		return;

	m_fcurrent_render_slot_mp = m_fcurrentmp - (_uint)m_fcurrentmp;

	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_GLOW, this);
}

HRESULT CUI_LOCKON::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;
	//0¹ø


	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_UVSpriteValue", &Sprite, sizeof(_float2))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_SpriteCurTime", &time, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_SpriteSpeed", &speed, sizeof(_float))))
		return E_FAIL;

//m_pShaderCo	m->Begin(0);
//
//	m_pVIBufferCom->Render();
	
	//m_fPosition.x -= 9.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_fmaxmp, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fcurrentmp, sizeof(_float))))
		return E_FAIL;
	_float2 garara = { 1.f,1.f };
	if (FAILED(m_pShaderCom->Set_RawValue("g_WinXY", &garara, sizeof(_float2))))
		return E_FAIL;
	
	//m_pTransformCom->Set_Rotation({ 0.f,0.f,45.f });
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	
	
	
	if (m_bRenderDiamond)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(2))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlphaDiamond, sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_POTRAIT_ALLBLUE);

		m_pVIBufferCom->Render();

		m_pShaderCom->Begin(UI_PROGRESSBAR);

		m_pVIBufferCom->Render();

		m_fGlowPosition[0] = m_fPosition;
		m_fGlowSize[0] = m_fSize;
	}
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlphaNomal, sizeof(_float))))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fcurrent_render_slot_mp, sizeof(_float))))
		return E_FAIL;*/

	if (!m_bStrikefonton)
	{
		m_fSize.x = 80.f;
		m_fSize.y = 80.f;
		//	m_fPosition.x -= 3.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		//	m_pTransformCom->Set_Rotation({ 0.f,0.f,0.f });
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pVIBufferCom->Render();
	}
	else
	{
		m_fSize.x = 80.f;
		m_fSize.y = 80.f;
		//	m_fPosition.x -= 3.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		//	m_pTransformCom->Set_Rotation({ 0.f,0.f,0.f });
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(8))))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pVIBufferCom->Render();

		m_fGlowPosition[1] = m_fPosition;
		m_fGlowSize[1] = m_fSize;
	}

	


	//m_fSize.x = 1500.f;
	//m_fSize.y = 1000.f;
	////	m_fPosition.x -= 3.f;

	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(9))))
	//	return E_FAIL;
	//m_pShaderCom->Begin(UI_STRIKEEFFECT);

	//m_pVIBufferCom->Render();

	if (m_bStrikeon)
	{
		/*m_fSize.x = 128.f * m_fLockonScaler;
		m_fSize.y = 128.f * m_fLockonScaler;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(7))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();*/


		m_fPosition.y += 50.f;
		m_fSize.x = 128.f * m_bStrikeScaler;
		m_fSize.y = 32.f * m_bStrikeScaler;
		//	m_fPosition.x -= 3.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(7))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_POTRAIT_ALLBLUE);

		m_pVIBufferCom->Render();

		
	}

	if (m_bStrikeon && !m_bStrikeSmallonetime) //jansang
	{
		m_fSize.x = 128.f * m_bStrikeFakeScaler;
		m_fSize.y = 32.f * m_bStrikeFakeScaler;
		//	m_fPosition.x -= 3.f;
		_float fake = m_fAlphaNomal *0.5f;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &fake, sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(7))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_POTRAIT_ALLBLUE);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_LOCKON::Render_Glow()
{
	if (!m_bStrikeon)
		return S_OK;

	if (!m_pShaderCom || !m_pVIBufferCom)
		return E_FAIL; 
	

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	/* Set Glow Shader Variables. */
	_float3 vColor = _float3(.25f, .67f, .78f);
	if (FAILED(m_pShaderCom->Set_RawValue("g_vGlowColor", &vColor, sizeof(_float3))))
		return E_FAIL;
	_bool bUseDiffuseColor = false;
	if (FAILED(m_pShaderCom->Set_RawValue("g_bUseDiffuseColor", &bUseDiffuseColor, sizeof(_bool))))
		return E_FAIL;

	/* Gauge Glow */

	// TODO: Position Change.
	// ..

	if (m_bRenderDiamond)
	{

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fGlowSize[0].x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fGlowSize[0].y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fGlowPosition[0].x - g_iWinSizeX * 0.5f, -m_fGlowPosition[0].y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GlowTexture", m_pTextureGlowCom->Get_SRV(0))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_GLOW);
		m_pVIBufferCom->Render();

	}

	/* Keys Glow */
	
	// TODO: Position Change.
	// ..

	if (m_bStrikefonton)
	{
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fGlowSize[1].x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fGlowSize[1].y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fGlowPosition[1].x - g_iWinSizeX * 0.5f, -m_fGlowPosition[1].y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GlowTexture", m_pTextureGlowCom->Get_SRV(1))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fGlowTimer", &m_fGlowtimer, sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_GLOW);
		m_pVIBufferCom->Render();
	}
	

	return S_OK;
}

HRESULT CUI_LOCKON::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lockon"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_TextureGlow */
	if (FAILED(__super::Add_Components(TEXT("Com_TextureGlow"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_LockOn_Glow"), (CComponent**)&m_pTextureGlowCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_LOCKON::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;





	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fcurrent_render_slot_mp, sizeof(_float))))
		return E_FAIL;



	

	return S_OK;
}

CUI_LOCKON * CUI_LOCKON::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_LOCKON*	pInstance = new CUI_LOCKON(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_LOCKON"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_LOCKON::Clone(void * pArg)
{
	CUI_LOCKON*	pInstance = new CUI_LOCKON(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_LOCKON"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_LOCKON::Free()
{
	//Safe_Release(m_pVIBufferCom1);
	__super::Free();
}
