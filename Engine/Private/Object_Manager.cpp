#include "..\Public\Object_Manager.h"
#include "Layer.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Component.h"


CObject_Manager::CObject_Manager()	
	: m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pLayers = new map<const _wstring, CLayer*>[iNumLevels];

	return S_OK;
}

void CObject_Manager::Priority_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Priority_Update(fTimeDelta);		
		}
	}
}

void CObject_Manager::Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Update(fTimeDelta);
	}
}

void CObject_Manager::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Late_Update(fTimeDelta);
	}
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])	
	{
		Safe_Release(Pair.second);
	}

	m_pLayers[iLevelIndex].clear();
}

HRESULT CObject_Manager::Add_GameObject_To_Layer(_uint iPrototypeLevelIndex, const _wstring & strPrototypeTag, _uint iLevelIndex, const _wstring & strLayerTag, void * pArg)
{
	/* ���̾ �߰��� �纻��ü�� �����ؼ� �����´�. */
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;


	/* �纻��ü�� �߰��� ���̾ ã��. */
	CLayer*		pLayer = Find_Layer(iLevelIndex, strLayerTag);

	/* �纻��ü�� �߰��� ���̾ ������. \*/
	/* ������ ���̾�ü ���� �߰��ϱ����� ��ü����. */
	/* �׷��ϱ� ���̾ �����ߵȴ�. */
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	else /* �纻��ü�� �߰��� ���̾ �̹� �־���. �׷��ϱ� �� ���̾ �׳� ������ ����./ */
		pLayer->Add_GameObject(pGameObject);	

	return S_OK;
}

CComponent* CObject_Manager::Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentName)
{
	/*������Ʈ �� ���̾� ã��*/
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	/* ���̾ �ִ� ���� ������Ʈ ã��*/
	CComponent* pComponents = pLayer->Find_GameObject(strComponentName);	

	if(pComponents != nullptr)
	{

		return pComponents;
	}
	/* ���ӿ�����Ʈ �ȿ��ִ� ������Ʈ ã��*/
	
	return nullptr;
}



CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const _wstring & strLayerTag)
{
	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;	
}

CObject_Manager * CObject_Manager::Create(_uint iNumLevels)
{
	CObject_Manager*	pInstance = new CObject_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed To Created : CObject_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])		
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	Safe_Release(m_pGameInstance);
}
