
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END



class CSC_StepBox final : public CGameObject
{
private:
	CSC_StepBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSC_StepBox(const CSC_StepBox& Prototype);
	virtual ~CSC_StepBox() = default;

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
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Ready_Components();

public:
	static CSC_StepBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;	
	virtual void Free() override;	
};





