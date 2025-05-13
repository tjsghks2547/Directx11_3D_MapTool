#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END


class CSM_Brick_W_B_Mod2 final : public CGameObject
{
private:
	CSM_Brick_W_B_Mod2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSM_Brick_W_B_Mod2(const CSM_Brick_W_B_Mod2& Prototype);
	virtual ~CSM_Brick_W_B_Mod2() = default;
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
	static CSM_Brick_W_B_Mod2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};



