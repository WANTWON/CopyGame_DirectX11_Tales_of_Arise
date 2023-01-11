#pragma once
#include "Client_Defines.h"
#include "Effect.h"
#include "GameInstance.h"



BEGIN(Client)
class CMonsterEffect final : public CEffect
{
public:
	enum TYPE { 
		/* For Octorock */
		BULLET_SMOKE, BLAST_RING, OCTOROCK_STONE,

		/* For Albatoss */
		CLAW_SMOKE, CLAW_RING, FLAPPING_SMOKE, 
		FEATHER, ALBATROSSDEAD_SMOKE, GLOW_SPHERE,

		/* For Rola */
		ROLLING_SMOKE, ROLA_SMOKE, ROLLING_FLASH, ROLLING_FLASH2,
		ROLA_PUSHSPARK, ROLA_PUSHRING, ROLA_PUSHFLASH

	};



protected:
	CMonsterEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonsterEffect(const CMonsterEffect& rhs);
	virtual ~CMonsterEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Target(CBaseObj* pObject) { m_pTarget = pObject; }
	void Set_EndMode(_bool type) { m_bEndMode = type; }

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr);
	virtual HRESULT SetUp_ShaderID();
	virtual HRESULT SetUp_ShaderResources() override;

	virtual void Change_Animation(_float fTimeDelta);
	virtual void Change_Texture(_float fTimeDelta) override;

private:
	void Tick_Grass(_float fTimeDelta);
	void Tick_OctoRockEffect(_float fTimeDelta);
	void Tick_AlbatossEffect(_float fTimeDelta);
	void Tick_GlowSphere(_float fTimeDelta);
	void Tick_RolaEffect(_float fTimeDelta);

private:
	_float	m_fAngle = 0.f;
	_float	m_fColorTime = 0.f;
	_float	m_fSpeed = 0.f;
	
	CTexture*		m_pSmokeDstTexture = nullptr;
	CTexture*		m_pDissolveTexture = nullptr;

	vector<_vector> m_vecColor;
	_int			m_iColorIndex = 0;
	_bool			m_bColorChaged = false;
	_bool			m_bEndMode = false;
	_bool			m_bMax = false;
	_float3			m_vMaxScale = _float3(2.f, 0.5f, 0.f);

public:
	static CMonsterEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END