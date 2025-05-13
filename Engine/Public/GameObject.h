#pragma once

#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:

	struct GAMEOBJECT_DESC : public CTransform::TRANSFORM_DESC
	{

	};	

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	virtual HRESULT Render();

	/* 10월 31일 추가*/
	virtual HRESULT Ready_Component();

	/* 11월 11일 추가*/
	map<const wstring, class CComponent*>* Get_Components() { return &m_Components;}

	const _char* Get_Name()
	{
		return m_szName;
	}

	class CComponent* Find_Component(const _wstring& strComponentTag);	

protected:
	_char				        m_szName[MAX_PATH] = {};
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };
	class CTransform*			m_pTransformCom = { nullptr };

protected:
	map<const wstring, class CComponent*>	m_Components; //다른 게임오브젝트가 찾을 수 있으므로. 맵설정
	
protected:
	
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototype, 
		const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);
	

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END