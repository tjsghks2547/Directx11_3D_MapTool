#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:
	enum STATE
	{
		STATE_IDLE = 0x00000001,
		STATE_WALK = 0x00000002,
		STATE_ATTACK = 0x00000004,
		//STATE_HI = 0x0000008,
	};

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

private:
	_uint						m_iState = {};

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END