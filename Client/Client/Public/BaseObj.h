#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "UI_Manager.h"
#include "Collision_Manger.h"
#include "Data_Manager.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CCollider;
END

BEGIN(Client)

class CBaseObj abstract : public CGameObject
{
public:
	enum DIRINPUT {DIR_X,DIR_Z, DIR_END};

protected:
	CBaseObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBaseObj(const CBaseObj& rhs);
	virtual ~CBaseObj() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Change_Direction();
	void Update_Collider();
	virtual _uint Take_Damage(float fDamage, void* DamageType, CBaseObj* DamageCauser) { return 0; };
	_bool Check_IsinFrustum(_float fOffset = 0);
	void SetUp_BillBoard();


public: /* Getter Setter */
	OBJID		Get_ObjectID() { return m_eObjectID; }
	_float3		Get_Scale() { return m_vScale; };
	_vector		Get_TransformState(CTransform::STATE eState);
	_float2		Get_ProjPosition();
	void		Set_State(CTransform::STATE eState, _fvector vState);
	void		Set_Scale(_float3 vScale);
	CCollider*	Get_Collider();
	_bool		Get_Stop() { return m_bStop; }
	void		Set_Stop(_bool type) { m_bStop = type; m_eDir[DIR_X] = 0;  m_eDir[DIR_Z] = 0; }
	_bool		Get_IsSocket() { return m_bSocket; }
	
protected: /* For.Components */
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	CCollider*				m_pSPHERECom = nullptr;

protected:
	virtual HRESULT Ready_Components(void* pArg) = 0;
	virtual HRESULT SetUp_ShaderID() { return S_OK; };
	virtual HRESULT SetUp_ShaderResources() = 0;

protected:
	_bool			m_bIsLoop = true;
	_bool			m_bStop = false;
	_bool			m_bSocket = false;


	_bool					m_bSoundOnce = false;
	_float					m_fSoundTime = 0.f;
	_float					m_fSoundEndTime = 0.f;


	LEVEL			m_iCurrentLevel = LEVEL_END;
	_uint			m_eShaderID = SHADER_DEFAULT;
	OBJID			m_eObjectID = OBJ_END;
	_float3			m_vScale = _float3(1.f,1.f,1.f);
	_float3			m_vMousePickPos;
	_float			m_eDir[DIR_END] = { 0 };
	_float			m_ePreDir[DIR_END] = { 0 };
	_float			m_fWalkingHeight = 0.f;
	_float			m_fAlpha = 1.f;

public:
	virtual void Free() override;
};

END