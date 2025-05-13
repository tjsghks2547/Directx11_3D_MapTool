#include "pch.h"	
#include "Player.h"
#include "GameInstance.h"
#include "Body_Player.h"
#include "Weapon.h"


CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject(pDevice, pContext)
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	:CContainerObject(Prototype)
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;



	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};

	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;


	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{

	//if (GetKeyState(VK_DOWN) & 0x8000)
	//{
	//	m_pTransformCom->Go_BackWard(fTimeDelta);	
	//	//m_iState = STATE_IDLE;	
	//}
	//
	//if (GetKeyState(VK_LEFT) & 0x8000)
	//{
	//	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * -1.f);
	//}
	//
	//if (GetKeyState(VK_RIGHT) & 0x8000)
	//{
	//	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	//}
	//
	//if (GetKeyState(VK_UP) & 0x8000)
	//{
	//	//m_pTransformCom->Go_Straight(fTimeDelta);
	//
	//	m_iState = STATE_WALK;
	//}
	//else
	//{
	//	if (m_iState & STATE_WALK)
	//	{
	//		m_iState ^= STATE_WALK;
	//		m_iState |= STATE_IDLE;
	//	}
	//}

	__super::Priority_Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPlayer::Render()
{
	return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody_Player::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentState = &m_iState;
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.fSpeedPerSec = 0.f;
	BodyDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Body_Player"), &BodyDesc)))
		return E_FAIL;

	/*CModel*	pBodyModelCom = dynamic_cast<CModel*>
		(m_PartObjects[TEXT("Part_Body")]->Find_Component(TEXT("Com_Model")));*/
		/* 위의 코드는 만약 Part_Body가 없으면 맵컨테이너에서 추가해서 집어넣기 때문에 디버깅이 안됨 따라서 안좋음 */

			/* 무기를 만든다ㅏ. */
	CWeapon::WEAPON_DESC		WeaponDesc{};

	CModel* pBodyModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body"), TEXT("Com_Model")));
	if (nullptr == pBodyModelCom)
		return E_FAIL;

	WeaponDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("DM_L_Hand"); /* 캐릭터 모델마다 다름 */
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	WeaponDesc.fSpeedPerSec = 0.f;
	WeaponDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Weapon"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Weapon"), &WeaponDesc)))
		return E_FAIL;

	/* 이펙트를 추가하나다. */

	/* 유아이르르 추가하한다. */
	return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
}
