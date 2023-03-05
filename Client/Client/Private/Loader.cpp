#include "stdafx.h"
#include "Loader.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Camera_Action.h"
#include "Camera_MiniGame.h"
#include "Terrain.h"
#include "Weapon.h"
#include "Sky.h"
#include "Water.h"
#include "NonAnim_Instance.h"
#include "NonAnim.h"
#include "Trigger.h"
#include "Portal.h"

//Effect & Bullet
#include "ScreenDistortion.h"
#include "EffectTexture.h"
#include "EffectMesh.h"
#include "ParticleSystem.h"
#include "Particle_Rect.h"
#include "Particle_Point.h"
#include "RinwellSkills.h"
#include "SionSkills.h"
#include "AlphenSkills.h"
#include "EffectObject.h"
#include "BossSkills.h"

//Actor
#include "Alphen.h"
#include "Sion.h"
#include "SnowFieldNpc.h"
#include "CityNpc.h"
#include "MiniGameNpc.h"
#include "BattleNpc.h"

#include "Rinwell.h"
#include "Law.h"

#include "Ice_Wolf.h"
#include "Berserker.h"
#include "Hawk.h"
#include "Slime.h"
#include "AstralDoubt.h"
#include "AiRinwell.h"
#include "MonsterLaw.h"

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
#include "UI_MENU_Back.h"
#include "UI_Dialogue.h"
#include "UI_RuneEffect.h"
#include "UI_Dialoguepopup.h"
#include "UI_InterectMsg.h"
#include "UI_Get_item_Popup.h"
#include "UI_QuestClear.h"
#include "UI_QuestStartScreen.h"
#include "UI_Questmsg.h"
#include "Itemmsgbox.h"
#include "DamageFont.h"
#include "UI_Skillmessage.h"
#include "Damagefont_Critical.h"
#include "CriticalFont.h"
#include "UI_Monster_HPbar.h"
#include "UI_BossMonster_HPbar.h"
#include "UI_BattleResult.h"
#include "UI_LevelUp.h"
#include "UI_SidePopup.h"
#include "UI_Dialogue_Caption.h"
#include "UI_AlphaScreen.h"
#include "UI_PartyjoinMsg.h"
#include "UI_JustDodgeEffect.h"
#include "UI_StrikeFinish.h"
#include "UI_SystemMessageBox.h"
#include "UI_Damagefont_Break.h"
#include "UI_Minigame1.h"
#include "UI_Minigame2.h"
#include "UI_MainPlayerMark.h"


//InteractObject
#include "TreasureBox.h"
#include "Item.h"
#include "ThrowingObject.h"

#include "ShootingObject.h"
#include "Road.h"

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
	case LEVEL_BATTLE:
		pLoader->Set_Finished(true);
		break;
	case LEVEL_SNOWFIELD:
		pLoader->Set_Finished(true);
		break;
	case LEVEL_BOSS:
		pLoader->Loading_ForBossLevel();
		break;
	case LEVEL_CITY:
		pLoader->Set_Finished(true);
		break;
	case LEVEL_RESTAURANT:
		pLoader->Loading_ForRestaurantLevel();
		break;
	case LEVEL_WORKTOOL:
		pLoader->Loading_ForWorkToolLevel();
		break;
	case LEVEL_LAWBATTLE:
		pLoader->Loading_ForLawBattleLevel();
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

	/* Static Resource */
	lstrcpy(m_szLoadingText, TEXT("Static Resource Loading"));
	if (FAILED(Loading_ForStaticLevel()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Interact Object Loading"));
	if (FAILED(Loading_ForInteract()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Static Map Object Loading"));
	if (FAILED(Loading_ForStaticMapObject()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading SnowField Object"));
	if (FAILED(Loading_ForSnowFieldLevel()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading BattleZone Object"));
	if (FAILED(Loading_ForBattleLevel()))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("Loading City Object"));
	if (FAILED(Loading_ForCityLevel()))
		return E_FAIL;

	/* Loading Prototype */
	lstrcpy(m_szLoadingText, TEXT("Loading Prototype"));
	if (FAILED(Loading_ForPrototype()))
		return E_FAIL;

	/* UI Loading */
	lstrcpy(m_szLoadingText, TEXT("UI Loading"));
	if (FAILED(Loading_ForUITexture()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Effect Loading"));
	if (FAILED(Loading_ForEffect()))
		return E_FAIL;
	if (FAILED(Loading_ForEffectTexture()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Finished Loading"));

	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForPrototype()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;

	/*For.Prototype_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectObject"),
		CEffectObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MonsterLaw"),
		CMonsterLaw::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BattleNpc"),
		CBattleNpc::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShootingObject"),
		CShootingObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Road"),
		CRoad::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MiniGameNpc"),
		CMiniGameNpc::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CityNpc"),
		CCityNpc::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_MiniGame"),
		CCamera_MiniGame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Portal */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_CameraAction */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CameraAction"),
		CCamera_Action::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Law */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Law"),
		CLaw::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Rinwell */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rinwell"),
		CRinwell::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_RinwellSkills */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RinwellSkills"),
		CRinwellSkills::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_SionSkills */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SionSkills"),
		CSionSkills::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_AlphenSkills */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AlphenSkills"),
		CAlphenSkills::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_BossSkills */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BossSkills"),
		CBossSkills::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_AiRinwell */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AiRinwell"),
		CAiRinwell::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Trigger */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SnowFieldNpc"),
		CSnowFieldNpc::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Trigger */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Trigger"),
		CTrigger::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_NonAnim */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NonAnim"),
		CNonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_NonAnim */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NonAnim_Instance"),
		CNonAnim_Instance::Create(m_pDevice, m_pContext))))
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

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MENU_BACK"),
		CUI_MENU_Back::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Dialogue"),
		CUI_Dialogue::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Dialoguepopup"),
		CUI_Dialoguepopup::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_GetITEMPOPUP"),
		CUI_Get_item_Popup::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_INTERECTMSG"),
		CUI_InterectMsg::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_QUESTSTART"),
		CUI_QuestStartScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_QUESTCLEAR"),
		CUI_QuestClear::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_QUESTMESSAGE"),
		CUI_Questmsg::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_itemmsgbox"),
		CItemmsgbox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Damagefont"),
		CDamageFont::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Damagefont_Critical"),
		CDamagefont_Critical::Create(m_pDevice, m_pContext))))
		return E_FAIL;*/

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Critical"),
		CCriticalFont::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Skillmsg"),
		CUI_Skillmessage::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MonsterHP"),
		CUI_Monster_HPbar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_BossMonsterHP"),
		CUI_BossMonster_HPbar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_BattleResult"),
		CUI_BattleResult::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Levelup"),
		CUI_LevelUp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Sidepopup"),
		CUI_SidePopup::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Dialogue_Caption"),
		CUI_Dialogue_Caption::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Alpha_Screen"),
		CUI_AlphaScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_PartyMessage"),
		CUI_PartyjoinMsg::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_JustDodgeEffect"),
		CUI_JustDodgeEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_StrikeFinish"),
		CUI_StrikeFinish::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SystemMessagebox"),
		CUI_SystemMessageBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_damagefontbreak"),
		CUI_Damagefont_Break::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_minigame1"),
		CUI_Minigame1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_minigame2"),
		CUI_Minigame2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MainPlayerMark"),
		CUI_MainPlayerMark::Create(m_pDevice, m_pContext))))
		return E_FAIL;






	/*For.Prototype_GameObject_Sky */                                
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Player*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Alphen"),
		CAlphen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Player*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sion"),
		CSion::Create(m_pDevice, m_pContext))))
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

	/*For.Prototype_GameObject_ScreenDistortion*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ScreenDistortion"),
		CScreenDistortion::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_EffectTexture*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectTexture"),
		CEffectTexture::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_EffectMesh*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectMesh"),
		CEffectMesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.GameObject_ParticleSystem*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ParticleSystem"),
		CParticleSystem::Create(m_pDevice, m_pContext))))
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

	///*For.Prototype_GameObject_Boss_AstralDoubt*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AstralDoubt"),
		CAstralDoubt::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	//InteractObject

	/*For.Prototype_GameObject_TreasureBox*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TreasureBox"),
		CTreasureBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_TreasureBox*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item"),
		CItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_ThrowingObject*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ThrowingObject"),
		CThrowingObject::Create(m_pDevice, m_pContext))))
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

	/* For.Prototype_Component_Shader_Effect*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_Effect.hlsl"), VTXTEXALPHA_DECLARATION::Elements, VTXTEXALPHA_DECLARATION::iNumElements))))
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

	/* For.Prototype_Component_Shader_VtxModelInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxModelInstance.hlsl"), VTXMODELINSTANCE_DECLARATION::Elements, VTXMODELINSTANCE_DECLARATION::iNumElements))))
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
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext))))
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
	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Terrain.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_BattleZone_Terrain"), CVIBuffer_Terrain::Create(m_pDevice, m_pContext, hFile, dwByte, true))))
		return E_FAIL;
	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	iNum = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/WaterTerrain.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_SnowField_WaterTerrain"), CVIBuffer_Terrain::Create(m_pDevice, m_pContext, hFile, dwByte, true))))
		return E_FAIL;
	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	iNum = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Terrain.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_SnowField_Terrain"), CVIBuffer_Terrain::Create(m_pDevice, m_pContext, hFile, dwByte, true))))
		return E_FAIL;
	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	iNum = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/Terrain.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_City_WaterTerrain"), CVIBuffer_Terrain::Create(m_pDevice, m_pContext, hFile, dwByte, true))))
		return E_FAIL;
	CloseHandle(hFile);

#pragma endregion Buffer Loading

#pragma region Texture Loading
	/*For.Prototype_Component_Texture_Dissolve*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Dissolve.png"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_WaterNoise*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WaterNoise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Water/PerlinNoise.png"), 1))))
		return E_FAIL;
	/*For.Prototype_Component_Texture_WaterNormal*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WaterNormal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Water/WaterDistortion.png"), 1))))
		return E_FAIL;
#pragma endregion Texture Loading

#pragma region Model Loading
	/* For.Prototype_Component_Model_Alphen */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Alphen"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Alphen/Alphen.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Sion */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Sion"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Sion/Sion.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Sion */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NpcFemaleYoung"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NpcFemaleYoung/NpcFemaleYoung.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NMM_GLD"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NMM_Gld/NPC_NMM_GLD.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NMO_DOK"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NMO_Dok/NPC_NMO_DOK.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NMY_PLC"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NMY_Plc/NPC_NMY_PLC.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Sion */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Rinwell"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Rinwell/Rinwell.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Sion */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("AIRinwell"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/AIRinwell/Rinwell.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_AiLaw */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("AILaw"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/AILaw/Law.dat"))))
		return E_FAIL;


	/* For.Prototype_Component_Model_Rinwell */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NpcRinwell"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NpcRinwell/Rinwell.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Rinwell */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NpcLaw"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NpcLaw/Law.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
 	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water_Plane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Water/Plane.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Alphen_Weapon01 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SWO2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO2(R00)/SWO2(R00).dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Sion_Weapon01 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SIOW(00)"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/SIOW(00)/SIOW(00).dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_IceWolf*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ice_Wolf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Ice_Wolf/Ice_Wolf.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Hawk*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Hawk"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Hawk/Hawk.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Berserker*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Berserker"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Berserker/Berserker.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Slime*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Slime"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Slime/Slime.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Astral_Doubbt*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Astral_Doubt"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Astral_Doubt/Astral_Doubt.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Law */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Law"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Law/Law.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NFC_SLV_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NFC_SLV_000/NPC_NFC_SLV_000.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NFC_SLV_000_2th"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NFC_SLV_000_2th/NPC_NFC_SLV_000_2th.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NFM_SLV_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NFM_SLV_000/NPC_NFM_SLV_000.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NFY_FIA_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NFY_FIA_000/NPC_NFY_FIA_000.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NMM_BEF_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NMM_BEF_000/NPC_NMM_BEF_000.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NMM_BLS_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NMM_BLS_000/NPC_NMM_BLS_000.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NMM_DIM_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NMM_DIM_000/NPC_NMM_DIM_000.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NMM_MHB_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NMM_MHB_000/NPC_NMM_MHB_000.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NMM_SLV_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NMM_SLV_000/NPC_NMM_SLV_000.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NMY_GNL"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NMY_GNL/NPC_NMY_GNL.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NPC_NFY_WAC"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NPC_NFY_WAC/NPC_NFY_WAC.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Dog"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Dog/Dog.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Duck"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Duck/Duck.dat"))))
		return E_FAIL;

#pragma endregion Model Loading

#pragma region Navigation Loading

	/* Loading Navigation */
	lstrcpy(m_szLoadingText, TEXT("Loading Navigation "));
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Battle_Navi.dat")))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SnowField_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/Field_Data/Field_Navi.dat")))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Boss_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/BattleZoneData/BossMap/Navigation.dat")))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_City_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/City_Data/Navigation.dat")))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Restaurant_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Navigation.dat")))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Restaurant_MIniGameNavigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/MIniGameNavigation.dat")))))
		return E_FAIL;


	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_WorkTool_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Navigation.dat")))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_LawBattle_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/BattleZoneData/LawBattle/Navigation.dat")))))
		return E_FAIL;

#pragma endregion Navigation Loading



	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForBattleLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);
	


#pragma region Instancing Battle


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Conifer3_BattleZone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Conifer3.dat",
			"../../../Bin/Data/BattleZoneData/SnowPlane/Conifer.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Dead_Grass_BattleZone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Dead_Grass.dat",
			"../../../Bin/Data/BattleZoneData/SnowPlane/Dead_Grass.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Dead_Tree1_BattleZone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Dead_Tree1.dat",
			"../../../Bin/Data/BattleZoneData/SnowPlane/Dead_Tree1.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bush_BattleZone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Bush.dat",
			"../../../Bin/Data/BattleZoneData/SnowPlane/Bush.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Broken_Tree_BattleZone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Broken_Tree.dat",
			"../../../Bin/Data/BattleZoneData/SnowPlane/Broken_Tree.dat"))))
		return E_FAIL;

#pragma endregion Instancing Battle


	/*For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain_SnowBattle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/SnowBattleZone/Floor%d_C.dds"), 4))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_SnowNormal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TerrainNormal_SnowBattle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/SnowBattleZone/Floor%d_N.dds"), 4))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Brush_SnowBattle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Filter_SnowBattle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/BattleZone1.bmp"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Snow */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Snow_SnowBattle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Snow/snow.png"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky_SnowBattle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/SkyBox/Sky_SpaceBlue2.dds"), 1))))
		return E_FAIL;



	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLoader::Loading_ForCityLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* Loading Texture */
	lstrcpy(m_szLoadingText, TEXT("Loading Texture"));

	/*For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky_City"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/SkyBox/City_SkyBox.dds"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain_City"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/City/Floor%d_C.dds"), 4))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NormalTerrain_City"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/City/Floor%d_N.dds"), 4))))
		return E_FAIL;

#pragma region Deco CityMap Object


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Stair01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Stair01.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("LineNew01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/LineNew01.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("LineNew02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/LineNew02.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("LineNew03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/LineNew03.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ground01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ground01.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ground02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ground02.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ground03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ground03.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ground04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ground04.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("House01_2F"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/House01_2F.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("House01_2F_1200x800"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/House01_2F_1200x800.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("house01_3F_1200x1200"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Outside/House/house01_3F_1200x1200/house01_3F_1200x1200.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bridge_base"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Bridge_base/Bridge_base.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CliffWall01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/CliffWall01.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CliffWall02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/CliffWall02.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("castleHouse"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/castleHouse.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CliffWall03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/CliffWall03.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Castle01_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Castle01_Lod1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("BigTower01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/BigTower01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("BridgeLight"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/BridgeLight.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("BigGateArch01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/BigGateArch01.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Fountain"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Fountain.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShopShader01A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/ShopShader01A.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShopShader01B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/ShopShader01B.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShopShader01C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/ShopShader01C.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MarketProps01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/MarketProps01.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MarketProps02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/MarketProps02.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MarketProps02B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/MarketProps02B.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MarketProps03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/MarketProps03.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MineProps01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/MineProps01.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MineProps02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/MineProps02.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ModeLine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/ModeLine.dat"))))
		return E_FAIL;
#pragma endregion Deco CityMap Object



#pragma region Instancing City
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CIty_BigWall"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Saternas/Big_Wall01.dat",
			"../../../Bin/Data/City_Data/BigWall.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CIty_Bench"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Saternas/Bench.dat",
			"../../../Bin/Data/City_Data/Bench.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CIty_Chair"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Saternas/Chair.dat",
			"../../../Bin/Data/City_Data/Chair.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("City_Grass0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Grass/Grass0.dat",
			"../../../Bin/Data/City_Data/Grass0.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CIty_Grass10"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Grass/Grass10.dat",
			"../../../Bin/Data/City_Data/Grass10.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("City_Ivy1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Saternas/Ivy1.dat",
			"../../../Bin/Data/City_Data/Ivy1.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("City_LightPillar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Saternas/Light01.dat",
			"../../../Bin/Data/City_Data/LightPillar.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("City_Palm"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Saternas/Palm01.dat",
			"../../../Bin/Data/City_Data/Palm01.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("City_Planter"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Saternas/Planter01.dat",
			"../../../Bin/Data/City_Data/Planter.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("City_Table"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Saternas/table01.dat",
			"../../../Bin/Data/City_Data/Table.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SAT_Wall02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Saternas/SAT_Wall02.dat",
			"../../../Bin/Data/City_Data/SAT_Wall02.dat"))))
		return E_FAIL;
#pragma endregion Instancing City




	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLoader::Loading_ForSnowFieldLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);


#pragma region Instancing SnowField
	/* Loading Instancing */
	lstrcpy(m_szLoadingText, TEXT("Loading Instancing "));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bush_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Bush.dat",
			"../../../Bin/Data/Field_Data/Bush.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Birch1_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Birch1.dat",
			"../../../Bin/Data/Field_Data/Birch1.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Birch2_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Birch2.dat",
			"../../../Bin/Data/Field_Data/Birch2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Broken_Tree_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Broken_Tree.dat",
			"../../../Bin/Data/Field_Data/Brokentree.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("BushWood_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Bld/BushWood.dat",
			"../../../Bin/Data/Field_Data/BushWood.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Conifer3_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Conifer3.dat",
			"../../../Bin/Data/Field_Data/Conifer.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Dead_Grass_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Dead_Grass.dat",
			"../../../Bin/Data/Field_Data/Dead_Grass.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Dead_Tree_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Dead_Tree1.dat",
			"../../../Bin/Data/Field_Data/Dead_Tree.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Fence_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Bld/Fence.dat",
			"../../../Bin/Data/Field_Data/Fence.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Lamppillar_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Light/StreetLight_002.dat",
			"../../../Bin/Data/Field_Data/Lamppillar.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SmallRock2_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Bld/SmallRock2.dat",
			"../../../Bin/Data/Field_Data/SmallRock2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Snow2_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow2.dat",
			"../../../Bin/Data/Field_Data/Snow.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Snow1_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow.dat",
			"../../../Bin/Data/Field_Data/Snow1.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Stalagmite5_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite5.dat",
			"../../../Bin/Data/Field_Data/Stalagmite5.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Stalagmite4_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite4.dat",
			"../../../Bin/Data/Field_Data/Stalagmite4.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Tree5_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Conifer3.dat",
			"../../../Bin/Data/Field_Data/Tree5.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Tree_SnowField"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Tree1.dat",
			"../../../Bin/Data/Field_Data/Tree.dat"))))
		return E_FAIL;

#pragma endregion Instancing SnowField


#pragma region texture
	/* Loading Texture */
	lstrcpy(m_szLoadingText, TEXT("Loading Texture"));

	/*For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/SnowField/Terrain%d_D.dds"), 4))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NormalTerrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/SnowField/Terrain%d_N.dds"), 4))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/SnowField.bmp"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Snow */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Snow/snow.png"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky_SnowField"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/SkyBox/Sky_SpaceBlue2.dds"), 1))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForRestaurantLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;
	Safe_AddRef(pGameInstance);

	/*For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESTAURANT, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/SkyBox/City_SkyBox.dds"), 1))))
		return E_FAIL;

	/* Loading Collider */
	lstrcpy(m_szLoadingText, TEXT("Loading Collider"));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESTAURANT, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESTAURANT, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_RESTAURANT, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Finished"));


	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForWorkToolLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;
	Safe_AddRef(pGameInstance);

	/*For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORKTOOL, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/SkyBox/City_SkyBox.dds"), 1))))
		return E_FAIL;

	/* Loading Collider */
	lstrcpy(m_szLoadingText, TEXT("Loading Collider"));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORKTOOL, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORKTOOL, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_WORKTOOL, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Finished"));


	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForBossLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/*For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/SkyBox/Sky_SpaceBlue2.dds"), 1))))
		return E_FAIL;

	/* Loading Collider */
	lstrcpy(m_szLoadingText, TEXT("Loading Collider"));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Finished"));


	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForLawBattleLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/*For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LAWBATTLE, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/SkyBox/Sky_SpaceBlue2.dds"), 1))))
		return E_FAIL;

	/* Loading Collider */
	lstrcpy(m_szLoadingText, TEXT("Loading Collider"));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LAWBATTLE, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LAWBATTLE, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LAWBATTLE, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Finished"));


	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForUITexture()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	/* Loading UI_Texture */
	lstrcpy(m_szLoadingText, TEXT("Loading UI_Texture "));

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
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/HPbar/HP_bar%d.dds"), 4))))
		return E_FAIL;


	/*For.gradation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/HPbar/hpgrad.dds"), 1))))
		return E_FAIL;

	/*For.hpfont */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_font"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/numberfont/tfont%d.dds"), 12))))
		return E_FAIL;

	/*For.hpfont */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_smallfont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/numberfont/smallfont%d.dds"), 11))))
		return E_FAIL;

	/*For.hpfont */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_battlefont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/numberfont/battlefont%d.dds"), 11))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_redfont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/numberfont/redfont%d.dds"), 11))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_bluefont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/numberfont/blue%d.dds"), 11))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_normalfont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/numberfont/normal%d.dds"), 11))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_yellowfont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/numberfont/yellow%d.dds"), 11))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_greenfont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/numberfont/green%d.dds"), 11))))
		return E_FAIL;




	/*For.line */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_line"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/comboline/sline%d.png"), 3))))
		return E_FAIL;

	/*For.combofont */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_hitfont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/combofonts/hitfont0.dds"), 1))))
		return E_FAIL;

	/*For.combofont */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_damagesfont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/combofonts/damagefont0.dds"), 1))))
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
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/portrait/battle_portrait_%d.dds"), 6))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portrait_Glow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/portrait/portrait_glow.png"), 1))))
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
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/charactername/charactername%d.dds"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lockon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/lockon/lockon%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LockOn_Glow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/lockon/LockOn_Glow%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENU_BACK"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/menuback/menuback%d.dds"), 14))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENU_ICON"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/menuback/menuicon%d.dds"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENU_SELECT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/menuback/menuselect%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/menuback/Inventoryback%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_name"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/itemname/itemname%d.dds"), 24))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENULINE"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/menuback/menuline%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITEMICON"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/itemicon/itemicon%d.dds"), 20))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITEMINFO"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/iteminfo/iteminfo%d.dds"), 24))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENUGALD"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/menuback/menugald%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENUBUTTON"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/skillkey/menubutton%d.dds"), 6))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENUFONT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/skillkey/menufont%d.dds"), 12))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CATEGORYNAMEFONT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/categorynamefont/categorynamefont%d.dds"), 5))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FLAG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/flags/flag0.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITEMUSEPORTRAIT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/itemuseportrait/portrait%d.dds"), 12))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LIGHTEFFECT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/lighteffect/light%d.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dialoguebox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/dialogue/maintextbox%d.dds"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sideportrait"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/dialogue/sideportrait%d.dds"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SYSTEM_MSG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/interectmsg/sysmsg%d.dds"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUEST"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/quest/quest%d.dds"), 14))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTNAME"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/quest/questname%d.dds"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTMSG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/questmsg/questmsg%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITemMsgobox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/itemmsgbox/itemmsgbox%d.dds"), 7))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skillmsg"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/skillmsg/skillmsg%d.dds"), 4))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skillname"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/skillmsg/skillname%d.dds"), 31))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTMENU"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/quest/questmenu%d.dds"), 5))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTTODO"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/quest/questtodo%d.dds"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTINFO"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/quest/questinformation%d.dds"), 2))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTDIRECTION"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/quest/questdirection%d.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SpecialDamagefont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/specialdamage/specialdamage%d.dds"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monstername"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/monstername/monstername%d.dds"), 7))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Battlestart"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/battlestart/battlestart%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Exp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/expbar/expbar%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Battleresult"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/battleresult/battleresult%d.dds"), 5))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Itemusemsg"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/skillmsg/itemuse%d.dds"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_AlphaScreen"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/alphascreen/alphascreen.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Partyjoin"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/partyjoin/partyjoin%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_dialogueportrait"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/dialogue/TalkUI_%d.dds"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_battleorder"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/battleorder/battleorder%d.dds"), 8))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_JustDodge"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/justdodge/justdodge.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Captionback"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/dialogue/Captionback.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_systemmessage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/sysmsg/sysmsg%d.dds"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_break"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/break/break.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Minigame_pointnum"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/MinigameCount/PointNum/MIniGameBlueNum_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Minigame_timenum"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/MinigameCount/TimeNum/MIniGameNum_%d.dds"), 11))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Minigame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/MinigameCount/Minigame/minigame%d.dds"), 9))))
		return E_FAIL;

	

	


	
	return S_OK;
}

HRESULT CLoader::Loading_ForEffect()
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (!pGameInstance)
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Plane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Plane/Plane.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("FloorCircle"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Plane/FloorCircle.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrossPlane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossPlane.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrossBuffer1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossBuffer1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrossBuffer2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossBuffer2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrossBuffer3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossBuffer3.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrossBuffer4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossBuffer4.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrossBuffer5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossBuffer5.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrossBuffer6"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossBuffer6.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrossFlash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossFlash.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring0.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring3.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring4.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring5.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Pipe_is_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Pipe_is_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring_hn_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_hn_03.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring_hn_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_hn_02.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring_nm_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_nm_03.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring_Ak1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_Ak1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring_Ak2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_Ak2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring_hn1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_hn1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("RingVertical"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/RingVertical.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ring_Warp"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_Warp.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cylinder0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder0.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cylinder1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cylinder2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CylinderRt1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/CylinderRt1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CylinderSO2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/CylinderSO2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cylinder_SO2_2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder_SO2_2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cylinder_sw_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder_sw_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cylinder_nm_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder_nm_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cylinder_ko_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder_ko_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CylinderSphere_nm_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/CylinderSphere_nm_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CylinderWind"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/CylinderWind.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cylinder_hn02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder_hn02.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cylinder_rt02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder_rt02.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Hurricane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/Hurricane.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Hurricane2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/Hurricane2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Hurricane3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/Hurricane3.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Hurricane4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/Hurricane4.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("waterhorn"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/waterhorn.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("waterhorn1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/waterhorn1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("waterhorn2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/waterhorn2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("FountainWater"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/FountainWater.dat"))))
		return E_FAIL;



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Akizame"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Akizame/Akizame.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Sphere0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Sphere/Sphere0.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HalfSphere_ak_04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Sphere/HalfSphere_ak_04.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spiral_hn01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_hn01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spiral_hn02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_hn02.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spiral_hn03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_hn03.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spiral_hn04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_hn04.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spiral_rm_07"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_rm_07.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spiral_is01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_is01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spiral_is05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_is05.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spiral_ny05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_ny05.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spiral_tk_05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_tk_05.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spiral_su_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_su_02.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SionStrikLaserCrossPlane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/SionStrike/SionStrikLaserCrossPlane.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SionStrikLaserCylinder"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/SionStrike/SionStrikLaserCylinder.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SionStrikLaserCylinder2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/SionStrike/SionStrikLaserCylinder2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SionStrikLaserCylinder3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/SionStrike/SionStrikLaserCylinder3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Plane_aj01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Plane/Plane_aj01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Plane_ak20"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Plane/Plane_ak20.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SparkLight01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Plane/SparkLight01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Plane_Flash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Plane/Plane_Flash.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bullet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Bullet/Bullet.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ice0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ice/Ice0.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ice1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ice/Ice1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ice2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ice/Ice2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Auratrail"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Trail/Auratrail.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Firetrail"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Trail/Firetrail.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Trail1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Trail/Trail1.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Rock1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Rock/Rock1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Rock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Rock/Rock2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder3.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder4.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder5.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder6"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder6.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder7"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder7.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder8"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder8.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder9"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder9.dat"))))
		return E_FAIL;



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HolyLanceGround"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/HolyLanceGround.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HolyLanceHalfSphere"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/HolyLanceHalfSphere.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HolyLanceQuardRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/HolyLanceQuardRing.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HolyLanceRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/HolyLanceRing.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HolyLanceTrail"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/HolyLanceTrail.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HolyLanceTube"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/HolyLanceTube.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Splash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/Splash.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Crystal_BreakPts_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/Crystal_BreakPts_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Crystal_BreakPts_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/Crystal_BreakPts_02.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Crystal_BreakPts_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/Crystal_BreakPts_03.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrystalPts_rt_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/CrystalPts_rt_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrystalPts_rt_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/CrystalPts_rt_02.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrystalPts_rt_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/CrystalPts_rt_03.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrystalPts_rt_04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/CrystalPts_rt_04.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrystalPts_rt_05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/CrystalPts_rt_05.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("FreezeLance"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ice/FreezeLance.dat"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForEffectTexture()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (!pGameInstance)
		return E_FAIL;

	/* Effect Textures */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HolyLance_Mask_04_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/HolyLance/HolyLance_Mask_04_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HolyLance_Trail01_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/HolyLance/HolyLance_Trail01_M.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HolyLanceGradient02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/HolyLance/HolyLanceGradient02.png"), 1))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Distortion_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Distortion_Noise.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Particle_Horizontal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Particle_Horizontal.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Particle_Vertical"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Particle_Vertical.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Default_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Default_Mask.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spark_00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Spark_00.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spark_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Spark_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spark_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Spark_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spark_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Spark_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Flash_00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Flash_00.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Flash_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Flash_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Line"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SpellText"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/SpellText.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SpellText_rm"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/SpellText_rm.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Aura_ak_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Aura_ak_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Aura_ak_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Aura_ak_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Aura_ak_04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Aura_ak_04.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Noise_02_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Noise_02_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Noize_03_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Noize_03_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_01_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_01_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_02_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_02_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_03_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_03_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_04_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_04_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_05_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_05_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_06_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_06_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_07_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_07_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_08_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_08_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_09_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_09_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_WaveNoise_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_WaveNoise_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Nois_hn_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Nois_hn_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Nois_hn_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Nois_hn_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Nois_hn_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Nois_hn_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Nois_hn_04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Nois_hn_04.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Nois_hn_05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Nois_hn_05.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_07_SO_AlphaC"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_07_SO_AlphaC.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_256_ny_08"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_256_ny_08.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_256_ny_10"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_256_ny_10.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_256_ny_11"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_256_ny_11.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_04.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_05.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_06"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_06.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_07"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_07.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_09"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_09.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_12"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_12.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_13"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_13.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_14"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_14.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_15"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_15.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_16"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_16.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_17"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_17.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_18"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_18.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_ny_AlphaC09"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_ny_AlphaC09.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Noise_SO_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Noise_SO_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC01_t2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC01_t2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC02_t2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC02_t2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC04.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC08"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC08.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC09"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC09.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC10"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC10.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_t2_10"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_t2_10.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Cloud_ny_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/TO14_T_FX_Cloud_ny_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_SandMist_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/TO14_T_FX_Common_SandMist_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Snow_01_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/TO14_T_FX_Common_Snow_01_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Snow_01_M_copy"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/TO14_T_FX_Common_Snow_01_M_copy.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MagumaTrail_tk_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Water/MagumaTrail_tk_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("copy_Foam2_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Water/copy_Foam2_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Warter_Splash_N_ny01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Water/Warter_Splash_N_ny01.dds"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Warter_Splash_ny01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Water/Warter_Splash_ny01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Water_rm_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Water/Water_rm_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Wave_SO_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Water/Wave_SO_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("FlareHorizon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/FlareHorizon.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("FlareHorizon_Vertical"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/FlareHorizon_Vertical.png"), 1))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Line_hn_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/Line_hn_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Line_hn_11"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/Line_hn_11.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("LineScroll_SO_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/LineScroll_SO_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("LineUnique_ym_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/LineUnique_ym_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_trail_ny_08"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/TO14_T_FX_trail_ny_08.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShockWave"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Shock/ShockWave.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShockWave2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Shock/ShockWave2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShockWave3"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Shock/ShockWave3.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShockWave4"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Shock/ShockWave4.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Flare01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Shock/Flare01.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("RockFormation0"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Particle/RockFormation0.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("RockFormation1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Particle/RockFormation1.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("RockFormation2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Particle/RockFormation2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_RadialGradient_01_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/TO14_T_FX_Common_RadialGradient_01_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_RadialGradient_inv_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/TO14_T_FX_Common_RadialGradient_inv_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_RainbowFlare01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/TO14_T_FX_Common_RainbowFlare01.dds"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_rm_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_rm_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_rm_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_rm_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_rm_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_rm_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_tk_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_tk_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_tk_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_tk_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_tk_05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_tk_05.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_tk_06"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_tk_06.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Noise_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/Noise_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Voronoi_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/Voronoi_Noise.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Gradient"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Gradient.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_circle_ny_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_circle_ny_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Gradation_ak_05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Gradation_ak_05.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Gradation_hn_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Gradation_hn_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_RadiusQuarter_tk_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_RadiusQuarter_tk_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Ring_hn_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Ring_hn_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Ring_hn_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Ring_hn_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Ring_hn_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Ring_hn_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Ring_hn_04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Ring_hn_04.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Round_SO_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Round_SO_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Round_SO_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Round_SO_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Round_SO_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Round_SO_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Round_SO_04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Round_SO_04.png"), 1))))
		return E_FAIL;



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CoreAura_rm_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Unique/CoreAura_rm_01.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CoreAura_rm_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Unique/CoreAura_rm_02.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DropMask_tk_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Unique/DropMask_tk_01.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SlashMask_tk_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Unique/SlashMask_tk_01.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Smash_rm_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Unique/Smash_rm_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Smash_rm_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Unique/Smash_rm_02.png"), 1))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("circle_ny_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/circle_ny_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Gradient_rm_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/Gradient_rm_01.dds"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Round_SO_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/Round_SO_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Round_SO_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/Round_SO_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Round_SO_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/Round_SO_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Round_SO_04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/Round_SO_04.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Round_SO_04_2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/Round_SO_04_2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Explode_Fractal01_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Explode/Explode_Fractal01_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Explode_Smoke01_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Explode/Explode_Smoke01_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("FireNoise_is_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Explode/FireNoise_is_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DustParticle_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Particle/DustParticle_M.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Smoke_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/Smoke_Mask.dds"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Smoke_Mask1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/Smoke_Mask1.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Puddle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Mask/Puddle.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrackPattern"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Crack/CrackPattern.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MagmaCracks"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Crack/MagmaCracks.dds"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Crack_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Crack/Crack_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Crack_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Crack/Crack_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Crack_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Crack/Crack_03.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height_Gradation"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Height_Gradation.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height_Gradation2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Height_Gradation2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height_Gradation3"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Height_Gradation3.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height_Gradation4"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Height_Gradation4.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height_Gradation5"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Height_Gradation5.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height_Gradation6"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Height_Gradation6.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("floormask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Mask/floormask.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_1.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_3"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_3.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_4"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_4.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_a"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_a.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_b"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_b.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_c"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_c.png"), 1))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForStaticMapObject()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;


#pragma region Deco NonAnim Object

	/*For.Prototype_Component_Model_TreasureBox02*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TreasureBox02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/TreasureBox02/TreasureBox02.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Vehicle_Boat"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Vehicle_Boat/Vehicle_Boat.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CliffRock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/CliffRock.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CliffRock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/CliffRock2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CliffRock3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/CliffRock3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CliffRock4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/CliffRock4.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MapRock1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Rock1.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MapRock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Rock2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SteelFence"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/SteelFence.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Torch"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Torch.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("WoodShielf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/WoodShielf.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("WoodShielf2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/WoodShielf2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Snow_Mountain"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Snow_Mountain/Snow_Mountain.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bag"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bag.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bag2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bag2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bag3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bag3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bale"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bale.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bale2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bale2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bale3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bale3.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Barrel"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Barrel.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Barrel3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Barrel3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Basket"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Basket.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Basket2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Basket2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Boat"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Boat.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bonfire"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bonfire.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bowl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bowl.dat"))))
		return E_FAIL;

	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bucket"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bucket.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cage"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Cage.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CampFire"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/CampFire.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Hut"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Hut.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("WoodFloor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/WoodFloor.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bridge_End"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bridge_End.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bridge_Middle"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bridge_Middle.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CommonBridge"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/CommonBridge.dat"))))
		return E_FAIL;





	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Snow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Snow2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Snow3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Stalagmite3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Stalagmite4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite4.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Stalagmite5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite5.dat"))))
		return E_FAIL;


#pragma endregion Deco NonAnim Object

#pragma region Deco BossMap Object
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ceiling"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Ceil/Ceiling/Ceiling.dat"))))
		return E_FAIL;       
	
	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bld_TilingA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Tiling/Bld_TilingA/Bld_TilingA.dat"))))
		return E_FAIL;*/

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bld_Chandelier_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Chandelier/Bld_Chandelier_Lod1/Bld_Chandelier_Lod1.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bld_Floor_A_200_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_Floor_A_200_01/Bld_Floor_A_200_01.dat"))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bld_Floor01_"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_Floor01_/Bld_Floor01_.dat"))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bld_D04_Door01_Wall_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_D04_Door01_Wall_Lod1/Bld_D04_Door01_Wall_Lod1.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bld_Floor01_outside_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_Floor01_outside_Lod1/Bld_Floor01_outside_Lod1.dat"))))
		return E_FAIL;
#pragma endregion Deco BossMap Object

#pragma region Deco Restaurant Object
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("AugustDoor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/AugustDoor.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cabinet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cabinet.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cabinet3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cabinet3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cabinet4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cabinet4.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Carpet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Carpet.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Carpet3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Carpet3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Chair04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Chair04.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Chest"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Chest.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cupboard"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Cupboard.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cupboard2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cupboard2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cupboard3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cupboard3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cupboard5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cupboard5.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Drawer"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Drawer.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Drawer2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Drawer2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("InteriorFloor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/InteriorFloor.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("InLight"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/InLight.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("napkinRed"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/napkinRed.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Painting"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Painting.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Picture"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Picture.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShopCounter"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/ShopCounter.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Skull"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Skull.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CookingTable"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/CookingTable.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bottle"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bottle.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bottle_mdl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bottle_mdl.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bottle_mdl2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bottle_mdl2.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bottle3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bottle3.dat"))))
		return E_FAIL;


	


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Fryigpan"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fryigpan.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Gourd_mdl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Gourd_mdl.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Jar_mdl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Jar_mdl.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Pitcher_mdl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Pitcher_mdl.dat"))))
		return E_FAIL;



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Potbig"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Potbig.dat"))))
		return E_FAIL;


	


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ServingPlate"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/ServingPlate.dat"))))
		return E_FAIL;

	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("tray"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/tray.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ChimneyPart"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/ChimneyPart.dat"))))
		return E_FAIL;
#pragma endregion Deco Restaurant Object

#pragma region Deco WorkTool Object
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShelfLarge"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/ShelfLarge.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Table06"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Table06.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Shelf02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Shelf02.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Door"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Door.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Banner"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Banner.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Anvil"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Anvil.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Broom"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Broom.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Furnace"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Furnace.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("FurnaceB"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/FurnaceB.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Hammer"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Hammer.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Nail"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Nail.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Pitchfork"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Pitchfork.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Rake"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Rake.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Shovel"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Shovel.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Tannery"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Tannery.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Tong"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Tong.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Bow.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("BreastPlate"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/BreastPlate.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DisPlayStand"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/DisPlayStand.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Gun"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Gun.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Halberd"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Halberd.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Helmet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Helmet.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Mace"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Helmet.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Road"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Road.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Scabbard"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Scabbard.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Shield"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Shield.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spear"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Spear.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Sword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Sword.dat"))))
		return E_FAIL;

	
	
#pragma endregion Deco WorkTool Object

#pragma region Deco LawBattle Object
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("FountainDel"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/LawBattle/FountainDel.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SkyDome"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/LawBattle/SkyDome.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("StoneBlock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/LawBattle/StoneBlock.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("StoneBlockA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/LawBattle/StoneBlockA.dat"))))
		return E_FAIL;
#pragma endregion Deco LawBattle Object



#pragma region Instancing Boss

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bld_TilingA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Tiling/Bld_TilingA/Bld_TilingA.dat",
			"../../../Bin/Data/BattleZoneData/BossMap/Tilling.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bld_Wall01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_Wall01/Bld_Wall01.dat",
			"../../../Bin/Data/BattleZoneData/BossMap/Wall.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bld_Wall01_B02_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_Wall01_B02_Lod1/Bld_Wall01_B02_Lod1.dat",
			"../../../Bin/Data/BattleZoneData/BossMap/Wall_B.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bld_WallB"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_WallB/Bld_WallB.dat",
			"../../../Bin/Data/BattleZoneData/BossMap/Wall_BB.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bld_Pillar01_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Pillar/Bld_Pillar01_Lod1/Bld_Pillar01_Lod1.dat",
			"../../../Bin/Data/BattleZoneData/BossMap/Pillar.dat"))))
		return E_FAIL;

#pragma endregion Instancing Boss

#pragma region Instancing Restuarant
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("houseFloor_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Interior/houseFloor.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/houseFloor.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("InteriorWall_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Interior/InteriorWall.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/InteriorWall.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Plate_mdl_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Plate_mdl.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Plate_mdl.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Potted_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Interior/Potted.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Potted.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("WallDeco_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Interior/Shelf03.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/WallDeco.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Vase_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Interior/Vase.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Vase.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Table_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Interior/Table07Round.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Table.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Chair_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Interior/Chair04.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Chair.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Window_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Saternas/HouseWindow.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Window.dat"))))
		return E_FAIL;






	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Apple_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Apple.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Apple.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Banana_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Banana.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Banana.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bread_French_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_French.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Bread_French.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Carrot_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Carrot.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Carrot.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Mango_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Mango.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/mango.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PineApple_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Pineapple.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/PineApple.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Potato_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Food_Potato.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Potato.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("RedOnion_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/RedOnion.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/RedOnion.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Broccoli_Restaurant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Food_Broccoli.dat",
			"../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Broccoli.dat"))))
		return E_FAIL;

#pragma endregion Instancing Restuarant

#pragma region Instancing WorkTool
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("BenchSmall_WorkTool"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Interior/BenchSmall.dat",
			"../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Chair.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Floor_WorkTool"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/WorkshopFloor.dat",
			"../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Floor.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Furnace_WorkTool"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Furnace.dat",
			"../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Furnace.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Pillow_WorkTool"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Interior/PillowBlue.dat",
			"../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Pillow.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Pillow2_WorkTool"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Interior/PillowRed.dat",
			"../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Pillow2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Railing_WorkTool"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Railing.dat",
			"../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Railing.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Rubble_WorkTool"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Floor_Rubble.dat",
			"../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Rubble.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShopCounter_WorkTool"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Interior/ShopCounter.dat",
			"../../../Bin/Data/MiniGameRoom_Data/ShootingGame/ShopCounter.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SmallRock2_WorkTool"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Bld/SmallRock2.dat",
			"../../../Bin/Data/MiniGameRoom_Data/ShootingGame/SmallRock2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Wall_WorkTool"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/WorkshopWall.dat",
			"../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Wall.dat"))))
		return E_FAIL;

#pragma endregion Instancing WorkTool

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForInteract()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;



#pragma region Interact Object 

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Box"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Box.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Box2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Box2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Box3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Box3.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Apple*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Apple"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Apple/Apple.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Lattice"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Lattice.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Egg*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Egg"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Egg/Egg.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Mushroom*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Mushroom"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Mushroom/Mushroom.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Jewel"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/ORE_003/ORE_003.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Lettuce_001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Lettuce/Lettuce_001.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Lettuce_002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Lettuce/Lettuce_002.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("GroundPlant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Plant/GroundPlant.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SlimPlant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Plant/SlimPlant.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Crystal"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Crystal/Crystal.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("GlassJar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/GlassJar.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Potion"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Potion.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cigar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cigar.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Chess"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Chess.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bread_Challah"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_Challah.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bread_Croissant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_Croissant.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bread_French"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_French.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bread_French_Slice"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_French_Slice.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bread_Loaf_Sourdough"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_Loaf_Sourdough.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bread_Muffin"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_Muffin.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Bread_Pizza"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_Pizza.dat"))))
		return E_FAIL;



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Carrot"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Carrot.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Carrot_Set"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Carrot_Set.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Cauldron"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Cauldron.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Food_Broccoli"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Food_Broccoli.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Food_Potato"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Food_Potato.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Fruit_Apple"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Apple.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Fruit_Apple_Green"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Apple_Green.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Fruit_Apple_Quarter"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Apple_Quarter.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Fruit_Apple_Red"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Apple_Red.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Fruit_Avocado"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Avocado.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Fruit_Banana"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Banana.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Fruit_Mandarin"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Mandarin.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Fruit_Mango"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Mango.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Fruit_Pineapple"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Pineapple.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Steak"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Steak.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("RedOnion"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/RedOnion.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Sausage"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Sausage.dat"))))
		return E_FAIL;
#pragma endregion Interact Object 

	RELEASE_INSTANCE(CGameInstance);

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