#include "pch.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Rect.h"
#include "FreeCamera.h"
#include "Terrain.h"
#include "Monster.h"

#include "Player.h"
#include "Body_Player.h"
#include "Weapon.h"

#pragma region 맵툴 오브젝트 
#include "SC_Elemental_Boss_02.h"	
#include "Temple_Ground.h"
#include "Mushroom.h"
#include "AlterBase.h"
#include "SM_TempleBrick_04.h"
#include "TempleBlcok.h"
#include "SC_Steps_A_Mod.h"
#include "SC_Corner_D_Mod.h"
#include "M_GuardianP_POSE_A_SC_Rock_01.h"
#include "M_GuardianP_POSE_A_SC_Rock_02.h"
#include "M_GuardianP_POSE_A_SC_Rock_01_Weapon.h"	
#include "SC_Brick_v1.h"	
#include "SM_TempleBrick_10.h"	
#include "SM_Brick_W_B_Mod2.h"	
#include "SM_LotusFlower_07a1.h"		
#include "ST_LiftTable_a2_sui_00.h"
#include "SM_TempleBrick_01.h"
#include "SC_Brick_W_B_Mod.h"
#include "SC_Brick_W_B_Mod_10.h"
#include "SM_Templelamp05.h"
#include "SM_Templelamp06.h"
#include "SC_FlapTrap_03.h"
#include "TurnTable_Head.h"
#include "TurnTable_Body.h"
#include "SC_Stone_HeavyBOX_A_Mod.h"
#include "Box_Lift.h"	
#include "LeverButton.h"
#include "SC_TempleDoor_05.h"
#include "SM_TempleFloor_02.h"	
#include "SC_Object_StoneBall.h"
#include "SC_SteriousTotem_Broken_G_sui.h"	
#include "SC_StepBox.h"	
#include "SC_TotemB.h"	
#include "SM_Pillar_01.h"
#include "SC_WaterTemple_Parts_b.h"
#include "SC_RuneStatue.h"
#include "SC_Elemental_Boss_04.h"
#include "SC_Elemental_Boss_05.h"
#include "Wind_EleTower_09.h"
#include "Wind_Elementak_Boss.h"
#include "Boss.h"
#include "S_HpFlower.h"
#include "SM_MushroomA_L.h"
#pragma endregion

#pragma region Water
#include "Water.h"
#pragma endregion 

#include "Sky.h"
#include "M_Guardian_Model_C.h"
#include "M_Guardian_Model_D.h"

#pragma region Npc

#include "Strong_Man.h"

#pragma endregion 	



CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pGameInstance);
}

_uint APIENTRY Thread_Main(void* pArg)
{
	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	pLoader->Start_Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{


	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	// m_eNextLevelID가 필요로하는 자원을 로드해주기위한 스레드를 만들어낸다. 
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Start_Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	CoInitializeEx(nullptr, 0);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_Level_GamePlay();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

void CLoader::SetUp_WindowText()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Loading_For_Level_Logo()
{
#pragma region 게임오브젝트 원형들 

	/* LOGO LEVEL 을 위한 자원을 로드합니다.  */

	lstrcpyW(m_szLoadingText, TEXT("로고레벨을 위한 자원을 로딩 중입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
#pragma endregion
#pragma region 컴포넌트 원형들

	/* LOGO LEVEL 을 위한 자원을 로드합니다.  */

	lstrcpyW(m_szLoadingText, TEXT("사각형 원형을 생성한다"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect")
		, CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
#pragma endregion
#pragma region 쉐이더 

	lstrcpyW(m_szLoadingText, TEXT("사각형의 셰이더를 생성한다"));

	if(FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO,TEXT("Prototype_Component_VIBuffer_Rect_Shader")
		,CShader::Create(m_pDevice,m_pContext,TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"),VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
	//여기서부터 다시 작성하기. 

#pragma endregion
#pragma region 텍스처 

	lstrcpyW(m_szLoadingText, TEXT("텍스처를 생성한다"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Splash.bmp"), 1))))
		return E_FAIL;


#pragma endregion
	/* 로딩이 완료되었습ㄴ미다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));
	m_isFinished = true;

	return S_OK;
}


HRESULT CLoader::Loading_For_Level_GamePlay()
{
	/* 게임플레이레벨을 위한 자원을 로드합니다.  */
	lstrcpyW(m_szLoadingText, TEXT("게임플레이레벨을 위한 자원을 로딩 중입니다."));		
	_matrix PreTransformMatrix = XMMatrixIdentity();
	
	

#pragma region S_HpFlower
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_MushroomA_L"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_MushroomA_L/SM_MushroomA_L.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_MushroomA_L"),
		CSM_MushroomA_L::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region S_HpFlower
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_S_HpFlower"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/S_HpFlower/S_HpFlower.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_S_HpFlower"),
		CS_HpFlower::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region Boss


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Boss/Boss.fbx", CModel::MODEL_ANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss"),	
		CBoss::Create(m_pDevice, m_pContext))))	
		return E_FAIL;


#pragma endregion 

	

#pragma region Wind_Elementak_Boss
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wind_Elementak_Boss"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/Wind_Elementak_Boss/Wind_Elementak_Boss.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Wind_Elementak_Boss"),
		CWind_Elementak_Boss::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region Wind_EleTower_09	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wind_EleTower_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/Wind_EleTower_09/Wind_EleTower_09.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Wind_EleTower_09"),	
		CWind_EleTower_09::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	
#pragma endregion 	



#pragma region SC_Elemental_Boss_04		
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Elemental_Boss_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Elemental_Boss_05/SC_Elemental_Boss_05.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Elemental_Boss_05"),
		CSC_Elemental_Boss_05::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	
#pragma endregion 	

#pragma region SC_Elemental_Boss_04		
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Elemental_Boss_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Elemental_Boss_04/SC_Elemental_Boss_04.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Elemental_Boss_04"),
		CSC_Elemental_Boss_04::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 	
	
#pragma region SC_RuneStatue	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_RuneStatue"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_RuneStatue/SC_RuneStatue.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_RuneStatue"),
		CSC_RuneStatue::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 	

	
#pragma region SC_WaterTemple_Parts_b
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_WaterTemple_Parts_b"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_WaterTemple_Parts_b/SC_WaterTemple_Parts_b.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_WaterTemple_Parts_b"),
		CSC_WaterTemple_Parts_b::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 	


#pragma region SM_Pillar_01
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Pillar_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_Pillar_01/SM_Pillar_01.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Pillar_01"),
		CSM_Pillar_01::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 	

#pragma region SC_TotemB
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_TotemB"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_TotemB/SC_TotemB.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_TotemB"),
		CSC_TotemB::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 	


#pragma region SC_StepBox
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_StepBox"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_StepBox/SC_StepBox.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_StepBox"),	
		CSC_StepBox::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 


#pragma region Strong_Man_Npc


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Strong_Man"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Npc/StrongMan/StrongMan.fbx", CModel::MODEL_ANIM))))		
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_StrongMan"),		
		CStrong_Man::Create(m_pDevice, m_pContext))))			
		return E_FAIL;		


#pragma endregion 

#pragma region SC_Object_StoneBall
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Object_StoneBall"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Object_StoneBall/SC_Object_StoneBall2.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"),
		CSC_Object_StoneBall::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region SM_TempleFloor_02
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_TempleFloor_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_TempleFloor_02/SM_TempleFloor_02.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleFloor_02"),
		CSM_TempleFloor_02::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 

#pragma region SC_TempleDoor_05

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_TempleDoor_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_TempleDoor_05/SC_TempleDoor_05.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_TempleDoor_05"),
		CSC_TempleDoor_05::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 
	

#pragma region LeverButton

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LeverButton"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/LeverButton/LeverButton.fbx", CModel::MODEL_ANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_LeverButton"),	
		CLeverButton::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	

#pragma endregion 



#pragma region Box_Lift

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Box_Lift"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/Box_Lift/Box_Lift.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Box_Lift"),	
		CBox_Lift::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	


#pragma endregion 



#pragma region SC_Stone_HeavyBOX_A_Mod

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Stone_HeavyBOX_A_Mod"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Stone_HeavyBOX_A_Mod/SC_Stone_HeavyBOX_A_Mod.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Stone_HeavyBOX_A_Mod"),	
		CSC_Stone_HeavyBOX_A_Mod::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 

#pragma region TurnTable_Body

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TurnTable_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/TurnTable_Body/TurnTable_Body.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TurnTable_Body"),	
		CTurnTable_Body::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 




#pragma region TurnTable_Head 

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TurnTable_Head"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/TurnTable_Head/TurnTable_Head.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TurnTable_Head"),
		CTurnTable_Head::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 


#pragma region SC_FlapTrap_03

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_FlapTrap_03"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_FlapTrap_03/SC_FlapTrap_03.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_FlapTrap_03"),	
		CSC_FlapTrap_03::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 


#pragma region SM_Templelamp06

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Templelamp06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_Templelamp06/SM_Templelamp06.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Templelamp06"),
		CSM_Templelamp06::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 

#pragma region SM_Templelamp05

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Templelamp05"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_Templelamp05/SM_Templelamp05.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Templelamp05"),	
		CSM_Templelamp05::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 




#pragma region M_Guardian_Model_D
	/* For.Prototype_Component_M_Guardian_Model_C */
	//PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) * XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_M_Guardian_Model_D"), 
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/M_Guardian_Model_D/M_Guardian_Model_D_real.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_Guardian_Model_D"),
		CM_Guardian_Model_D::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();	


	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_KakuWeapon"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/KakuWeapon/weapon.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
	//	return E_FAIL;
#pragma endregion 


#pragma region SC_Brick_W_B_Mod_10

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Brick_W_B_Mod_10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Brick_W_B_Mod_10/SC_Brick_W_B_Mod_10.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Brick_W_B_Mod_10"),
		CSC_Brick_W_B_Mod_10::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 



#pragma region SC_Brick_W_B_Mod

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Brick_W_B_Mod"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Brick_W_B_Mod/SC_Brick_W_B_Mod.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Brick_W_B_Mod"),
		CSC_Brick_W_B_Mod::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 



#pragma region SM_TempleBrick_01

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_TempleBrick_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_TempleBrick_01/SM_TempleBrick_01.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleBrick_01"),
		CSM_TempleBrick_01::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

#pragma region ST_LiftTable_a2_sui_00

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ST_LiftTable_a2_sui_00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/ST_LiftTable_a2_sui_00/ST_LiftTable_a2_sui_00.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"),
		CST_LiftTable_a2_sui_00::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 
#pragma region SM_LotusFlower_07a1

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_LotusFlower_07a1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_LotusFlower_07a1/SM_LotusFlower_07a1.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_LotusFlower_07a1"),
		CSM_LotusFlower_07a1::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 
#pragma region Sky

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky")	
	//	,CSky::Create(m_pDevice, m_pContext))))	
	//	return E_FAIL;	
	//
	///* For.Prototype_Component_Texture_Sky */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),	
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	//	return E_FAIL;	
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCubeSky"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeSky.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
	//	return E_FAIL;
#pragma endregion 
#pragma region Water
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Water")		
		,CWater::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	
#pragma endregion 
#pragma region SM_Brick_W_B_Mod2

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Brick_W_B_Mod2"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_Brick_W_B_Mod2/SM_Brick_W_B_Mod2.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Brick_W_B_Mod2"),	
		CSM_Brick_W_B_Mod2::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	


#pragma endregion 

#pragma region SM_TempleBrick_v10

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_TempleBrick_10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_TempleBrick_10/SM_TempleBrick_10.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleBrick_10"),	
		CSM_TempleBrick_10::Create(m_pDevice, m_pContext))))		
		return E_FAIL;			


#pragma endregion 


#pragma region SC_Brick_v1

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Brick_v1"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Brick_v1/SC_Brick_v1.fbx", CModel::MODEL_NONANIM))))	
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Brick_v1"),
		CSC_Brick_v1::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 
#pragma region M_GuardianP_POSE_A_SC_Rock_01_Weapon

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_M_GuardianP_POSE_A_SC_Rock_01_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/M_GuardianP_POSE_A_SC_Rock_01_Weapon/M_GuardianP_POSE_A_SC_Rock_01_weapon.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_01_Weapon"),
		CM_GuardianP_POSE_A_SC_Rock_01_Weapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion	
#pragma region M_GuardianP_POSE_A_SC_Rock_01

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_M_GuardianP_POSE_A_SC_Rock_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/M_GuardianP_POSE_A_SC_Rock_01/M_GuardianP_POSE_A_SC_Rock_01.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_01"),
		CM_GuardianP_POSE_A_SC_Rock_01::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion
#pragma region M_GuardianP_POSE_A_SC_Rock_02

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_M_GuardianP_POSE_A_SC_Rock_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/M_GuardianP_POSE_A_SC_Rock_02/M_GuardianP_POSE_A_SC_Rock_02.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_02"),
		CM_GuardianP_POSE_A_SC_Rock_02::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion
#pragma region SC_Corner_D_Mod
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Corner_D_Mod"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Corner_D_Mod/SC_Corner_D_Mod.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Corner_D_Mod"),
		CSC_Corner_D_Mod::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion
#pragma region SC_Steps_A_Mod

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Steps_A_Mod"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Steps_A_Mod/SC_Steps_A.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Steps_A_Mod"),
		CSC_Steps_A_Mod::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 
#pragma region Temple_Block

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Temple_Block"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/Temple_Block/Temple_Block.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Temple_Block"),	
		CTempleBlock::Create(m_pDevice, m_pContext))))	
		return E_FAIL;


#pragma endregion 
#pragma region SM_TempleBrick_04

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_TempleBrick_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_TempleBrick_04/SM_TempleBrick_04.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleBrick_04"),
		CSM_TempleBrick_04::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 
#pragma region Temple_Ground
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Temple_Ground"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/Temple_Ground/Temple_Ground.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Temple_Ground"),
		CTemple_Ground::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 
#pragma region Alter_Base

	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_AlterBase"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_AltarBase/SM_AltarBase.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_AlterBase"),
		CAlterBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion
#pragma region SC_Elemental_Boss_02	
	PreTransformMatrix = XMMatrixScaling(0.001f, 0.001f, 0.001f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Elemental_Boss_02"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_Elemental_Boss_02/SC_Elemental_Boss_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Elemental_Boss_02"),
		CSC_Elemental_Boss_02::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion
#pragma region 게임오브젝트 원형들 
	/* GAMEPLAY Level */
	lstrcpyW(m_szLoadingText, TEXT("카메라 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FreeCamera")
		, CFreeCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("테스트 렉트 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CRect"),
		CRect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("지형 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion	
#pragma region 카쿠 모델
	// 초기 설정을 위한 트랜스폼 
			
	///* For.Prototype_GameObject_Monster */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),
	//	CPlayer::Create(m_pDevice, m_pContext))))		
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Body_Player */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Body_Player"),
	//	CBody_Player::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Weapon */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Weapon"),
	//	CWeapon::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Kaku*/
	//PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Kaku"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Kaku/final2.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
	//	return E_FAIL;

	///* For.Prototype_Component_Model_Kaku_Weapon*/
	////PreTransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);	
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Kaku_Weapon"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/KakuWeapon/weapon.fbx", CModel::MODEL_NONANIM))))
	//	return E_FAIL;


#pragma endregion
#pragma region M_Guardian_Model_C
	/* For.Prototype_Component_M_Guardian_Model_C */	
	//PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) * XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/M_Guardian_Model_C/plz666.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))	
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_Guardian_Model_C"),
		CM_Guardian_Model_C::Create(m_pDevice, m_pContext))))	
		return E_FAIL;


	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_KakuWeapon"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/KakuWeapon/weapon.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
	//	return E_FAIL;
#pragma endregion 

#pragma region 버섯


	/* For.Prototype_GameObject_Mushroom */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mushroom"),
		CMushroom::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Mushroom"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Plant/Mushroom_A/mushroom.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

#pragma endregion 
#pragma region 컴포넌트 원형들 

	lstrcpyW(m_szLoadingText, TEXT("사각형 원형을 생성한다"));	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect")
		, CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 100, 100, 1, nullptr))))
		return E_FAIL;


	lstrcpyW(m_szLoadingText, TEXT("사각형 텍스쿠드2개 원형을 생성한다"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Texcood2")
		, CVIBuffer_Rect_Texcood2::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
	//lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain")
	//	, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 129, 129, 1
	//		,TEXT("../Bin/Resources/Textures/Height.bmp")))))
	//	return E_FAIL;

#pragma endregion
#pragma region 셰이더

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Shader")
		, CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Shader_Brush")
		, CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Water */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Water"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Water.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;
#pragma endregion
#pragma region 텍스처
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/SC_Snow_Texture.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Water_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Water/Water.dds"), 1))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Altar_Base_Texture"),	
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Temple/AltarBase.dds"), 1))))	
	//	return E_FAIL;	



#pragma region Collider (충돌체)

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

#pragma endregion 


#pragma region 큐브 컴포넌트
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),	
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))	
		return E_FAIL;	

	
#pragma endregion 


#pragma endregion

	/* 로딩이 완료되었습ㄴ미다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));	
	m_isFinished = true;
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);	
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
