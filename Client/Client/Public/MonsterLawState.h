#pragma once

#include "MonsterLaw.h"
#include "Client_Defines.h"
#include "PlayerManager.h"
#include "GameInstance.h"
#include "Player.h"

BEGIN(Client)
class CMonsterLawState
{
public:
	enum STATE_ID
	{
		STATE_IDLE,
		STATE_BATTLESTART,
		NORMALATTACK_1,
		NORMALATTACK_2,
		NORMALATTACK_3,
		NORMALATTACK_4,
		NORMALATTACK_5,
	    SKILL_R,
		SKILL_E,
		SKILL_F,
		SKILL_STRIKE,
		STATE_DAMAGE,
		STATE_DOWN,
		STATE_DEAD
	};


	enum SKILL_TYPE
	{
		SKILL_END
	};

	virtual ~CMonsterLawState() {};
	virtual CMonsterLawState* AI_Behaviour(_float fTimeDelta) { return nullptr; };
	virtual CMonsterLawState* Tick(_float fTimeDelta) PURE;
	virtual CMonsterLawState* LateTick(_float fTimeDelta) PURE;

	virtual void Enter() PURE;
	virtual void Exit() PURE;

	CMonsterLawState* ChangeState(CMonsterLawState* pCurrentState, CMonsterLawState* pNewState)
	{
		if (pCurrentState)
		{
			pCurrentState->Exit();
			Safe_Delete(pCurrentState);
		}

		pCurrentState = pNewState;
		pCurrentState->Enter();

		return pCurrentState;
	}

public:

	

	STATE_ID Get_StateId() { return m_eStateId; }
	_bool Has_Aggro() { return m_pOwner->Get_Aggro(); }
	void Reset_Target() { m_pTarget = nullptr; }

	virtual _float Find_Target(_uint Index)
	{

		vector<CPlayer*> pPlayerList = CPlayerManager::Get_Instance()->Get_AIPlayers();
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		pPlayerList.push_back(pPlayer);

		//if (Index >= pPlayerList.size() || pPlayerList[Index]->Get_Dead())
		m_pTarget = pPlayer;
		//else
		//m_pTarget = pPlayerList[Index];

		_vector vPlayerPosition = m_pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
		return fDistance;
	}

protected:
	virtual _float Find_ActiveTarget()
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();

		m_pTarget = pPlayer;
		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
		return fDistance;
	}


	virtual _vector Get_PlayerPos()
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);

		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		return vPlayerPosition;
	}

	virtual _float RadianToTarget(_vector vPos)
	{
		_float fWidth = 0.f, fHeight = 0.f, fDepth = 0.f;

		_vector vPlayerPosition = vPos;
		_vector vWolfPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vWolfLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);

		_vector vDistance = vPlayerPosition - vWolfPosition;
		vDistance = XMVector4Normalize(vDistance);

		_float DotValue = XMVectorGetX(XMVector3Dot(vDistance, vWolfLook));

		_float fRadian = acosf(DotValue);
		_float fdegree = XMConvertToDegrees(fRadian);

		return fdegree;

	}




protected:
	STATETYPE	m_eStateType = STATETYPE_DEFAULT;
	STATE_ID	m_eStateId = STATE_IDLE;
	_uint		m_eSkillType = SKILL_END;
	_bool		m_bIsAnimationFinished = false;
	_float		m_fTarget_Distance;
	class CMonsterLaw* m_pOwner = nullptr;
	class CPlayer* m_pTarget = nullptr;		/* If TRUE, has Aggro. */
	_int    m_iPhase = 0;

};
END