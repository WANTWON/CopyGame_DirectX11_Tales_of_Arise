#pragma once
#include "Bullet.h"

BEGIN(Client)
class CSionSkills final : public CBullet
{
public:
	enum TYPE { NORMALATTACK };

public:
	CSionSkills(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSionSkills(const CSionSkills& rhs);
	virtual ~CSionSkills() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;

private:
	void Tick_NormalAttack(_float fTimeDelta);


public:
	static CSionSkills* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

};

END