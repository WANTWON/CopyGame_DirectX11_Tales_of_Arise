#pragma once

#include "Player.h"
#include "AiRinwell.h"
#include "Client_Defines.h"
#include "PlayerManager.h"
#include "GameInstance.h"

BEGIN(Client)
class CRinwellState
{
public:
	enum STATE_ID
	{
		STATE_IDLE
	};



	virtual ~CRinwellState() {};
	virtual CRinwellState* AI_Behaviour(_float fTimeDelta) { return nullptr; };
	virtual CRinwellState* Tick(_float fTimeDelta) PURE;
	virtual CRinwellState* LateTick(_float fTimeDelta) PURE;

	virtual void Enter() PURE;
	virtual void Exit() PURE;

	CRinwellState* ChangeState(CRinwellState* pCurrentState, CRinwellState* pNewState)
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
	void Set_BattleMode() { m_bBattleMode = true; }

protected:
	virtual void Find_Target(_bool bHasAggro = false)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);
		if (!pPlayer)
			return;

		if (bHasAggro)
			m_pTarget = pPlayer;
		else
		{
			_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
			_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

			_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
			if (fDistance < m_pOwner->Get_AggroRadius())
			{
				m_pTarget = pPlayer;

				m_pOwner->Get_Transform()->Change_Speed(m_pOwner->Get_Stats().m_fRunSpeed);

				if (5 > fDistance)
				{
					m_bBattleMode = true;

				}

			}
		}
	}

	virtual _float Find_BattleTarget()
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();

		m_pTarget = pPlayer;
		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
		return fDistance;

		if (fDistance < m_pOwner->Get_Attack_BiteRadius())
		{
			m_pOwner->Get_Transform()->Change_Speed(m_pOwner->Get_Stats().m_fRunSpeed);

			m_bBitePossible = true;
		}
	}


	virtual _vector Get_PlayerPos()
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);

		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		return vPlayerPosition;
	}

	virtual _float RadianToTarget()
	{
		_float fWidth = 0.f, fHeight = 0.f, fDepth = 0.f;

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);
		if (!pPlayer)
			return 0.f;

		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
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
	STATETYPE m_eStateType = STATETYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_IDLE;

	_bool		m_bIsAnimationFinished = false;
	_bool		m_bHasSpottedTarget = false;
	_bool		m_bBattleMode = false;
	_bool		m_bBitePossible = false;
	_float		m_fTimeDletaAcc = 0.f;
	_float		m_fTarget_Distance;
	_float		m_fDegreeToTarget;
	class CAiRinwell* m_pOwner = nullptr;
	class CPlayer* m_pTarget = nullptr;		/* If TRUE, has Aggro. */

};
END