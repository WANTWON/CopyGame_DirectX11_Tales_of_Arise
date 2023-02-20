#pragma once

#include "BaseObj.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)
class CAnim final :	public CBaseObj
{
public:
	enum PARTS { PARTS_WEAPON, PARTS_END };

public:
	typedef struct tagPlayerDesc
	{
		_float3 vPosition;
		_float3 vScale;
		_float3 vRotation;
		MODEL eModel;
		_bool isOriginalLoad = false;
	}PLAYERDESC;

private:
	CAnim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAnim(const CAnim& rhs);
	virtual ~CAnim() = default;

public:
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
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render(void);
	
public:
	void Set_ModelDesc(NONANIMDESC* NonAnimDesc) { memcpy(&m_ModelDesc, NonAnimDesc, sizeof(NONANIMDESC)); }

private:
	CModel* m_pModelCom = nullptr;
	vector<class CGameObject*> m_Parts;
	_bool m_isPlayAnim = false;
	PLAYERDESC m_ModelDesc;

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT SetUp_ShaderResources(void) override;
	virtual HRESULT SetUp_ShaderID(void) override;

public:
	static CAnim* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};
END