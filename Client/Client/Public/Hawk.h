#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CHawk final : public CMonster
{
public:
	enum ANIM {

		//ARI00,   //3D���� 1�� 
		//FLAPPING,
		//ATTACK_ROTATION,
		//ATTACK_ROTATION2,//4
		//ATTACK_CHARGE,//5
		//ATTACK_CHARGE_ROTATION, //6
		//FLYING_BACK,//7
		//ATTACK_GRAB_START1, //8
		//ATTACK_GRAB_START2, //9//��ŸƮ�� ���⿣ ������ �ʹ� ª��.
		//REVOLVING_FLIGHT, //10
		//ATTACK_GRAB_END,  //11
		//ATTACK_GRAB_LOOP2, //12
		//HANGING_INTHE_AIR, //13
		//ATTACK_BOMBING_DUP, //6//ATTACK1
		//ATTACK_BRAVE, //7
		//ATTACK_CHARGE2, //8
		//ATTACK_DASH, //9
		//ATTACK_FLUTTER,
		//ATTACK_GRAB_END2,
		//ATTACK_GRAB_LOOP3, //12
		//ATTACK_GRAB_START3, //13
		//ATTACK_PECK,
		//ATTACK_TORNADE, //15
		//BLOW_DOWN_B,
		//BLOW_DOWN_F,
		//BLOW_UP_B,
		//BLOW_UP_F,
		//DAMAGE_AIR_LARGE_B,
		//DAMAGE_AIR_LARGE_F0,
		//DAMAGE_AIR_LARGE_L,
		//DAMAGE_AIR_LARGE_F,
		//DAMAGE_AIR_LARGE_R,
		//DAMAGE_AIR_LOOP,
		//DAMAGE_AIR_SMALL_B,
		//DAMAGE_AIR_SMALL_F,
		//DAMAGE_AIR_SMALL_L,
		//DEAD1,
		//DAMAGE_AIR_SMALL_R,
		//DAMAGE_LARGE_B,
		//DAMAGE_LARGE_F,
		//DAMAGE_LARGE_L,
		//DAMAGE_LARGE_R,
		//DAMAGE_LOOP,
		//DEAD2,
		//DEAD3,
		//DAMAGE_SMALL_B,
		//DAMAGE_SMALL_F,
		//DAMAGE_SMALL_L,//33 �ǰݸ�� 
		//DAMAGE_SMALL_R,//34 �ǰݸ�� 
		//DEAD, 
		//FAST_FLAP_OF_WINGS,//47
		//DOWN_B,//35 DEAD
		//DOWN_F,//36 DEAD
		//DOWN_UNIQUE,
		//MOVE_IDLE,//38���߿��� �����ֱ�
		//MOVE_RUN,//39��������
		//MOVE_WALK_B,//40õõ�� ����
		//MOVE_WALK_F,//41õõ�� ����
		//TURN_L, //42
		//TURN_POSE,
		//TURN_R, //43
		//SYMBOL_DETECT_IDLE,//44
		//SYMBOL_DETECT_STOP,//45 �ɾ��ִ� ���ƿ�����
		//SYMBOL_IDLE,
		//SYMBOL_RUN,//47
		//SYMBOL_STOP,//x
		//SYMBOL_TURN_LEFT, //49x�������� ���� 
		//SYMBOL_TURN_POSE,//x�������� ���� 
		//SYMBOL_TURN_RIGHT,//x�������� ���� . 50
		//SYMBOL_WALK, 
		//ADVENT, //x�������� ���� 52 �ָ��� ���� 
		ARISE_B,
		ARISE_F,
		ATTACK_BOMBING,
		ATTACK_BRAVE,
		ATTACK_CHARGE,
		ATTACK_DASH,
		ATTACK_FLUTTER,
		ATTACK_GRAB_END,
		ATTACK_GRAB_LOOP,
		ATTACK_GRAB_START,
		ATTACK_PECK,
		ATTACK_TORNADE,
		BLOW_DOWN_B,
		BLOW_DOWN_F,
		BLOW_UP_B,
		BLOW_UP_F,
		DAMAGE_AIR_LARGE_B,
		DAMAGE_AIR_LARGE_F,
		DAMAGE_AIR_LARGE_L,
		DAMAGE_AIR_LARGE_R,
		DAMAGE_AIR_LOOP,
		DAMAGE_AIR_SMALL_B,
		DAMAGE_AIR_SMALL_F,
		DAMAGE_AIR_SMALL_L,
		DAMAGE_AIR_SMALL_R,
		DAMAGE_LARGE_B,
		DAMAGE_LARGE_F,
		DAMAGE_LARGE_L,
		DAMAGE_LARGE_R,
		DAMAGE_LOOP,
		DAMAGE_SMALL_B,
		DAMAGE_SMALL_F,
		DAMAGE_SMALL_L,
		DAMAGE_SMALL_R,
		DEAD,
		DOWN_B,
		DOWN_F,
		MOVE_IDLE,
		MOVE_RUN,
		MOVE_WALK_B,
		MOVE_WALK_F,
		TURN_L,
		TURN_R,
		SYMBOL_DETECT_IDLE,
		SYMBOL_DETECT_STOP,
		SYMBOL_IDLE,
		SYMBOL_RUN,
		SYMBOL_STOP,
		SYMBOL_TURN_LEFT,
		SYMBOL_TURN_RIGHT,
		SYMBOL_WALK,
		ADVENT,

	};



public:
	CModel* Get_Model() { return m_pModelCom; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	class CHawkState* Get_State() { return m_pHawkState; }
	void Set_PlayerState(class CHawkState* pHawkState) { m_pHawkState = pHawkState; }

public:
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;

	virtual _int Take_Damage(int fDamage, CBaseObj* DamageCauser) override;

	HRESULT CHawk::SetUp_ShaderID();
private:
	CHawk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHawk(const CHawk& rhs);
	virtual ~CHawk() = default;

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
	class CHawkState*  m_pHawkState = nullptr;
		  _bool		   m_bDoneChangeState = false;
public:
	static CHawk* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END