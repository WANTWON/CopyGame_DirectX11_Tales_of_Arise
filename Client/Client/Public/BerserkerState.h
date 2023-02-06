#pragma once

#include "Player.h"
#include "Berserker.h"
#include "Client_Defines.h"


BEGIN(Client)
class CBerserkerState
{
public:
	enum FIELD_STATE_ID
	{
		FIELD_STATE_IDLE,
		STATE_WALK,
		STATE_TURN_L,
		STATE_TURN_R,
		STATE_CHASE,
		STATE_HOWLING,
		FIELD_STATE_END
	};

	enum STATE_ID
	{
		STATE_IDLE,
		STATE_MOVE,
		STATE_CALM,
		STATE_ANGRY,
		STATE_CRAZY,
		STATE_DISCOVER,
		STATE_BATTLE,
		STATE_DASH_START, // ¸ø¾¸
		STATE_DASH_LOOP,
		STATE_DASH_SCRATCHES, //¸ø ¾¸.
		STATE_QUADRUPLE,
		STATE_POUNCING,
		STATE_FIREBALL,
		STATE_HIT,
		STATE_DEAD,
		STATE_END
	};

	enum EMOTION_STATE
	{
		EMO_CALM,
		EMO_READY_ANGRY,
		EMO_ANGRY
	};

	virtual ~CBerserkerState() {};
	virtual CBerserkerState* AI_Behaviour(_float fTimeDelta) { return nullptr; };
	virtual CBerserkerState* Tick(_float fTimeDelta) PURE;
	virtual CBerserkerState* LateTick(_float fTimeDelta) PURE;

	virtual void Enter() PURE;
	virtual void Exit() PURE;

	CBerserkerState* ChangeState(CBerserkerState* pCurrentState, CBerserkerState* pNewState)
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
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);
		//if (!pPlayer)
		//	return;

			m_pTarget = pPlayer;

			_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
			_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

			_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
			

			if (fDistance < m_pOwner->Get_Attack_BiteRadius())
			{

				m_pOwner->Get_Transform()->Change_Speed(m_pOwner->Get_Stats().m_fRunSpeed);
				
				m_bBitePossible = true;

			}

			if (fDistance < -10000)
				int a = 0;
		
			return fDistance;
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

	_float Find_ToTargetCosign(_fvector vAt)
	{
		//_vector vTargetDir = XMVector3Normalize(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pMonSterTransform->Get_State(CTransform::STATE_TRANSLATION));
		//_vector vLook = XMVector3Normalize(pMonSterTransform->Get_State(CTransform::STATE_LOOK));

		_vector		vMonPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
		_vector		vLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));


		_vector		vTargetDir = XMVector3Normalize(vAt - vMonPos);
		_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		_vector		vRight = XMVector3Cross(vAxisY, vLook);
		_vector		vUp = XMVector3Cross(vLook, vRight);

	
		_float fDot = XMVectorGetX(XMVector3Dot(vLook, vTargetDir));

		//_float fRadian = acos(fDot);

		return fDot;
	}

	_bool Find_ToTargetRightSide(_fvector vAt)
	{
		_vector		vMonPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
		_vector		vLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));
		_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		_vector		vRight = XMVector3Cross(vAxisY, vLook);
		_vector		vUp = XMVector3Cross(vLook, vRight);

		_vector		vTargetDir = XMVector3Normalize(vAt - vMonPos);

		_float fDot = XMVectorGetX(XMVector3Dot(vRight, vTargetDir));

		if (fDot > 0)
			return true;	//¿À¸¥ÂÊ 

		else
			return false;  
		

		return false;
		
	}


protected:
	STATETYPE m_eStateType = STATETYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;
	
	_bool	    m_bIsAnimationFinished = false;
	_bool	    m_bHasSpottedTarget = false;
	_bool	    m_bBattleMode = false;
	_bool	    m_bBitePossible = false;
	_bool		m_bTriggerTurn = false;
	_bool		m_bAngry = false;
	_bool		m_bTarget_isRight = false;
	_bool		m_bFisrtSound = false;
	_bool		m_bSecondSound = false;

	_float		m_fTimeDeltaAcc = 0.f;
	_float		m_fTarget_Distance;
	_float		m_fRandTime;
	_float		m_fTarget_Radian = 0.f;
	_float		m_fTarget_Cosign = 0.f;
	_float		m_fCosignTimeAcc;
	_float		m_fCosign;
	_float		m_fEventSound_Count = 0.f;
	_float		m_fSoundStart = -1.f;
	CBaseObj*	m_pCurTarget = nullptr;
	_vector		m_vCurTargetPos;


	CBerserker* m_pOwner = nullptr;
	class CPlayer* m_pTarget = nullptr;		/* If TRUE, has Aggro. */
};
END