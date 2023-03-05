#pragma once

#include "Player.h"
#include "Hawk.h"
#include "Client_Defines.h"
#include "Collision_Manger.h"

BEGIN(Client)
class CHawkState
{
public:
	enum FIELD_STATE_ID
	{
		FIELD_STATE_IDLE,
		STATE_WALK,
		STATE_TURN_L,
		STATE_TURN_R,
		FIELD_STATE_END,
	};

	enum STATE_ID
	{
		START_BATTLEMODE,
		STATE_DORMANT,
		STATE_IDLE,
		STATE_MOVE,
		STATE_DISCOVER,
		STATE_BATTLE,
		STATE_GRAB_START,
		STATE_GRAB_LOOP,
		STATE_GRAB_END,
		STATE_TAKE_DAMAGE,
		STATE_DASH,
		STATE_PECK,
		STATE_TORNADE,
		STATE_DEAD,
		STATE_BRAVE,
		STATE_DOWN,
		STATE_ARISE,
		STATE_END
	};



	virtual ~CHawkState() {};
	virtual CHawkState* AI_Behaviour(_float fTimeDelta) { return nullptr; };
	virtual CHawkState* Tick(_float fTimeDelta) PURE;
	virtual CHawkState* LateTick(_float fTimeDelta) PURE;

	virtual void Enter() PURE;
	virtual void Exit() PURE;

	CHawkState* ChangeState(CHawkState* pCurrentState, CHawkState* pNewState)
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
	void Find_Target_InField(void)
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		if (!pPlayer)
			return;

		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));

		if (fDistance < m_pOwner->Get_AggroRadius())
		{
			m_pTarget = pPlayer;

			m_pOwner->Get_Transform()->Change_Speed(m_pOwner->Get_Stats().m_fRunSpeed);

			if (5 > fDistance)
				m_bBattleMode = true;
		}

		else
			m_pTarget = nullptr;


		m_fTarget_Distance = fDistance;
	}

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
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);

		m_pTarget = pPlayer;
		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
		return fDistance;


		m_pOwner->Get_Transform()->Change_Speed(m_pOwner->Get_Stats().m_fRunSpeed);
		if (fDistance < m_pOwner->Get_Attack_BiteRadius())
		{

			m_pOwner->Get_Transform()->Change_Speed(m_pOwner->Get_Stats().m_fRunSpeed);

		}
	}


	virtual _vector Get_PlayerPos()
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);
		/*if (!pPlayer)
			return ;*/

		_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		return vPlayerPosition;
	}

	_bool Is_TargetInSight(_fvector vAt, _float vSightAngle)
	{
		_vector		vMonPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
		_vector		vMonLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));


		_vector		vTargetDir = XMVector3Normalize(vAt - vMonPos);
		_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		_vector		vMonRight = XMVector3Cross(vAxisY, vMonLook);
		_vector		vMonUp = XMVector3Cross(vMonLook, vMonRight);


		_float fDot = XMVectorGetX(XMVector3Dot(vMonLook, vTargetDir));

		_float fRadian = acos(fDot) * (180 / XM_PI);

		//앞쪽인지 뒤쪽인지 
		_float fLookDot = XMVectorGetX(XMVector3Dot(vMonLook, vTargetDir));


		_float fSightAngle = vSightAngle;

		//앞쪽에 있고 시야각 안에 있다
		if (fLookDot > 0)
		{
			if (fRadian <= fSightAngle)
				return true;

			else
				return false;
		}

		else
			return false;

		return false;
	}



protected:
	STATETYPE m_eStateType = STATETYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;

	_bool m_bIsAnimationFinished = false;
	_bool m_bHasSpottedTarget = false;
	_bool m_bBattleMode = false;
	_bool m_bBitePossible = false;
	_bool m_bAnimFinish = false;
	_bool m_bFisrtSound = false;

	_float	m_fTimeDeltaAcc = 0.f;
	_float	m_fRandTime;
	_float m_fTarget_Distance = 0.f;
	CHawk* m_pOwner = nullptr;
	class CPlayer* m_pTarget = nullptr;		/* If TRUE, has Aggro. */
	CBaseObj*	m_pCurTarget = nullptr;
	_vector		m_vCurTargetPos;
	_bool		m_bIsATK = false;

};
END