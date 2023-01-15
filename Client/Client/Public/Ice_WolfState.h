
#include "GameInstance.h"
#include "Player.h"
#include "Ice_Wolf.h"
#include "Client_Defines.h"

BEGIN(Client)
class CIce_WolfState
{
public:
	enum STATE_ID
	{
		STATE_IDLE,
		STATE_MOVE,
		STATE_AGGRO,
		STATE_ATTACK,
		STATE_HIT,
		STATE_GUARD,
		STATE_STAGGER,
		STATE_FALL,
		STATE_DIE,
		STATE_END,
	};

public:
	virtual ~CIce_WolfState() {};
	virtual CIce_WolfState* AI_Behavior() PURE;
	virtual CIce_WolfState* Tick( _float fTimeDelta) { return nullptr; };
	virtual CIce_WolfState* LateTick(_float fTimeDelta) { return nullptr; };

	virtual void Enter() PURE;
	virtual void Exit() PURE;

	CIce_WolfState* ChangeState(CIce_WolfState* pCurrentState, CIce_WolfState* pNewState)
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
	_bool Has_Aggro() { return m_pTarget; }
	void Reset_Target() { m_pTarget = nullptr; }

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
			_vector vPlayerPosition = pPlayer->Get_Transform()->Get_State(CTransform::STATE::STATE_TRANSLATION);
			_vector vPosition = m_pOwner->Get_Transform()->Get_State(CTransform::STATE::STATE_TRANSLATION);

			_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
			if (fDistance < m_pOwner->Get_AggroRadius())
			{
				m_pTarget = pPlayer;

				m_pOwner->Get_Transform()->Change_Speed(m_pOwner->Get_Stats().m_fRunSpeed);
			}
		}
	}
	

protected:
	STATETYPE m_eStateType = STATETYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;

	_bool m_bIsAnimationFinished = false;
	_bool m_bHasSpottedTarget = false;

	CIce_Wolf* m_pOwner = nullptr;
	class CPlayer* m_pTarget = nullptr;		/* If TRUE, has Aggro. */
};
END