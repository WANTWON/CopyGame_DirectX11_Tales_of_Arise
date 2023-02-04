#pragma once
#include "BaseObj.h"
#include "GameInstance.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)
class CBullet abstract : public CBaseObj
{
public:
	typedef struct Bullettag
	{
		OBJECT_ID eBulletType = PLAYER; //Player or Monster bullet;
		_vector	   vInitPositon = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		_vector	   vLook = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		_float	   fStartTime = 0.f;
		_float	   fDeadTime = 10.f;
		_float	   fVelocity = 5.f;
		CBaseObj*  pTarget = nullptr;

	}BULLETDESC;

public:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr);
	virtual HRESULT SetUp_ShaderResources();
	virtual HRESULT SetUp_ShaderID();

protected:
	BULLETDESC m_BulletDesc;
	CModel* m_pModelCom = nullptr;


public:
	virtual void Free() override;
};


END