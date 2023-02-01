#include "..\Public\Texture_Manager.h"

IMPLEMENT_SINGLETON(CTexture_Manager)

CTexture_Manager::CTexture_Manager()
{
}

HRESULT CTexture_Manager::Add_Texture(const _tchar * pTextureTag, CTexture * pTexture)
{
	_tchar* pTextureTagEmplace = new _tchar[MAX_PATH];
	lstrcpy(pTextureTagEmplace, pTextureTag);

	m_pTextures.emplace(pTextureTagEmplace, pTexture);
	
	Safe_AddRef(pTexture);

	return S_OK;
}

CTexture * CTexture_Manager::Find_Texture(const _tchar * pTextureTag)
{
	auto iter = find_if(m_pTextures.begin(), m_pTextures.end(), CTag_Finder(pTextureTag));
	
	if (iter == m_pTextures.end())
		return nullptr;
	
	return iter->second;
}

void CTexture_Manager::Free(void)
{
	for (auto& Pair : m_pTextures)
	{
		Safe_Release(Pair.second);
		delete Pair.first;
	}
		
	m_pTextures.clear();
}
