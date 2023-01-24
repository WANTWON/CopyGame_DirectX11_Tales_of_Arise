#pragma once
#include "Client_Defines.h"
#include "BaseObj.h"
#include "GameInstance.h"
#include "Cell.h"

BEGIN(Engine)
class CModel;
class CTexture;
END

BEGIN(Client)
class CInteractObject abstract : public CBaseObj
{
protected:
	CInteractObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInteractObject(const CInteractObject& rhs);
	virtual ~CInteractObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

protected:
	virtual HRESULT SetUp_ShaderResources();
	virtual HRESULT SetUp_ShaderID();
	virtual HRESULT Drop_Items();

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr)	PURE;
	virtual _bool Is_AnimationLoop(_uint eAnimId)			PURE;
	
protected:

protected: /* For.Components */
	CModel*					m_pModelCom = nullptr;
	CTexture*				m_pDissolveTexture = nullptr;
	_uint					m_iCollisiongCount = 0;
	_bool					m_bCollision = false;
	_bool					m_bIsAnimationFinished = false;
public:
	virtual void Free() override;
};
END