#include "stdafx.h"
#include "Player.h"

#include "GameInstance.h"
#include "Weapon.h"
#include "PlayerState.h"
#include "PlayerIdleState.h"
#include "AIState.h"
#include "AIIdleState.h"

using namespace Player;
using namespace AIPlayer;

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));

	CAIState* pAIState = new AIPlayer::CIdleState(this);
	m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);

	/* Set State */
	CPlayerState* pPlayerState = new Player::CIdleState(this);
	m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pPlayerState);

	m_pPlayerManager = CPlayerManager::Get_Instance();
	Safe_AddRef(m_pPlayerManager);
	return S_OK;
}

int CPlayer::Tick(_float fTimeDelta)
{
	PLAYER_MODE eMode = m_pPlayerManager->Check_ActiveMode(this);

	switch (eMode)
	{
	case Client::ACTIVE:
		HandleInput();
		Tick_State(fTimeDelta);
		break;
	case Client::AI_MODE:
		Tick_AIState(fTimeDelta);
		break;
	case Client::UNVISIBLE:
		return S_OK;
	}	

	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());

	for (auto& pParts : m_Parts)
	{
		if(pParts != nullptr)
			pParts->Tick(fTimeDelta);
	}
		

	return OBJ_NOEVENT;
}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	PLAYER_MODE eMode = m_pPlayerManager->Check_ActiveMode(this);

	switch (eMode)
	{
	case Client::ACTIVE:
		LateTick_State(fTimeDelta);
		break;
	case Client::AI_MODE:
	{
		LateTick_AIState(fTimeDelta);
		if (CGameInstance::Get_Instance()->Key_Up(DIK_1) && m_ePlayerID == SION)
			m_pPlayerManager->Set_ActivePlayer(this);
	}
		break;
	case Client::UNVISIBLE:
		return;
	}

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_Parts[PARTS_WEAPON]);
#ifdef _DEBUG
		if (m_pNavigationCom != nullptr && eMode == ACTIVE)
			m_pRendererCom->Add_Debug(m_pNavigationCom);
		__super::Late_Tick(fTimeDelta);
#endif //_DEBUG

	}

	for (auto& pParts : m_Parts)
	{
		if (pParts != nullptr)
			pParts->Late_Tick(fTimeDelta);
	}

	Check_Navigation();
	
}

HRESULT CPlayer::Render()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer::Render_ShadowDepth()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_ShadowLightView(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, SHADER_ANIMSHADOW)))
			return S_OK;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CPlayer::HandleInput()
{
	CPlayerState* pNewState = m_pPlayerState->HandleInput();
	if (pNewState)
		m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pNewState);
}

void CPlayer::Tick_State(_float fTimeDelta)
{
	CPlayerState* pNewState = m_pPlayerState->Tick(fTimeDelta);
	if (pNewState)
		m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pNewState);
}

void CPlayer::LateTick_State(_float fTimeDelta)
{
	CPlayerState* pNewState = m_pPlayerState->LateTick(fTimeDelta);
	if (pNewState)
		m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pNewState);
}

void CPlayer::Tick_AIState(_float fTimeDelta)
{
	CAIState* pNewState = m_pAIState->Tick(fTimeDelta);
	if (pNewState)
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pNewState);
}

void CPlayer::LateTick_AIState(_float fTimeDelta)
{
	CAIState* pNewState = m_pAIState->LateTick(fTimeDelta);
	if (pNewState)
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pNewState);
}


HRESULT CPlayer::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CPlayer::Change_Navigation(LEVEL eLevel)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (eLevel)
	{
	case Client::LEVEL_BATTLE:
		m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_BattleNavigation")));
		break;
	case Client::LEVEL_SNOWFIELD:
		m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_FieldNavigation")));
		break;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	//if (m_fWalkingHeight > XMVectorGetY(vPosition))
	//{
		vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	//}
}

void CPlayer::Compute_CurrentIndex(LEVEL eLevel)
{
	m_pNavigationCom->Compute_CurrentIndex_byXZ(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
}


void CPlayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_Parts)
		Safe_Release(pGameObject);

	m_Parts.clear();

	for (auto& iter : m_vecNavigations)
		Safe_Release(iter);

	m_vecNavigations.clear();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pPlayerManager);


	Safe_Delete(m_pPlayerState);
	Safe_Delete(m_pAIState);
}