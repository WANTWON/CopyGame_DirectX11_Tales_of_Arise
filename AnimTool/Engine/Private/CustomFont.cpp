#include "..\Public\CustomFont.h"

CCustomFont::CCustomFont(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCustomFont::Initialize(const _tchar * pFontFilePath)
{
	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);
	m_pBatch = new SpriteBatch(m_pContext);

	return S_OK;
}

HRESULT CCustomFont::Render(const _tchar * pText, _fvector vPosition, _fvector vColor)
{
	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch, pText, vPosition, vColor, 0.f, g_XMZero, 1.5f);

	m_pBatch->End();

	return S_OK;
}

CCustomFont * CCustomFont::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pFontFilePath)
{
	CCustomFont* pInstance = new CCustomFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pFontFilePath)))
	{
		ERR_MSG(TEXT("Failed to Created : CCustomFont"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCustomFont::Free(void)
{
	Safe_Delete(m_pFont);
	Safe_Delete(m_pBatch);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
