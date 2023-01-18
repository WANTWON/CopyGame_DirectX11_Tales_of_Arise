#pragma once

#include "Client_Defines.h"
#include "BaseObj.h"


BEGIN(Engine)
class CNavigation;
class CModel;
END

BEGIN(Client)
class CWater final : public CBaseObj
{
private:
	CWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWater(const CWater& rhs);
	virtual ~CWater() = default;

	_matrix Get_ReflectionMatrix() { return m_ReflectionViewMatrix; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT SetUp_ShaderResources();

	void Compute_ReflectionView();

private:
	CModel* m_pModelCom = nullptr;
	CTexture* m_pWaveHeightMapCom = nullptr; /* Used to update the Normals to simulate waves. */
	CNavigation* m_pNavigationCom = nullptr; /* Also Water Objects need a Navigation Component in order to swim. */
	
	_matrix m_ReflectionViewMatrix;

	_float m_fScrollingTimer = 0.f;

public:
	static CWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END