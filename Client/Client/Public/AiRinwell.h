#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CAiRinwell final : public CMonster
{
public:
	enum ANIM {
	
	};



public:
	CModel* Get_Model() { return m_pModelCom; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	class CIceWolfState* Get_State() { return m_pState; }
	void Set_PlayerState(class CIceWolfState* pPlayerState) { m_pState = pPlayerState; }
	void Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }

public:
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	virtual _int Take_Damage(int fDamage, CBaseObj* DamageCauser) override;
	virtual HRESULT SetUp_ShaderID() override;

private:
	CAiRinwell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAiRinwell(const CAiRinwell& rhs);
	virtual ~CAiRinwell() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public: /*For.State*/
	void AI_Behavior(_float fTimeDelta);
	void Tick_State(_float fTimeDelta);
	void LateTick_State(_float fTimeDelta);


		/*For Navigation*/
	virtual void Check_Navigation() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	class CIceWolfState*  m_pState = nullptr;

	_float   m_fSpeed = 3.f;
	_bool   m_bDoneChangeState = false;
	

public:
	static CAiRinwell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END