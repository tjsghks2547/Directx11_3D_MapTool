#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END


class CSC_SteriousTotem_Broken_G_sui final : public CGameObject
{
private:
	CSC_SteriousTotem_Broken_G_sui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSC_SteriousTotem_Broken_G_sui(const  CSC_SteriousTotem_Broken_G_sui& Prototype);
	virtual ~CSC_SteriousTotem_Broken_G_sui() = default;
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
	static  CSC_SteriousTotem_Broken_G_sui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};



