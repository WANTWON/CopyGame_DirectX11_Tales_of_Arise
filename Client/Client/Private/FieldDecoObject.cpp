#include "stdafx.h"
#include "..\Public\FieldDecoObject.h"
#include "Player.h"
#include "MonsterBullet.h"
#include "Navigation.h"

CFieldDecoObject::CFieldDecoObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBaseObj(pDevice, pContext)
{
}

HRESULT CFieldDecoObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFieldDecoObject::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_DecoDesc, pArg, sizeof(DECODESC));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	_vector vecPostion = XMLoadFloat3(&m_DecoDesc.vInitPos);
	vecPostion = XMVectorSetW(vecPostion, 1.f);
	

	switch (m_DecoDesc.eDecoType)
	{
	case BUTTERFLY:
		vecPostion = XMVectorSetY(vecPostion, XMVectorGetY(vecPostion)+ 2.f);
		m_eState = IDLE;
		Set_Scale(_float3(1.2f, 1.2f, 1.2f));
		break;
	case BIRD_GREEN:
	case BIRD_ORANGE:
		m_eState = IDLE;
		Set_Scale(_float3(0.8f, 0.8f, 0.8f));
		CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_INTERACT, this);
		break;
	case WEATHER_CLOCK:
	case MOOSH:
		m_eState = IDLE;
		Set_Scale(_float3(1.2f, 1.2f, 1.2f));
		CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_BLOCK, this);
		break;
	case FOX:
		m_eState = FOX_IDLE;
		Set_Scale(_float3(1.2f, 1.2f, 1.2f));
		CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_BLOCK, this);
		break;
	default:
		break;
	}

	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_DecoDesc.fAngle));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);
	m_pNavigationCom->Compute_CurrentIndex_byDistance(vecPostion);
	m_fHeight = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	

	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//char cName[MAX_PATH];
	//ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//pData_Manager->TCtoC(TEXT("Moosh"), cName);
	//pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
	////ERR_MSG(TEXT("Save_Bin_Model"));
	//RELEASE_INSTANCE(CData_Manager);

	//RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

int CFieldDecoObject::Tick(_float fTimeDelta)
{
	if (Check_IsinFrustum() == false)
		return OBJ_NOEVENT;

	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;

	

	switch (m_DecoDesc.eDecoType)
	{
	case BUTTERFLY:
		Butterfly_Tick(fTimeDelta);
		break;
	case BIRD_GREEN:
	case BIRD_ORANGE:
		Bird_Tick(fTimeDelta);
		break;
	default:
		break;
	}

	
	m_pModelCom->Set_NextAnimIndex(m_eState);
	Change_Animation(fTimeDelta);

	
	

	return OBJ_NOEVENT;
}

void CFieldDecoObject::Late_Tick(_float fTimeDelta)
{
	
	if (Check_IsinFrustum(2.f) == false)
		return;

	__super::Late_Tick(fTimeDelta);


	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	}
		
	SetUp_ShaderID();

	Check_Navigation(fTimeDelta);

	

}

HRESULT CFieldDecoObject::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
			return E_FAIL;


		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CFieldDecoObject::Render_ShadowDepth()
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

void CFieldDecoObject::Change_Animation(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CFieldDecoObject::IDLE:
	case Client::CFieldDecoObject::WALK:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		break;
	default:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		break;

	}
}

void CFieldDecoObject::Butterfly_Tick(_float fTimeDelta)
{
	if (m_dwIdleTime + rand() % 5000 < GetTickCount())
	{
		m_fAngle = (rand() % 5000)*0.001f;
		m_dwIdleTime = GetTickCount();
	}


	m_fAngle -= 0.1f;
	if (m_fAngle <= 0.f)
	{
		m_fAngle = 0.f;
		m_fHeight += rand() % 2 == 0 ? 0.002f : -0.002f;
	}

	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPosition = XMVectorSetY(vPosition, m_fHeight);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fAngle);
	m_pTransformCom->Go_StraightSliding(fTimeDelta, m_pNavigationCom);
}

void CFieldDecoObject::Bird_Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CBaseObj* m_pTarget = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	if (m_pTarget == nullptr)
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	_vector vTargetPos = m_pTarget->Get_TransformState(CTransform::STATE_POSITION);
	_vector vDir = Get_TransformState(CTransform::STATE_POSITION) - vTargetPos;
	_float fDistanceToTarget = XMVectorGetX(XMVector3Length(vDir));

	if (fDistanceToTarget < 3)
	{
		if (!m_bFIrst)
		{
			_uint iNum = rand() % 3 + 1;
			_tchar	sz_SoundMonster[MAX_PATH];
			_float fVolume = 0.2f;
			wcscpy_s(sz_SoundMonster, TEXT("5_Obj_Bird_Wait (%d).wav"));
			wsprintf(sz_SoundMonster, sz_SoundMonster, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_SoundMonster, SOUND_OBJECT, 0.2f);
			m_bFIrst = true;
		}
	m_eState = WALK;
	m_pTransformCom->Go_StraightSliding(fTimeDelta, m_pNavigationCom);
	vDir = XMVectorSetY(vDir, 0.f);
	m_pTransformCom->LookDir(XMVector3Normalize(vDir));
	}
	else
	{
		// Switch between Idle and Walk (based on time)
		if (m_eState == STATE::IDLE)
		{
			if (GetTickCount() > m_dwIdleTime + (rand() % 1500) * (rand() % 3 + 1) + 3000)
			{
				if (fDistanceToTarget < 5)
				{
					_uint iNum = rand() % 3 + 1;
					_tchar	sz_SoundMonster[MAX_PATH];
					_float fVolume = 0.2f;
					wcscpy_s(sz_SoundMonster, TEXT("5_Obj_Bird_Wait (%d).wav"));
					wsprintf(sz_SoundMonster, sz_SoundMonster, iNum);
					CGameInstance::Get_Instance()->PlaySounds(sz_SoundMonster, SOUND_OBJECT, 0.2f);
				}
					
				m_eState = STATE::WALK;
				m_dwWalkTime = GetTickCount();

				m_eDir[DIR_X] = (rand() % 200)*0.01f - 1.f;
				m_eDir[DIR_Z] = (rand() % 200)*0.01f - 1.f;

			}
		}
		else if (m_eState == STATE::WALK)
		{
			if (GetTickCount() > m_dwWalkTime + (rand() % 3000) * (rand() % 3 + 1) + 1500)
			{
				m_bFIrst = false;
				m_eState = STATE::IDLE;
				m_dwIdleTime = GetTickCount();
			}
		}
	}


	// Movement
	if (m_eState == STATE::WALK)
	{
		Change_Direction();
		m_pTransformCom->Go_StraightSliding(fTimeDelta, m_pNavigationCom);
	}

	CBaseObj* pCollisionBird = nullptr;
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_INTERACT, m_pSPHERECom, &pCollisionBird))
	{

		_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pCollisionBird->Get_TransformState(CTransform::STATE_POSITION);
		if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
			vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
		else
			vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);
		m_pTransformCom->Go_PosDir(fTimeDelta, vDirection, m_pNavigationCom);
	}


	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CFieldDecoObject::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	switch (m_DecoDesc.eDecoType)
	{
	case BUTTERFLY:
		TransformDesc.fSpeedPerSec = 3.f;
		break;
	case BIRD_GREEN:
	case BIRD_ORANGE:
		TransformDesc.fSpeedPerSec = 3.f;
		break;
	case WEATHER_CLOCK:
		break;
	default:
		break;
	}
	
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	

	/* For.Com_Model*/
	switch (m_DecoDesc.eDecoType)
	{
	case BUTTERFLY:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Butterfly"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case BIRD_GREEN:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("BirdGreen"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case BIRD_ORANGE:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("BirdOrange"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case WEATHER_CLOCK:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("WeatherClock"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case MOOSH:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Moosh"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case FOX:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Fox"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	default:
		break;
	}

	
	/* For.Com_SPHERE*/
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _float3(1.0f, 1.0f, 1.0f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC			NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	NaviDesc.iCurrentCellIndex = 0;
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_Field"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Field"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CFieldDecoObject::SetUp_ShaderResources()
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

	return S_OK;
}



void CFieldDecoObject::Check_Navigation(_float fTimeDelta)
{
	if (m_pNavigationCom->Get_CurrentCelltype() == CCell::ACCESSIBLE)
	{
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float m_fHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);
		if (m_fHeight > XMVectorGetY(vPosition))
		{
			vPosition = XMVectorSetY(vPosition, m_fHeight);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

		}
	}
}



CFieldDecoObject * CFieldDecoObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFieldDecoObject*	pInstance = new CFieldDecoObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CFieldDecoObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFieldDecoObject::Clone(void * pArg)
{
	CFieldDecoObject*	pInstance = new CFieldDecoObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CFieldDecoObject"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFieldDecoObject::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pNavigationCom);
}

