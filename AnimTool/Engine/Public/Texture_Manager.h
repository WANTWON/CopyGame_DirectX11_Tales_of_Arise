#pragma once
#include "Texture.h"

BEGIN(Engine)

class CTexture_Manager final : public CBase
{
	DECLARE_SINGLETON(CTexture_Manager)
private:
	CTexture_Manager();
	virtual ~CTexture_Manager() = default;

public:
	HRESULT Add_Texture(const _tchar* pPrototypeTag, class CTexture* pPrototype);
	class CTexture* Find_Texture(const _tchar* pTextureTag);

private:
	map<const _tchar*, class CTexture*> m_pTextures;
	typedef map<const _tchar*, class CTexture*> TEXTURES;

public:
	virtual void Free(void) override;
};

END