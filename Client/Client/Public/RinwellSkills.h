#pragma once
#include "Bullet.h"

BEGIN(Client)
class CRinwellSkills final : public CBullet
{
public:
	enum TYPE { PHOTON_FLASH };

public:
	CRinwellSkills(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRinwellSkills(const CRinwellSkills& rhs);
	virtual ~CRinwellSkills() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;

private:
	void Tick_PhotonFlash(_float fTimeDelta);


public:
	static CRinwellSkills* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

};

END