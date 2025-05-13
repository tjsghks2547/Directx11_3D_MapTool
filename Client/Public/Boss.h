#pragma once

#include "Monster.h"

class CBoss final : public CMonster
{
private:
	CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	CBoss(const CBoss& Prototype);
	virtual ~CBoss() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:

public:
	virtual HRESULT Bind_ShaderResources();	
	virtual HRESULT Ready_Components();

public:
	static CBoss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual CGameObject* Clone(void* pArg) override;	
	virtual void Free() override;
};

