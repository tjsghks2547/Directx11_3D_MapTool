#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END


class CSC_Object_StoneBall final : public CGameObject
{
private:
	CSC_Object_StoneBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSC_Object_StoneBall(const CSC_Object_StoneBall& Prototype);
	virtual ~CSC_Object_StoneBall() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel*  m_pModelCom  = { nullptr };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CSC_Object_StoneBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};



