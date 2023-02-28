#include "stdafx.h"
#include "..\Public\UI_Dialoguepopup.h"

#include "GameInstance.h"
#include <fstream>
#include <iostream>
#include <string>


CUI_Dialoguepopup::CUI_Dialoguepopup(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Dialoguepopup::CUI_Dialoguepopup(const CUI_Dialoguepopup & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Dialoguepopup::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Dialoguepopup::Initialize(void * pArg)
{


	m_FontR = 0.9019607843137255f;
	m_FontG = 0.8235294117647059f;
	m_FontB = 0.6588235294117647f;
	m_fSize.x = 720.f;
	m_fSize.y = 150.f;
	m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;
	m_fFontPos.x = 500.f;
	m_fAlpha = 0;

	


	Read_TextFiles_for_dialogue();
	Read_TextFiles_for_dialogue_first_battle();
	Read_TextFiles_for_dialogue_jelous();
	Read_TextFiles_for_dialougeStage2Start();
	Read_TextFiles_for_dialogue_jelous2();

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Dialoguepopup::Tick(_float fTimeDelta)
{
	if (m_btick)
	{
		if (m_bfirstarrived)
			timer += fTimeDelta;

		if (timer > 5.f)
		{


			m_bfadein1 = true;
			m_bgoup = true;     //++m_iDialogueindex;
			m_bseconton = true;
			m_fDietimer += fTimeDelta;
			timer = 0.f;
			m_bDeadtimeron = true;

		}

		if (m_bDeadtimeron)
		{
			m_fDietimer += fTimeDelta;

			if (m_fDietimer > 4.f)
			{
				m_bfadeout = true;
				m_bfirstarrived = false;

				m_fDietimer = 0.f;
				m_bDeadtimeron = false;
			}


		}

		

		if (m_bfadein)
		{
			m_fAlpha += 0.1f; //생길때
			m_fFadeX += 2.f;
			//m_bfirstarrived = true;
		}
		else if (m_bfadeout)
		{
			m_fAlpha -= 0.1f;
			m_fAlpha1 -= 0.1f;
			m_fFade1X -= 2.f;
			m_fFadeX -= 2.f;

		}

		if (m_bfadein1)
		{
			m_fAlpha1 += 0.1f;
			m_fFade1X += 2.f;
		}

		if (m_bgoup)
		{
			m_fFadeY -= 5.f;
		}

		if (!m_bIsbattle)
		{
			if (m_fFadeY < -110.f)
				m_bgoup = false;
		}
		else
		{
			if (m_fFadeY < -350.f)
				m_bgoup = false;

		}
		



		//	m_fAlpha = 0.5f;

		//if (CGameInstance::Get_Instance()->Key_Up(DIK_4)) // 사라질때
		//{
		//	m_bfadeout = true;
		//}
		//if (CGameInstance::Get_Instance()->Key_Up(DIK_5)) // 생겨질때
		//{
		//	m_bfadein = true;
		//}
		//if (CGameInstance::Get_Instance()->Key_Up(DIK_7)) // 생겨질때
		//{
		//	m_bfadein = true;

		//}
	}






	return OBJ_NOEVENT;
}

void CUI_Dialoguepopup::Late_Tick(_float fTimeDelta)
{

	if (m_btick)
	{
		if (m_fAlpha >= 1.f && m_bfadein)
		{
			m_fAlpha = 1.f;
			m_bfadein = false;
			m_bfirstarrived = true;
		}

		if (m_fAlpha1 >= 1.f && m_bfadein1)
		{
			m_fAlpha1 = 1.f;
			m_bfadein1 = false;
		}

		if (m_fAlpha <= 0.f && m_bfadeout)
		{
			m_fAlpha = 0.f;
			m_bfadeout = false;
			m_fAlpha1 = 0;
			timer = 0.f;
			m_fDietimer = 0.f;
			m_bDeadtimeron = false;
			m_btick = false;
			if (!m_bIsbattle)
			{
				m_fFadeY = -20.f;
				m_fFade1Y = -20.f;
			}
			else
			{
				m_fFadeY = -260.f;
				m_fFade1Y = -260.f;
			}
			
		}

		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);
	}

	

}

HRESULT CUI_Dialoguepopup::Render()
{
	if (!m_btick)
		return S_OK;


	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;



	_float alpha = m_fAlpha * 0.1f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;
	m_fSize.x = 400.f;
	m_fSize.y = 40.f;
	m_fPosition.x = 190.f + m_fFadeX;
	m_fPosition.y = 680.f + m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_DIALOGUEBOX);

	m_pVIBufferCom->Render();


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 350.f;
	m_fSize.y = 5.f;
	m_fPosition.x = 200.f +m_fFadeX;
	m_fPosition.y = 700.f + m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_DIALOGUELINE);

	m_pVIBufferCom->Render();




	m_fSize.x = 72.f;
	m_fSize.y = 72.f;
	m_fPosition.x = 80.f +m_fFadeX;
	m_fPosition.y = 670.f + m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_itexnum))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();

	if (m_bseconton)
	{
		alpha = m_fAlpha1 * 0.1f;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
			return E_FAIL;
		m_fSize.x = 400.f;
		m_fSize.y = 40.f;
		m_fPosition.x = 190.f + m_fFade1X;
		m_fPosition.y = 680.f + m_fFade1Y;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_DIALOGUEBOX);

		m_pVIBufferCom->Render();


		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha1, sizeof(_float))))
			return E_FAIL;

		m_fSize.x = 350.f;
		m_fSize.y = 5.f;
		m_fPosition.x = 200.f +m_fFade1X;
		m_fPosition.y = 700.f + m_fFade1Y;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_DIALOGUELINE);

		m_pVIBufferCom->Render();


		m_fSize.x = 72.f;
		m_fSize.y = 72.f;
		m_fPosition.x = 80.f +m_fFade1X;
		m_fPosition.y = 670.f + m_fFade1Y;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_itexnum1))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();
	}
	


	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;*/

	m_fFontPos.x = 115.f + m_fFadeX;
	m_fFontPos.y = 670.f + m_fFadeY;
	m_fFontPos1.x = 115.f + m_fFade1X;
	m_fFontPos1.y = 670.f + m_fFade1Y;
//	for (_uint i = 0; i < m_vDialoguepopup[index].size(); ++i)
//	{
//CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vCurrentDialogue[m_iVectorIndex][index][i], XMVectorSet(m_fFontPos.x, m_fFontPos.y, 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha), m_fFontsize);
//	}
//CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vDialoguepopup[1][0], XMVectorSet(m_fFontPos1.x, m_fFontPos1.y, 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha1*2.f), m_FontG*(m_fAlpha1*2.f), m_FontB*(m_fAlpha1*2.f), m_fAlpha1), m_fFontsize);

	m_fFontsize = 0.7f;
for (_uint i = 0; i < m_vCurrentDialogue[m_iVectorIndex][0].size(); ++i)
{
	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vCurrentDialogue[m_iVectorIndex][0][i], XMVectorSet(m_fFontPos.x, m_fFontPos.y + (m_fFontOffsetY * (_float)i), 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha * 2.f), m_fFontsize);
}
for (_uint i = 0; i < m_vCurrentDialogue[m_iVectorIndex][1].size(); ++i)
{
	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vCurrentDialogue[m_iVectorIndex][1][i], XMVectorSet(m_fFontPos1.x, m_fFontPos1.y + (m_fFontOffsetY * (_float)i), 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha1*2.f), m_FontG*(m_fAlpha1*2.f), m_FontB*(m_fAlpha1*2.f), m_fAlpha1 * 2.f), m_fFontsize);
}





	/*m_fFontPos1.x = 500.f + m_fFade1X;
	m_fFontPos1.y = 575.f + m_fFade1Y;*/

//	Render_Fonts(m_iDialogueindex);



	return S_OK;
}

HRESULT CUI_Dialoguepopup::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dialoguebox"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sideportrait"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	/*if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_charactername"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;*/



	return S_OK;
}

HRESULT CUI_Dialoguepopup::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;*/

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fFlowCurrent, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_fFlowMAX, sizeof(_float))))
		return E_FAIL;*/


	return S_OK;
}

void CUI_Dialoguepopup::Read_TextFiles_for_dialogue()
{
	std::ifstream file("../../../Bin/Resources/popup/popup0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
	//		Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file1("../../../Bin/Resources/popup/popup1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
		//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}
	
	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialoguepopup[0]);
	matrix.push_back(m_vDialoguepopup[1]);

	m_vCurrentDialogue.push_back(matrix);

	std::ifstream file2("../../../Bin/Resources/popup/popup2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup1[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//		Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/Resources/popup/popup3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup1[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	vector<vector<_tchar*>> matrix1;
	matrix1.push_back(m_vDialoguepopup1[0]);
	matrix1.push_back(m_vDialoguepopup1[1]);

	m_vCurrentDialogue.push_back(matrix1);



}

void CUI_Dialoguepopup::Read_TextFiles_for_dialogue_first_battle()
{

	std::ifstream file("../../../Bin/Resources/popup/firstbt0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup_firstbattle[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//		Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file1("../../../Bin/Resources/popup/firstbt1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup_firstbattle[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	vector<vector<_tchar*>> matrix1;
	matrix1.push_back(m_vDialoguepopup_firstbattle[0]);
	matrix1.push_back(m_vDialoguepopup_firstbattle[1]);

	m_vCurrentDialogue.push_back(matrix1);
}

void CUI_Dialoguepopup::Read_TextFiles_for_dialogue_jelous()
{
	std::ifstream file("../../../Bin/Resources/popup/popup_jel0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup_jelous[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//		Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file1("../../../Bin/Resources/popup/popup_jel1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup_jelous[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	vector<vector<_tchar*>> matrix1;
	matrix1.push_back(m_vDialoguepopup_jelous[0]);
	matrix1.push_back(m_vDialoguepopup_jelous[1]);

	m_vCurrentDialogue.push_back(matrix1);
}

void CUI_Dialoguepopup::Read_TextFiles_for_dialougeStage2Start()
{
	std::ifstream file("../../../Bin/Resources/popup/stage2start0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup_Stage2Start1[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//		Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file1("../../../Bin/Resources/popup/stage2start1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup_Stage2Start1[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialoguepopup_Stage2Start1[0]);
	matrix.push_back(m_vDialoguepopup_Stage2Start1[1]);

	m_vCurrentDialogue.push_back(matrix);

	std::ifstream file2("../../../Bin/Resources/popup/stage2start2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup_Stage2Start2[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//		Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/Resources/popup/stage2start3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup_Stage2Start2[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	vector<vector<_tchar*>> matrix1;
	matrix1.push_back(m_vDialoguepopup_Stage2Start2[0]);
	matrix1.push_back(m_vDialoguepopup_Stage2Start2[1]);

	m_vCurrentDialogue.push_back(matrix1);
}

void CUI_Dialoguepopup::Read_TextFiles_for_dialogue_jelous2()
{
	std::ifstream file("../../../Bin/Resources/popup/popup_jel2.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup_jelous2[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//		Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file1("../../../Bin/Resources/popup/popup_jel3.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialoguepopup_jelous2[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	vector<vector<_tchar*>> matrix1;
	matrix1.push_back(m_vDialoguepopup_jelous2[0]);
	matrix1.push_back(m_vDialoguepopup_jelous2[1]);

	m_vCurrentDialogue.push_back(matrix1);
}

wchar_t * CUI_Dialoguepopup::ConverCtoWC(char * str)
{

	//wchar_t형 변수 선언
	wchar_t* pStr;
	//멀티 바이트 크기 계산 길이 반환
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	//wchar_t 메모리 할당
	pStr = new WCHAR[MAX_PATH];
	
	//형 변환
	MultiByteToWideChar(CP_ACP, 0, str, _uint(strlen(str) + 1), m_szTXT, MAX_PATH);
	Safe_Delete_Array(pStr);
	return pStr;

}

void CUI_Dialoguepopup::Open_Dialogue(_uint index , _bool tof , _uint who , _uint who1)//, _uint index1)
{
	m_iVectorIndex = index;
	m_bIsbattle = tof;
	m_btick = true;
//	m_iVectorIndex1 = index1;
	if (m_bIsbattle)
	{
		m_fFadeY = -260.f;
		m_fFade1Y = -260.f;
	}
	else
	{
		m_fFadeY = -20.f;
		m_fFade1Y = -20.f;
	}
	
	m_bfadein = true;
	m_itexnum = who;
	m_itexnum1 = who1;

	/*_uint test = 0;

	for (auto vec : (m_vCurrentDialogue[index])) test++;

	vectorsize = test;*/
}

void CUI_Dialoguepopup::Render_Fonts(_uint index)
{
	m_fFontsize = 0.6f;
	m_fFontOffsetY = 30.f;
	switch (m_vDialoguepopup[index].size())
	{
	case 0:
		return;
		break;
	case 1:
		m_fFontPos.y = 605.f + m_fFadeY;
		break;
	case 2:
		m_fFontPos.y = 590.f + m_fFadeY;
		break;
	case 3:
		m_fFontPos.y = 575.f + m_fFadeY;
		break;

	}

	for (_uint i = 0; i < m_vDialoguepopup[index].size(); ++i)
	{
		CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vDialoguepopup[index][i], XMVectorSet(m_fFontPos.x, m_fFontPos.y + (m_fFontOffsetY * (_float)i), 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha * 2.f), m_fFontsize);
	}

	/*CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vDialogue[index][0], XMVectorSet(m_fFontPos.x, m_fFontPos.y, 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha * 2.f), m_fFontsize);
	if (m_vDialogue[index].size() == 1)
	return;
	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vDialogue[index][1], XMVectorSet(m_fFontPos.x, m_fFontPos.y + 35.f, 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha * 2.f), m_fFontsize);
	if (m_vDialogue[index].size() == 2)
	return;
	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vDialogue[index][2], XMVectorSet(m_fFontPos.x, m_fFontPos.y + 70.f, 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha * 2.f), m_fFontsize);*/

}



CUI_Dialoguepopup * CUI_Dialoguepopup::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Dialoguepopup*	pInstance = new CUI_Dialoguepopup(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Dialoguepopup"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_Dialoguepopup::Clone(void * pArg)
{
	CUI_Dialoguepopup*	pInstance = new CUI_Dialoguepopup(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Dialoguepopup"));
		Safe_Release(pInstance);
	}

	CUI_Manager::Get_Instance()->Set_Dialoguepopup(pInstance);

	return pInstance;
}

void CUI_Dialoguepopup::Free()
{

	//for (int i = 0; i<m_vDialoguepopup[0].size(); i++)
	//{
	// 
	////	Safe_Delete(m_vDialoguepopup[0][i]);

	//	_tchar* temp = m_vDialoguepopup[0][i];
	//	delete[] temp;

	//}
	//for (int i = 0; i < m_vDialoguepopup[1].size(); i++)
	//{
	////	Safe_Delete(m_vDialoguepopup[1][i]);
	//	_tchar* temp = m_vDialoguepopup[1][i];
	//	delete[] temp;
	//}

	for (int i = 0; i < m_vCurrentDialogue.size(); ++i)
	{
		for (int j = 0; j <m_vCurrentDialogue[i].size(); ++j)
		{
			/*if (j == 2)
			continue;*/

			vector<_tchar*> temp = m_vCurrentDialogue[i][j];
			for (auto& iter : temp)
			{
				delete[] iter;
			}
			temp.clear();
		}


	}


	Safe_Release(m_pTextureCom1);
	__super::Free();
}


