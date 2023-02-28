#include "stdafx.h"
#include "..\Public\UI_Dialogue_Caption.h"


#include "GameInstance.h"
#include <fstream>
#include <iostream>
#include <string>
#include "UI_QuestClear.h"
#include "UI_Dialoguepopup.h"


CUI_Dialogue_Caption::CUI_Dialogue_Caption(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Dialogue_Caption::CUI_Dialogue_Caption(const CUI_Dialogue_Caption & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Dialogue_Caption::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Dialogue_Caption::Initialize(void * pArg)
{


	m_FontR = 0.9019607843137255f;
	m_FontG = 0.8235294117647059f;
	m_FontB = 0.6588235294117647f;
	m_fSize.x = 720.f;
	m_fSize.y = 150.f;
	m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;
	m_fFontPos.x = 500.f;
	m_fAlpha = 1.f;
	

	m_bfadein = true;
	/*CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LIFEBOTTLE, ITEMTYPE_POTION, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_POTATO, ITEMTYPE_VEGITABLE, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_DULLSWORD, ITEMTYPE_SWORD, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_PYOGOMUSHROOM, ITEMTYPE_ARMOR, false, false);*/

	Read_TextFiles_for_BossRinwell();
	Read_TextFiles_for_Strike_RinwellLaw();
	/*Read_TextFiles_for_Quest1Clear();
	Read_TextFiles_for_Quest2Strat();
	Read_TextFiles_for_Quest2Clear();
	Read_TextFiles_for_Quest3Start();
	Read_TextFiles_for_Quest3Clear();*/
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Dialogue_Caption::Tick(_float fTimeDelta)
{
	if (m_btick)
	{

		m_fTimer += fTimeDelta;


		switch (m_iVectorIndex)
		{
		case 0:
			if (m_fTimer > 4.f)
			{
				if (m_iDialogueindex < vectorsize)
				{
					++m_iDialogueindex;
					m_fTimer = 0.f;
				}

			}

			break;
		case 1:

			if (m_fTimer > 1.5f)
			{
				if (m_iDialogueindex < vectorsize)
				{
					++m_iDialogueindex;
					m_fTimer = 0.f;
				}

			}

			break;
		}
		
		
		

		if (m_iDialogueindex == vectorsize)
		{
			m_bfadeout = true;

			--m_iDialogueindex;
		}





	}

	




	return OBJ_NOEVENT;
}

void CUI_Dialogue_Caption::Late_Tick(_float fTimeDelta)
{

	
	if (m_btick)
	{
		if ( m_bfadein)
		{
			m_fAlpha = 1.f;
			m_bfadein = false;
		}

		if (m_bfadeout)
		{
			m_fAlpha = 0.f;
			m_bfadeout = false;
			m_btick = false;
			m_iDialogueindex = 0;
		}

		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);
	}



}

HRESULT CUI_Dialogue_Caption::Render()
{



	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;



	m_fSize.x = 1280;
	m_fSize.y = 720.f;
	m_fPosition.x = 640.f;
	m_fPosition.y = 360.f;// +m_fFade;;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	
	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();
	

	Render_Fonts(m_iDialogueindex);



	return S_OK;
}

HRESULT CUI_Dialogue_Caption::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Captionback"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;



	return S_OK;
}

HRESULT CUI_Dialogue_Caption::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fFlowCurrent, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_fFlowMAX, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

void CUI_Dialogue_Caption::Read_TextFiles_for_BossRinwell()
{
	std::ifstream file("../../../Bin/Resources/Caption/Linwell0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/Resources/Caption/Linwell1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue[1].push_back(pszDialog);
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
	matrix.push_back(m_vDialogue[0]);
	matrix.push_back(m_vDialogue[1]);


	m_vCurrentDialogue.push_back(matrix);


}

void CUI_Dialogue_Caption::Read_TextFiles_for_Strike_RinwellLaw()
{
	std::ifstream file("../../../Bin/Resources/Caption/RinwellLawST0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge1[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/Resources/Caption/RinwellLawST1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge1[1].push_back(pszDialog);
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

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/Resources/Caption/RinwellLawST2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge1[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/Resources/Caption/RinwellLawST3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge1[3].push_back(pszDialog);
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

	


	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialouge1[0]);
	matrix.push_back(m_vDialouge1[1]);
	matrix.push_back(m_vDialouge1[2]);
	matrix.push_back(m_vDialouge1[3]);



	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue_Caption::Read_TextFiles_for_Quest2Strat()
{
	std::ifstream file("../../../Bin/quest2start0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge2[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/quest2start1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge2[1].push_back(pszDialog);
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

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/quest2start2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge2[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/quest2start3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge2[3].push_back(pszDialog);
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



	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialouge2[0]);
	matrix.push_back(m_vDialouge2[1]);
	matrix.push_back(m_vDialouge2[2]);
	matrix.push_back(m_vDialouge2[3]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue_Caption::Read_TextFiles_for_Quest2Clear()
{
	std::ifstream file("../../../Bin/quest2clear0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge3[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/quest2clear1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge3[1].push_back(pszDialog);
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

	//m_vCurrentDialogue.




	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialouge3[0]);
	matrix.push_back(m_vDialouge3[1]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue_Caption::Read_TextFiles_for_Quest3Start()
{
	std::ifstream file("../../../Bin/quest3start0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge4[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/quest3start1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge4[1].push_back(pszDialog);
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

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/quest3start2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge4[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/quest3start3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge4[3].push_back(pszDialog);
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



	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialouge4[0]);
	matrix.push_back(m_vDialouge4[1]);
	matrix.push_back(m_vDialouge4[2]);
	matrix.push_back(m_vDialouge4[3]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue_Caption::Read_TextFiles_for_Quest3Clear()
{
	std::ifstream file("../../../Bin/quest3clear0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/quest3clear1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[1].push_back(pszDialog);
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

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/quest3clear2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/quest3clear3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[3].push_back(pszDialog);
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

	std::ifstream file4("../../../Bin/quest3clear4.txt");
	if (file4.is_open())
	{
		while (file4.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[4].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file4.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file5("../../../Bin/quest3clear5.txt");
	if (file5.is_open())
	{
		while (file5.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[5].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file5.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.








	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialouge5[0]);
	matrix.push_back(m_vDialouge5[1]);
	matrix.push_back(m_vDialouge5[2]);
	matrix.push_back(m_vDialouge5[3]);
	matrix.push_back(m_vDialouge5[4]);
	matrix.push_back(m_vDialouge5[5]);


	m_vCurrentDialogue.push_back(matrix);
}

wchar_t * CUI_Dialogue_Caption::ConverCtoWC(char * str)
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

void CUI_Dialogue_Caption::Render_Fonts(_uint index)
{
	m_fFontPos.x = 480.f;
	/*_uint test = 0;
	_uint test2 = sizeof(m_vDialogue);
	for (auto vec : m_vDialogue) test++;*/
	m_fFontsize = 1.f;
	m_fFontOffsetY = 30.f;
	switch (m_vCurrentDialogue[m_iVectorIndex][index].size())
	{
	case 0:
		return;
		break;
	case 1:
		m_fFontPos.y = 600.f; //+ m_fFade;
		break;
	case 2:
		m_fFontPos.y = 580.f;//+ m_fFade;
		break;
	case 3:
		m_fFontPos.y = 570.f;//+ m_fFade;
		break;

	}

	for (_uint i = 0; i < m_vCurrentDialogue[m_iVectorIndex][index].size() ; ++i)
	{
		CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vCurrentDialogue[m_iVectorIndex][index][i], XMVectorSet(m_fFontPos.x, m_fFontPos.y + (m_fFontOffsetY * (_float)i), 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha * 2.f), m_fFontsize);
	}


	//CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vCurrentDialogue[m_iVectorIndex][index][m_vCurrentDialogue[m_iVectorIndex][index].size() - 1], XMVectorSet(390.f, 535.f + m_fFade, 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha * 2.f), m_fFontsize);


}

void CUI_Dialogue_Caption::Open_Dialogue(_uint index)
{
	m_iVectorIndex = index; m_btick = true;

	m_bfadein = true;
	_uint test = 0;

	for (auto vec : (m_vCurrentDialogue[index])) test++;

	vectorsize = test;
}



CUI_Dialogue_Caption * CUI_Dialogue_Caption::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Dialogue_Caption*	pInstance = new CUI_Dialogue_Caption(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Dialogue_Caption"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_Dialogue_Caption::Clone(void * pArg)
{
	CUI_Dialogue_Caption*	pInstance = new CUI_Dialogue_Caption(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Dialogue_Caption"));
		Safe_Release(pInstance);
	}

	CUI_Manager::Get_Instance()->Set_DialogueCaption(pInstance);

	return pInstance;
}

void CUI_Dialogue_Caption::Free()
{

	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	//for (int j = 0; j < m_vDialogue[j].size(); ++j)
	//{
	//	for (int i = 0; i<m_vDialogue[i].size(); ++i)
	//	{
	//		//	Safe_Delete(m_vDialogue[i]);


	//		_tchar* temp = m_vDialogue[j][i];
	//		delete[] temp;

	//	}
	//}

	//
	//for (int i = 0; i < m_vDialogue[1].size(); ++i)
	//{
	////	Safe_Delete(m_vDialogue[i]);
	//	
	//	_tchar* temp = m_vDialogue[1][i];
	//	delete[] temp;
	//}
	////delete[] pszDialog;
	//_uint size = m_vCurrentDialogue[0].size();
	//size = m_vCurrentDialogue[0][0].size();
	//size = m_vCurrentDialogue[0][1].size();
	//size = m_vCurrentDialogue[1][0].size();
	//size = m_vCurrentDialogue[1][1].size();
	////size = m_vCurrentDialogue[1][1].size();

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
	//	m_vCurrentDialogue[i][j][k]




	__super::Free();


}

//}


