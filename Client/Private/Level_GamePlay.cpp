#include "pch.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Rect.h"
#include "FreeCamera.h"
#include "Water.h"
#include "Sky.h"
#include "M_Guardian_Model_C.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;	

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;


	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;	

	if (FAILED(Ready_Layer_Object(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Weapon(TEXT("Layer_Weapon"))))
		return E_FAIL;	

	if (FAILED(Ready_Layer_Plants(TEXT("Layer_Plant"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
}

HRESULT CLevel_GamePlay::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar* pLayerTag)
{

	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
	//	return E_FAIL;

	return S_OK;

}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CFreeCamera::FREECAMERA_DESC Desc{};

	Desc.vEye = _float4(0.f, 0.f, -1.f, 1.f);
	Desc.vAt  = _float4(0.f, 0.f, 0.f, 1.f);
	Desc.fAspect = (float)g_iWinSizeX / (float)g_iWinSizeY;
	Desc.fFovy = XMConvertToRadians(90.0f);	
	Desc.fNear = 0.1f;	
	Desc.fFar = 1000.f;		
	Desc.fRotationPerSec = 5.f; 
	Desc.fSpeedPerSec = 50.f; 
	Desc.fSensor = 0.1f; 
	

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FreeCamera"),
		LEVEL_GAMEPLAY, TEXT("FreeCamera"), &Desc)))
		return E_FAIL; 


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Object(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC   Desc{};		

	/* 지형 */
	//if(FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY,TEXT("Prototype_GameObject_Terrain")
	//	,LEVEL_GAMEPLAY,pLayerTag,&Desc)))
	//	return E_FAIL; 

	///* 테스트 렉트 */
	//if(FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CRect"),
	//	LEVEL_GAMEPLAY, pLayerTag,&Desc)))
	//	return E_FAIL;
	//
	//
	///* 물 */
	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Water")	
	//	, LEVEL_GAMEPLAY, pLayerTag, &Desc)))		
	//	return E_FAIL;	

	/* 스카이 박스 */
	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))		
	//	return E_FAIL;		


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar* pLayerTag)
{
	//CGameObject::GAMEOBJECT_DESC   Desc{};

	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_Guardian_Model_C"), LEVEL_GAMEPLAY, pLayerTag)))	
	//	return E_FAIL;	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Weapon(const _tchar* pLayerTag)
{
	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Weapon"), LEVEL_GAMEPLAY, pLayerTag)))	
	//	return E_FAIL;	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Plants(const _tchar* pLayerTag)
{
	//if(FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mushroom"), LEVEL_GAMEPLAY, pLayerTag)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType      =  LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);	
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);	
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);	
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL; 

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_GamePlay::Free()
{
	__super::Free();

}
