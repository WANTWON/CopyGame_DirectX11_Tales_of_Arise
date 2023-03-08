#include "stdafx.h"
#include "..\Public\UI_BossMonster_HPbar.h"

#include "GameInstance.h"

#include "Monster.h"

CUI_BossMonster_HPbar::CUI_BossMonster_HPbar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_BossMonster_HPbar::CUI_BossMonster_HPbar(const CUI_BossMonster_HPbar & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_BossMonster_HPbar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_BossMonster_HPbar::Initialize(void * pArg)
{
	

	m_fSize.x = 200.0f;
	m_fSize.y = 22.0f;
	m_fPosition.x = 100.f;
	m_fPosition.y = 100.f;
	m_fNext = 13.f;


	m_eShaderID = UI_HPBAR;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_BossMonster_HPbar::Tick(_float fTimeDelta)
{
	/*if (CBattleManager::Get_Instance()->Get_LackonMonster() == nullptr)
		return OBJ_NOEVENT;*/
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return OBJ_NOEVENT;
	if (CUI_Manager::Get_Instance()->Get_StopTick() || CBattleManager::Get_Instance()->Get_IsStrike())
		return OBJ_NOEVENT;

	m_bChangetoboss = CBattleManager::Get_Instance()->Get_Rinwellboss();


	if (CBattleManager::Get_Instance()->Get_BossMonster() == nullptr)
		return OBJ_DEAD;
	else
	{
		m_iMonstername = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_BossMonster())->Get_MonsterID();


		m_fcurrenthp = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_BossMonster())->Get_Stats().m_fCurrentHp;
		m_fmaxhp = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_BossMonster())->Get_Stats().m_fMaxHp;
	}

	/*m_iMonstername = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_BossMonster())->Get_MonsterID();


	m_fcurrenthp = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_BossMonster())->Get_Stats().m_fCurrentHp;
	m_fmaxhp = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_BossMonster())->Get_Stats().m_fMaxHp;*/

	if (m_bChangetoboss && CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_LAWBATTLE)
	{
		CMonster* pObject = dynamic_cast<CMonster*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_LAWBATTLE, TEXT("Layer_Rinwell")));
		m_fcurrenthp1 = pObject->Get_Stats().m_fCurrentHp;
		m_fmaxhp1 = pObject->Get_Stats().m_fMaxHp;
		m_iMonstername1 = pObject->Get_MonsterID();
		m_fYOffset = 25.f;
	}
	

	
	//m_iMonstername = 0;
	for (_uint i = 0; i < 6; ++i)
		m_fbrightpos_hp[i] += 0.015f;

	for (_uint i = 0; i < 6; ++i)
	{
		if (m_fbrightpos_hp[i] >= 3.f)
			m_fbrightpos_hp[i] = 0.f;
	}

	/*if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
	{
		m_fcurrenthp = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_Stats().m_fCurrentHp;
		m_fmaxhp = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_Stats().m_fMaxHp;

	}*/

	



	/*if (CGameInstance::Get_Instance()->Key_Pressing(DIK_J))
	{
		m_fcurrenthp += 100.f;
	}
	else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_K))
	{
		m_fcurrenthp -= 100.f;
	}*/


	if (m_fmaxhp * 0.75f < m_fcurrenthp)
		m_iHPbarindex = 4;
	else if (m_fmaxhp * 0.5f < m_fcurrenthp)
		m_iHPbarindex = 3;
	else if (m_fmaxhp * 0.25f < m_fcurrenthp)
		m_iHPbarindex = 2;
	else
		m_iHPbarindex = 1;

	if (m_fmaxhp1 * 0.75f < m_fcurrenthp1)
		m_iHPbarindex1 = 4;
	else if (m_fmaxhp1 * 0.5f < m_fcurrenthp1)
		m_iHPbarindex1 = 3;
	else if (m_fmaxhp1 * 0.25f < m_fcurrenthp1)
		m_iHPbarindex1 = 2;
	else
		m_iHPbarindex1 = 1;







	

	
	calculateboss1();
	calculateboss2();





	



	

	return OBJ_NOEVENT;
}

void CUI_BossMonster_HPbar::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return ;
	if (CUI_Manager::Get_Instance()->Get_StopTick() || CBattleManager::Get_Instance()->Get_IsStrike())
		return ;

	m_fHPbarvalue[3] = m_fmaxhp * 0.75f;
	m_fHPbarvalue[2] = m_fmaxhp * 0.5f;
	m_fHPbarvalue[1] = m_fmaxhp * 0.33f;
	m_fHPbarvalue[0] = m_fmaxhp * 0.25f;


	m_fHPcurbarvalue[3] = m_fcurrenthp;
	m_fHPcurbarvalue[2] = m_fcurrenthp * 0.5f;
	m_fHPcurbarvalue[1] = m_fcurrenthp * 0.33f;
	m_fHPcurbarvalue[0] = m_fcurrenthp * 0.25f;


	__super::Late_Tick(fTimeDelta);

}

HRESULT CUI_BossMonster_HPbar::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;


	

	m_fSize.x = 220.0f;
	m_fSize.y = 22.0f;
	m_fPosition.x = 400.f;
	m_fPosition.y = 40.f  - m_fYOffset;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	
	
	_float maxbar = m_fmaxhp * 0.25f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &maxbar, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fCurrentbar, sizeof(_float))))
		return E_FAIL;

	
	
	




	/*if (dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
	{*/



		m_fSize.x = 770.f;
		m_fSize.y = 30.0f;
		m_fPosition.x = 650.f;
		m_fPosition.y = 60.f - m_fYOffset;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(2))))
			return E_FAIL;
	
		m_pShaderCom->Begin(UI_BOSSHPBACKBLACK);

		m_pVIBufferCom->Render();
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(3))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BOSSHPBACKWHITE);

		m_pVIBufferCom->Render();

	//	m_eShaderID = UI_HPBAR;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
				return E_FAIL;
		m_fSize.x = 190.f;
		m_fSize.y = 22.0f;
		m_fPosition.x = 380.f;
		m_fPosition.y = 60.f - m_fYOffset;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		switch (m_iHPbarindex)
		{
		case 1:
			m_eShaderID = UI_BOSSHP;
			break;
		default:
			m_eShaderID = UI_BOSSHPFULL;
		
		}

		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();

		
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[1], sizeof(_float))))
			return E_FAIL;
		m_fPosition.x += 180.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		switch (m_iHPbarindex)
		{
		case 1:
			m_eShaderID = 0;
			break;

		case 2:
			m_eShaderID = UI_BOSSHP;
			m_pShaderCom->Begin(m_eShaderID);
			m_pVIBufferCom->Render();
			break;

		default:
			m_eShaderID = UI_BOSSHPFULL;
			m_pShaderCom->Begin(m_eShaderID);
			m_pVIBufferCom->Render();
		
		}
		
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[2], sizeof(_float))))
			return E_FAIL;
		m_fPosition.x += 180.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		switch (m_iHPbarindex)
		{
		case 1:
			m_eShaderID = 0;
			break;

		case 2:
			m_eShaderID = 0;
			break;

		case 3:
			m_eShaderID = UI_BOSSHP;
			m_pShaderCom->Begin(m_eShaderID);
			m_pVIBufferCom->Render();
			break;

		default:
			m_eShaderID = UI_BOSSHPFULL;
			m_pShaderCom->Begin(m_eShaderID);
			m_pVIBufferCom->Render();
		}
		
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[3], sizeof(_float))))
			return E_FAIL;
		m_fPosition.x += 180.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		switch (m_iHPbarindex)
		{
		case 4:
			m_eShaderID = UI_BOSSHP;
			m_pShaderCom->Begin(m_eShaderID);
			m_pVIBufferCom->Render();
			break;
		default:
			m_eShaderID = 0;
		}
		


		
		m_eShaderID = UI_BRIGHT;
		m_fSize.x = 20.0f;
		m_fSize.y = 20.0f;
		m_fPosition.x = 850.f;
		m_fPosition.y = 80.f - m_fYOffset;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

		RenderCurrenthpfont();


		m_fPosition.x += m_fNext;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(10))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();

		//return S_OK;

		RenderMaxhpfont();


		///////////////////////////////////////////
	

	

	/*if (CBattleManager::Get_Instance()->Get_LackonMonster() == nullptr)
		return S_OK;
	

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();*/

		if (m_bChangetoboss)
		{
			m_fYOffset = -20.f;

			m_fSize.x = 220.0f;
			m_fSize.y = 22.0f;
			m_fPosition.x = 400.f;
			m_fPosition.y = 40.f - m_fYOffset;
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(SetUp_ShaderResources()))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(m_iMonstername1))))
				return E_FAIL;
			m_pShaderCom->Begin(UI_BRIGHT);

			m_pVIBufferCom->Render();



			maxbar = m_fmaxhp1 * 0.25f;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &maxbar, sizeof(_float))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fCurrentbar1, sizeof(_float))))
				return E_FAIL;








			/*if (dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
			{*/



			m_fSize.x = 770.f;
			m_fSize.y = 30.0f;
			m_fPosition.x = 650.f;
			m_fPosition.y = 60.f - m_fYOffset;
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(2))))
				return E_FAIL;

			m_pShaderCom->Begin(UI_BOSSHPBACKBLACK);

			m_pVIBufferCom->Render();
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(3))))
				return E_FAIL;
			m_pShaderCom->Begin(UI_BOSSHPBACKWHITE);

			m_pVIBufferCom->Render();

			//	m_eShaderID = UI_HPBAR;

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
				return E_FAIL;
			m_fSize.x = 190.f;
			m_fSize.y = 22.0f;
			m_fPosition.x = 380.f;
			m_fPosition.y = 60.f - m_fYOffset;
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			switch (m_iHPbarindex1)
			{
			case 1:
				m_eShaderID = UI_BOSSHP;
				break;
			default:
				m_eShaderID = UI_BOSSHPFULL;

			}

			m_pShaderCom->Begin(m_eShaderID);
			m_pVIBufferCom->Render();


			if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[1], sizeof(_float))))
				return E_FAIL;
			m_fPosition.x += 180.f;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			switch (m_iHPbarindex1)
			{
			case 1:
				m_eShaderID = 0;
				break;

			case 2:
				m_eShaderID = UI_BOSSHP;
				m_pShaderCom->Begin(m_eShaderID);
				m_pVIBufferCom->Render();
				break;

			default:
				m_eShaderID = UI_BOSSHPFULL;
				m_pShaderCom->Begin(m_eShaderID);
				m_pVIBufferCom->Render();

			}

			if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[2], sizeof(_float))))
				return E_FAIL;
			m_fPosition.x += 180.f;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			switch (m_iHPbarindex1)
			{
			case 1:
				m_eShaderID = 0;
				break;

			case 2:
				m_eShaderID = 0;
				break;

			case 3:
				m_eShaderID = UI_BOSSHP;
				m_pShaderCom->Begin(m_eShaderID);
				m_pVIBufferCom->Render();
				break;

			default:
				m_eShaderID = UI_BOSSHPFULL;
				m_pShaderCom->Begin(m_eShaderID);
				m_pVIBufferCom->Render();
			}

			if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[3], sizeof(_float))))
				return E_FAIL;
			m_fPosition.x += 180.f;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			switch (m_iHPbarindex1)
			{
			case 4:
				m_eShaderID = UI_BOSSHP;
				m_pShaderCom->Begin(m_eShaderID);
				m_pVIBufferCom->Render();
				break;
			default:
				m_eShaderID = 0;
			}




			m_eShaderID = UI_BRIGHT;
			m_fSize.x = 20.0f;
			m_fSize.y = 20.0f;
			m_fPosition.x = 850.f;
			m_fPosition.y = 80.f - m_fYOffset;
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

			RenderCurrenthpfont1();


			m_fPosition.x += m_fNext;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
				return E_FAIL;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(10))))
				return E_FAIL;
			m_pShaderCom->Begin(m_eShaderID);

			m_pVIBufferCom->Render();

			//return S_OK;

			RenderMaxhpfont1();
		}
	
	return S_OK;

}

HRESULT CUI_BossMonster_HPbar::RenderCurrenthpfont()
{
	if (m_fcurrenthp >= 100000.f)
	{
		m_itexnum = (_uint)(m_fcurrenthp / 100000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}
	m_fPosition.x += m_fNext;
	if (m_fcurrenthp >= 10000.f)
	{
		m_itexnum = (((_uint)m_fcurrenthp % 100000) / 10000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[1], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fcurrenthp >= 1000.f)
	{
		m_itexnum = (((_uint)m_fcurrenthp % 10000) / 1000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[2], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fcurrenthp >= 100.f)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[3], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (((_uint)m_fcurrenthp % 1000) / 100);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}
	m_fPosition.x += m_fNext;
	if (m_fcurrenthp >= 10.f)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[4], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (((_uint)m_fcurrenthp % 100) / 10);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fcurrenthp >= 0)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[5], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (_uint)m_fcurrenthp % 10;


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_BossMonster_HPbar::RenderMaxhpfont()
{
	m_fPosition.x += m_fNext;
	if (m_fmaxhp >= 100000.f)
	{
		m_itexnum = (_uint)(m_fmaxhp / 100000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}
	m_fPosition.x += m_fNext;
	if (m_fmaxhp >= 10000.f)
	{
		m_itexnum = (((_uint)m_fmaxhp % 100000) / 10000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[1], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fmaxhp >= 1000.f)
	{
		m_itexnum = (((_uint)m_fmaxhp % 10000) / 1000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[2], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fmaxhp >= 100.f)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[3], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (((_uint)m_fmaxhp % 1000) / 100);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}
	m_fPosition.x += m_fNext;
	if (m_fmaxhp >= 10.f)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[4], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (((_uint)m_fmaxhp % 100) / 10);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}


	if (m_fmaxhp >= 0)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[5], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (_uint)m_fmaxhp % 10;

		m_fPosition.x += m_fNext;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_BossMonster_HPbar::RenderCurrenthpfont1()
{
	if (m_fcurrenthp1 >= 100000.f)
	{
		m_itexnum = (_uint)(m_fcurrenthp1 / 100000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}
	m_fPosition.x += m_fNext;
	if (m_fcurrenthp1 >= 10000.f)
	{
		m_itexnum = (((_uint)m_fcurrenthp1 % 100000) / 10000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[1], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fcurrenthp1 >= 1000.f)
	{
		m_itexnum = (((_uint)m_fcurrenthp1 % 10000) / 1000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[2], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fcurrenthp1 >= 100.f)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[3], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (((_uint)m_fcurrenthp1 % 1000) / 100);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}
	m_fPosition.x += m_fNext;
	if (m_fcurrenthp1 >= 10.f)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[4], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (((_uint)m_fcurrenthp1 % 100) / 10);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[5], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (_uint)m_fcurrenthp1 % 10;


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	
	return S_OK;
}

HRESULT CUI_BossMonster_HPbar::RenderMaxhpfont1()
{
	m_fPosition.x += m_fNext;
	if (m_fmaxhp1 >= 100000.f)
	{
		m_itexnum = (_uint)(m_fmaxhp1 / 100000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}
	m_fPosition.x += m_fNext;
	if (m_fmaxhp1 >= 10000.f)
	{
		m_itexnum = (((_uint)m_fmaxhp1 % 100000) / 10000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[1], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fmaxhp1 >= 1000.f)
	{
		m_itexnum = (((_uint)m_fmaxhp1 % 10000) / 1000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[2], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fmaxhp1 >= 100.f)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[3], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (((_uint)m_fmaxhp1 % 1000) / 100);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}
	m_fPosition.x += m_fNext;
	if (m_fmaxhp1 >= 10.f)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[4], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (((_uint)m_fmaxhp1 % 100) / 10);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}


	
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[5], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (_uint)m_fmaxhp1 % 10;

		m_fPosition.x += m_fNext;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	

	return S_OK;
}

void CUI_BossMonster_HPbar::calculateboss1()
{
	m_fMinusHp = m_fmaxhp - m_fcurrenthp;
	_uint barindex = 4;
	_float maxonebar = m_fmaxhp*0.25f;

	if (maxonebar - m_fMinusHp >= 0.f)       // 양수면 이번거에!
	{
		m_fCurrentbar = maxonebar - m_fMinusHp;
		return;
	}
	//이걸던지자

	else
	{
		--barindex;
		m_fMinusHp -= maxonebar;                                //음수면           3
	}

	if (maxonebar - m_fMinusHp >= 0.f)       // 양수면 이번거에!
	{
		m_fCurrentbar = maxonebar - m_fMinusHp;
		return ;
	}

	else
	{
		--barindex;
		m_fMinusHp -= maxonebar;                                //음수면             2
	}

	if (maxonebar - m_fMinusHp >= 0.f)       // 양수면 이번거에!
	{
		m_fCurrentbar = maxonebar - m_fMinusHp;
		return ;
	}

	else
	{
		--barindex;
		m_fMinusHp -= maxonebar;                                //음수면             1
	}

	m_fCurrentbar = maxonebar - m_fMinusHp;
}

void CUI_BossMonster_HPbar::calculateboss2()
{
	m_fMinusHp1 = m_fmaxhp1 - m_fcurrenthp1;
	_uint barindex = 4;
	_float maxonebar = m_fmaxhp1*0.25f;

	if (maxonebar - m_fMinusHp1 >= 0.f)       // 양수면 이번거에!
	{
		m_fCurrentbar1 = maxonebar - m_fMinusHp1;
		return;
	}
	//이걸던지자

	else
	{
		--barindex;
		m_fMinusHp1 -= maxonebar;                                //음수면           3
	}

	if (maxonebar - m_fMinusHp1 >= 0.f)       // 양수면 이번거에!
	{
		m_fCurrentbar1 = maxonebar - m_fMinusHp1;
		return;
	}

	else
	{
		--barindex;
		m_fMinusHp1 -= maxonebar;                                //음수면             2
	}

	if (maxonebar - m_fMinusHp1 >= 0.f)       // 양수면 이번거에!
	{
		m_fCurrentbar1 = maxonebar - m_fMinusHp1;
		return;
	}

	else
	{
		--barindex;
		m_fMinusHp1 -= maxonebar;                                //음수면             1
	}

	m_fCurrentbar1 = maxonebar - m_fMinusHp1;
}

HRESULT CUI_BossMonster_HPbar::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HPbar"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_font"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monstername"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BossMonster_HPbar::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(m_iMonstername))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fcurrenthp, sizeof(_float))))
		return E_FAIL;

	



	return S_OK;
}

CUI_BossMonster_HPbar * CUI_BossMonster_HPbar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_BossMonster_HPbar*	pInstance = new CUI_BossMonster_HPbar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_BossMonster_HPbar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_BossMonster_HPbar::Clone(void * pArg)
{
	CUI_BossMonster_HPbar*	pInstance = new CUI_BossMonster_HPbar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_BossMonster_HPbar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BossMonster_HPbar::Free()
{

	//Safe_Release(m_pTextureCom1);
	__super::Free();
	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	Safe_Release(m_pTextureCom3);
}
