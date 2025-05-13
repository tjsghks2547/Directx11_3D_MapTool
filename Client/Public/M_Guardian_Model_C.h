#pragma once

#include "Monster.h"

class CM_Guardian_Model_C final : public CMonster
{
private:
	CM_Guardian_Model_C(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CM_Guardian_Model_C(const CM_Guardian_Model_C& Prototype);
	virtual ~CM_Guardian_Model_C() = default; 

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
	static CM_Guardian_Model_C* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual CGameObject* Clone(void* pArg) override;	
	virtual void Free() override;
};

