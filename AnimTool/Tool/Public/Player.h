#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CModel;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	enum PARTS { PARTS_WEAPON, PARTS_END };

public:
	typedef struct tagPlayerDesc
	{
		PIVOT tPivot;
		MODEL eModel;
		_bool isOriginalLoad = false;
	}PLAYERDESC;

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	PLAYERDESC Get_Pivot(void) const { return m_tPivot; }
	void Set_AnimIndex(_int iIndex);
	void StartAnim(void) { m_isPlayAnim = true; }
	void StopAnim(void) { m_isPlayAnim = false; }
	
public:
	void PlayAnimation(void);
	HRESULT Ready_Parts(const char* pBoneName, const _tchar* pModelTag);
	HRESULT Change_Bone(const char* pBoneName);

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render(void);

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;

private:
	PLAYERDESC m_tPivot;
	vector<class CGameObject*> m_Parts;
	_bool m_isPlayAnim = false;

private:
	HRESULT Ready_Components(void);
	HRESULT SetUp_ShaderResources(void);

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};

END