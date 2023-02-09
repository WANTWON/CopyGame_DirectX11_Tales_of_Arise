#pragma once
#include "BaseObj.h"
#include "GameInstance.h"
#include "Effect.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)
class CBullet abstract : public CBaseObj
{
public:
	typedef struct Bullettag
	{
		OBJECT_ID  eCollisionGroup = PLAYER; //Player or Monster bullet;
		_uint	   eBulletType = 0;

		_vector	   vInitPositon = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		
		_float	   fStartTime = 0.f;
		_float	   fDeadTime = 10.f;
		_float	   fVelocity = 5.f;
		_int	   iDamage = 20;
		
		_vector	   vTargetDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		_vector	   vTargetPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);

		CBaseObj*  pTarget = nullptr;
		CBaseObj*  pOwner = nullptr;
		_float offsetX = 0.f;
		_float offsetZ = 0.f;

	}BULLETDESC;

protected:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr);
	virtual HRESULT SetUp_ShaderResources();
	virtual HRESULT SetUp_ShaderID();
	virtual void Collision_Check();

protected:
	BULLETDESC m_BulletDesc;
	CModel* m_pModelCom = nullptr;
	vector<CEffect*> m_pEffects;
	vector<CEffect*> m_pDeadEffects;

	_float	m_fTime = 0;
	_bool	m_bDeadEffect = false;

public:
	virtual void Free() override;
};


END