#include "stdafx.h"
#include "Loader.h"

#include "GameInstance.h"

#include "UI_Portrait.h"
#include "Camera_Dynamic.h"
#include "Particle_Rect.h"
#include "Particle_Point.h"
#include "UI_Screen.h"
#include "Terrain.h"
#include "Player.h"
#include "Weapon.h"
#include "Sky.h"
#include "Water.h"

//UI
#include "UI_Portrait.h"
#include "UI_Screen.h"
#include "ScreenFadeEffect.h"
#include "UI_Loading.h"
#include "HPbar.h"
#include "HP_Font.h"
#include "UI_Comboline.h"
#include "UI_Combo_font_Damages.h"
#include "UI_Combo_font_Hits.h"
#include "UI_font_Damage_number.h"
#include "UI_font_Hits_number.h"
#include "CP_Guage.h"
#include "CP_Guage_font.h"
#include "UI_MPguage.h"
#include "HPbar_World.h"
#include "HP_Font_World.h"
#include "UI_Portraitback.h"
#include "UI_Portraitfront.h"
#include "UI_Portraitfront_top.h"
#include "UI_Portraitfront_right.h"
#include "UI_Portraitfront_bottom.h"
#include "UI_Portraitfront_left.h"
#include "UI_Skillbutton.h"
#include "UI_Changebutton.h"
#include "UI_LOCKON.h"

#include "UI_RuneEffect.h"

//Monster
#include "Ice_Wolf.h"
#include "Berserker.h"
#include "Hawk.h"
#include "Slime.h"

//InteractObject
#include "TreasureBox.h"
#include "Apple.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

unsigned int APIENTRY Thread_Main(void* pArg)
{
	CLoader* pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlayLevel();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	CoInitializeEx(nullptr, 0);

	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;
		
	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;

	/* Static Resource 중. */
	lstrcpy(m_szLoadingText, TEXT("Static Resource 로딩 중."));
	if (FAILED(Loading_ForStaticLevel()))
		return E_FAIL;

	/* 객체 원형 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("객체 원형 생성 중."));
	if (FAILED(Loading_ForPrototype()))
		return E_FAIL;

	/* UI 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("UI 생성 중."));
	if (FAILED(Loading_ForUITexture()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForPrototype()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;

	/*For.Prototype_BackGround_UI */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Screen"),
		CUI_Screen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI"),
		CUI_Portrait::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ScreenFadeEffect"),
		CScreenFadeEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HPbar"),
		CHPbar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HPbar_World"),
		CHPbar_World::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_CPguage"),
		CCP_Guage::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_CPguage_font"),
		CCP_Guage_font::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MPguage"),
		CMP_Guage::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HPfont"),
		CHP_Font::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HPfont_World"),
		CHP_Font_World::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Comboline"),
		CUI_Comboline::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Damagesfont"),
		CUI_Combo_font_Damages::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_HITS_font"),
		CUI_Combo_font_Hits::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_DAMAGES_fontnum"),
		CUI_font_Damage_number::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_HITS_fontnum"),
		CUI_font_Hits_number::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Portraitback"),
		CPortraitback::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_top"),
		CUI_Portraitfront_top::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_right"),
		CUI_Portraitfront_right::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_bottom"),
		CUI_Portraitfront_bottom::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_left"),
		CUI_Portraitfront_left::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SKill_button"),
		CUI_Skillbutton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance ->Add_Prototype(TEXT("Prototype_GameObject_UI_SKill_change_button"),
		CUI_Changebutton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Rune_Effect"),
		CUI_RuneEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_LOCKON"),
		CUI_LOCKON::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Portraitfront"),
		CUI_Portraitfront::Create(m_pDevice, m_pContext))))
		return E_FAIL;*/


	/*For.Prototype_GameObject_Sky */                                
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Player*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Water*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Water"),
		CWater::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Camera_Dynamic */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		CCamera_Dynamic::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Weapon */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon"),
		CWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Particle_Rect */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Rect"),
		CParticle_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Particle_Point */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Point"),
		CParticle_Point::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//Monster//

	/*For.Prototype_GameObject_Ice_Wolf */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ice_Wolf"),
		CIce_Wolf::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Berserker*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Berserker"),
		CBerserker::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Hawk*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hawk"),
		CHawk::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Slime*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Slime"),
		CSlime::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	//InteractObject

	/*For.Prototype_GameObject_TreasureBox*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TreasureBox"),
		CTreasureBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_TreasureBox*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Apple"),
		CApple::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForStaticLevel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;
	
#pragma region Static Shader Loading
	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxRectInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxRectInstance.hlsl"), VTXRECTINSTANCE_DECLARATION::Elements, VTXRECTINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxPointInstance.hlsl"), VTXPOINTINSTANCE_DECLARATION::Elements, VTXPOINTINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCubeTexture */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeTexture"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxCubeTexture.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPoint */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPoint"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxPoint.hlsl"), VTXPOINT_DECLARATION::Elements, VTXPOINT_DECLARATION::iNumElements))))
		return E_FAIL;

#pragma endregion Static Shader Loading

#pragma region Buffer Loading
	/*For.Prototype_Component_VIBuffer_RectInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pContext, 50))))
		return E_FAIL;

	/*For.Prototype_Component_VIBuffer_PointInstance*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, 50))))
		return E_FAIL;

	/*For.Prototype_Component_VIBuffer_Point*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point"),
		CVIBuffer_Point::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/HeightMap3.bmp"), true))))
		return E_FAIL;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Terrain_Test.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	//for (_uint i = 0; i < iNum; ++i)
	//{
		//_tchar			szFullPath[MAX_PATH] = TEXT("Prototype_Component_VIBuffer_Terrain_Load");
		//if (pGameInstance->Check_Prototype(LEVEL_STATIC, szFullPath) == S_OK)
		pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_Load"), CVIBuffer_Terrain::Create(m_pDevice, m_pContext, hFile, dwByte, false));
	//}

	CloseHandle(hFile);
#pragma endregion Buffer Loading

#pragma region Model Loading
	/* For.Prototype_Component_Model_Alphen */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Alphen"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Alphen/Alphen.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water_Plane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Water/Plane.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Alphen_Weapon01 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SWO1(R00)"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/SWO1(R00)/SWO1(R00).dat"))))
		return E_FAIL;

#pragma endregion Model Loading
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐 로딩 중."));

	/*For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/MP_snow%d_D.dds"), 2))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_SnowNormal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_TerrainNormal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/MP_snow%d_N.dds"), 2))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Newfilter2.dds"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Snow */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Snow/snow.png"), 1))))
		return E_FAIL;

	

	/*For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/SkyBox/Sky_SnowDark.dds"), 1))))
		return E_FAIL;

	/* 모델 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("모델 로딩 중."));

	/*For.Prototype_Component_Model_IceWolf*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice_Wolf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Ice_Wolf/Ice_Wolf.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Berserker*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Berserker"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Berserker/Berserker.dat"))))
		return E_FAIL;
	
	/*For.Prototype_Component_Model_Hawk*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hawk"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Hawk/Hawk.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Slime*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Slime"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Slime/Slime.dat"))))
		return E_FAIL;


	/*For.Prototype_Component_Model_TreasureBox00*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TreasureBox00"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/TreasureBox00/TreasureBox00.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TreasureBox02_Blue*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TreasureBox02_Blue"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/TreasureBox02_Blue/TreasureBox02_Blue.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TreasureBox02_Red*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TreasureBox02_Red"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/TreasureBox02_Red/TreasureBox02_Red.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TreasureBox02*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TreasureBox02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/TreasureBox02/TreasureBox02.dat"))))
		return E_FAIL;

	///NON ANIM 

	/*For.Prototype_Component_Model_Apple*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Apple"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Apple/Apple.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Egg*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Egg"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Egg/Egg.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Mushroom*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Mushroom"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Mushroom/Mushroom.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_NOBJ_BAG_002*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NOBJ_BAG_002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NOBJ_BAG_002/NOBJ_BAG_002.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_NOBJ_BASK_001*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NOBJ_BASK_001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NOBJ_BASK_001/NOBJ_BASK_001.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_NOBJ_BOX_000*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NOBJ_BOX_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NOBJ_BOX_000/NOBJ_BOX_000.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_NOBJ_BOX_002*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NOBJ_BOX_002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NOBJ_BOX_002/NOBJ_BOX_002.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_ORE_001*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ORE_001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/ORE_001/ORE_001.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_ORE_002*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ORE_002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/ORE_002/ORE_002.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_ORE_003*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ORE_003"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/ORE_003/ORE_003.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Pot_001*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Pot_001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Pot_001/Pot_001.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Berserker*/
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Berserker"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Berserker/Berserker.fbx", PivotMatrix))))
	//	return E_FAIL;
	//CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Berserker"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);

	/*For.Prototype_Component_Model_Hawk*/
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hawk"),
	//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Hawk/Hawk.fbx", PivotMatrix))))
	//return E_FAIL;
	//CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Hawk"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);


	/* 콜라이더 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("콜라이더 생성 중."));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("네비게이션 생성 중"));
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/Filed_Navi.dat")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForUITexture()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("유아이 텍스쳐 로딩 중."));

	/*For.texturelogo*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Logo/logo%d.dds"), 4))))
		return E_FAIL;

	/*For.texturefade */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fade"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/fade.png"), 1))))
		return E_FAIL;

	/*For.texturelogosprite */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logosprite"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/logosprite/logosprite%d.png"), 237))))
		return E_FAIL;

	/*For.hpbar */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HPbar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/HPbar/HP_bar%d.dds"), 2))))
		return E_FAIL;


	/*For.gradation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/HPbar/hpgrad.dds"), 1))))
		return E_FAIL;

	/*For.hpfont */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_font"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/numberfont/tfont%d.dds"), 11))))
		return E_FAIL;

	/*For.line */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_line"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/comboline/sline%d.png"), 3))))
		return E_FAIL;

	/*For.combofont */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_hitfont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/combofonts/hitfont0.png"), 1))))
		return E_FAIL;

	/*For.combofont */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_damagesfont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/combofonts/damagefont0.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CPguage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/CPguage/cpguage%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MPguage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/MPguage/MPguage%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portrait_back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/portrait/portraitbase%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portrait_front"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/portrait/battle_portrait_%d.dds"), 5))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skillbutton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/skillkey/skillbutton%d.dds"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_changebutton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/skillkey/ctrlicon.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RuneEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/runeeffect/rune%d.dds"), 5))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_charactername"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/charactername/charactername%d.dds"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lockon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/lockon/lockon%d.dds"), 5))))
		return E_FAIL;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		ERR_MSG(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}