#include "pch.h"
#include "LeverButton.h"
#include "GameInstance.h"
#include "ImguiManager.h"

CLeverButton::CLeverButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CLeverButton::CLeverButton(const CLeverButton& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CLeverButton::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLeverButton::Initialize(void* pArg)
{
	strcpy_s(m_szName, "Prototype_GameObject_LeverButton");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(CImguiManager::GetInstance()->Get_ImguiObjectTool()->Get_Mesh_Scale());

	/* 12¿ù 20ÀÏ */
	_float4x4 Result_WorldMatrix = {};
	_matrix Result_Matrix = {};

	_matrix Rotation_Matrix = CImguiManager::GetInstance()->Get_ImguiObjectTool()->Get_Picking_Mesh_Rotation_Matrix();
	_vector Translation = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	Rotation_Matrix.r[3] = { 0.f,0.f,0.f,1.f };


	Result_Matrix = XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), Rotation_Matrix);
	Result_Matrix.r[3] = Translation;
	XMStoreFloat4x4(&Result_WorldMatrix, Result_Matrix);


	m_pTransformCom->Set_WorldMatrix(Result_WorldMatrix);

	m_pModelCom->SetUp_Animation(0, true);

	return S_OK;
}

void CLeverButton::Priority_Update(_float fTimeDelta)
{
	m_pModelCom->SetUp_Animation(0, true);
}

void CLeverButton::Update(_float fTimeDelta)
{
	if (true == m_pModelCom->Play_Animation(fTimeDelta))
		int a = 10;
}

void CLeverButton::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CLeverButton::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{


		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
			return E_FAIL;*/

		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
			return E_FAIL;

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}
}

HRESULT CLeverButton::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LeverButton"),	
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLeverButton::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CLeverButton* CLeverButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLeverButton* pInstance = new CLeverButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLeverButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLeverButton::Clone(void* pArg)
{
	CLeverButton* pInstance = new CLeverButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLeverButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeverButton::Free()
{
	__super::Free();	

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);

}
