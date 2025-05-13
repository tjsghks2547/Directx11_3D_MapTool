#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END


class CST_LiftTable_a2_sui_00 final : public CGameObject	
{
private:
	CST_LiftTable_a2_sui_00(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CST_LiftTable_a2_sui_00(const CST_LiftTable_a2_sui_00& Prototype);
	virtual ~CST_LiftTable_a2_sui_00() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CST_LiftTable_a2_sui_00* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};



