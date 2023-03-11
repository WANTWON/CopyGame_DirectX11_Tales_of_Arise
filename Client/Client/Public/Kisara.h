#pragma once
#include "Player.h"

BEGIN(Client)
class CKisara final : public CPlayer
{
public:
	enum ANIM {
		STRIKE_HELPER
	};
private:
	CKisara(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKisara(const CKisara& rhs);
	virtual ~CKisara() = default;

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);

public:
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;

private:
	virtual HRESULT Ready_Parts(void) override;
	virtual HRESULT Ready_Components(void* pArg) override;
	HRESULT SetUp_ShaderResources(void);

public:
	static CKisara* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};
END