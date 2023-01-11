#pragma once
#include "NonAnim.h"

BEGIN(Client)

class CGrass final : public CNonAnim
{
public:
	enum TYPE { LAWN, GRASS, GRASS2x2 };

	typedef struct KeyTag
	{
		TYPE eType = LAWN;
		_float3 vPosition = _float3(0.f, 0.f, 0.f);
		
	}GRASSDESC;


private:
	CGrass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGrass(const CGrass& rhs);
	virtual ~CGrass() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	GRASSDESC				m_GrassDesc;
	_bool					m_bCut = false;

	CModel*					m_pModelCom2 = nullptr;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CGrass* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END