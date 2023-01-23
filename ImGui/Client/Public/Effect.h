#pragma once

#include "BaseObj.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CModel;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)
class CEffect abstract : public CBaseObj
{
public:
	enum EFFECT_TYPE 
	{ 
		TYPE_TEXTURE,	
		TYPE_MESH,
		TYPE_PARTICLE,
		TYPE_END 
	};

	typedef struct tagEffectDescription
	{
		EFFECT_TYPE eType = TYPE_END;

		_tchar wcFileName[MAX_PATH] = TEXT("");				// "Effect.dds" ~ "Effect.png" ~ "Effect.fbx" 
		_tchar wcFilePath[MAX_PATH] = TEXT("");				// "../../../Bin/Resources/Textures/Effect/*" ~ "../../../Bin/Resources/Meshes/Effect/*"

		_tchar wcTexturePrototypeId[MAX_PATH] = TEXT("");
		_tchar wcModelPrototypeId[MAX_PATH] = TEXT("");

		_bool bIsBillboard = true;
	} EFFECTDESC;
	
	EFFECTDESC Get_EffectDesc() { return m_tEffectDesc; }
	_bool Get_Play() { return m_bPlay; }
	void Set_Play(_bool bPlay) { m_bPlay = bPlay; }

public:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override; 

protected:
	EFFECTDESC m_tEffectDesc;

	_bool m_bPlay = false;

public:
	virtual void Free() override;
};
END