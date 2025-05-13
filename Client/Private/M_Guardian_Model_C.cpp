#include "pch.h"
#include "M_Guardian_Model_C.h"
#include "GameInstance.h"
#include "ImguiManager.h"

CM_Guardian_Model_C::CM_Guardian_Model_C(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice,pContext)
{
}

CM_Guardian_Model_C::CM_Guardian_Model_C(const CM_Guardian_Model_C& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CM_Guardian_Model_C::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))	
		return E_FAIL;	

	return S_OK;
}

HRESULT CM_Guardian_Model_C::Initialize(void* pArg)
{
	strcpy_s(m_szName, "Prototype_GameObject_M_Guardian_Model_C");	

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

void CM_Guardian_Model_C::Priority_Update(_float fTimeDelta)
{
	m_pModelCom->SetUp_Animation(0, true);
}

void CM_Guardian_Model_C::Update(_float fTimeDelta)
{
	if (true == m_pModelCom->Play_Animation(fTimeDelta))
		int a = 10;
}

void CM_Guardian_Model_C::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);		
}

HRESULT CM_Guardian_Model_C::Render()
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

HRESULT CM_Guardian_Model_C::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;	
}

HRESULT CM_Guardian_Model_C::Bind_ShaderResources()
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

CM_Guardian_Model_C* CM_Guardian_Model_C::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CM_Guardian_Model_C* pInstance = new CM_Guardian_Model_C(pDevice, pContext);

	if(FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CM_Guardian_Model_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CM_Guardian_Model_C::Clone(void* pArg)
{
	CM_Guardian_Model_C* pInstance = new CM_Guardian_Model_C(*this);

	if(FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CM_Guardian_Model_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CM_Guardian_Model_C::Free()
{
	__super::Free();	

}
