#include "pch.h"
#include "Water.h"
#include "GameInstance.h"
#include "ImguiManager.h"	


using namespace TriangleTests;

CWater::CWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CWater::CWater(const CWater& Prototype)
	:CGameObject(Prototype)
	, m_fTimeDelta(0.f)
{
}

HRESULT CWater::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWater::Initialize(void* pArg)
{
	strcpy_s(m_szName, "Prototype_GameObject_Water");	

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;


	m_famplitude = 0.5f;
	m_fwavelength = 3.0f;
	m_fSpeed = 0.2f;

	m_pTransformCom->Scaling(CImguiManager::GetInstance()->Get_ImguiObjectTool()->Get_Mesh_Scale());
	//m_pTransformCom->Scaling(_float3{ 0.1f,0.5f,0.1f });

	 /* 12월 20일 */
	_float4x4 Result_WorldMatrix = {};
	_matrix Result_Matrix = {};

	_matrix Rotation_Matrix = CImguiManager::GetInstance()->Get_ImguiObjectTool()->Get_Picking_Mesh_Rotation_Matrix();
	_vector Translation = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	Rotation_Matrix.r[3] = { 0.f,0.f,0.f,1.f };


	Result_Matrix = XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), Rotation_Matrix);
	Result_Matrix.r[3] = Translation;
	XMStoreFloat4x4(&Result_WorldMatrix, Result_Matrix);


	m_pTransformCom->Set_WorldMatrix(Result_WorldMatrix);


	return S_OK;
}

void CWater::Priority_Update(_float fTimeDelta)
{
	int  a = 10;
}

void CWater::Update(_float fTimeDelta)
{
	//D3D11_MAPPED_SUBRESOURCE mapped_VB_Resource; 
	//ZeroMemory(&mapped_VB_Resource, sizeof(D3D11_MAPPED_SUBRESOURCE));	
	//
	//HRESULT hr = m_pContext->Map(m_pVIBufferCom->Get_VB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_VB_Resource);
	//if (FAILED(hr))
	//{
	//	MSG_BOX("failed to Open Water Map");
	//	return;
	//}
	//
	//VTXNORTEX* pVertices = reinterpret_cast<VTXNORTEX*>(mapped_VB_Resource.pData);
	//
	//m_famplitude = 0.5f;  // 파동의 높이	
	//m_fwavelength = 5.0f; // 파동의 주기	
	//
	///**/
	//static float accumulatedTime = 0.0f; // 누적 시간	
	//accumulatedTime += fTimeDelta;	
	//float repeatTime = 6.28318f; // 2 * pi	
	////float adjustedTime = fmod(accumulatedTime * 5.f, repeatTime); // 시간을 주기적으로 반복		

	//for (size_t i = 0; i < m_pVIBufferCom->Get_NumVertices(); i++) {
	//	float waveX = m_famplitude * sin((pVertices[i].vPosition.x + accumulatedTime * 10.f) / m_fwavelength);		
	//	float waveZ = m_famplitude * cos((pVertices[i].vPosition.z + accumulatedTime * 10.f) / m_fwavelength);		
	//	pVertices[i].vPosition.y = waveX  + waveZ;	
	//}
	//m_pContext->Unmap(m_pVIBufferCom->Get_VB_Buffer(), 0);


	/*m_famplitude = 0.5f;
	m_fwavelength = 5.0f;
	m_faccumulatedTime += fTimeDelta;
	m_fSpeed = 10.f;*/


	m_faccumulatedTime += fTimeDelta;



}

void CWater::Late_Update(_float fTimeDelta)
{
	m_fTimeDelta += fTimeDelta * 0.1f;

	m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CWater::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Bind_InputAssembler();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CWater::Ready_Component()
{
	/* 버퍼 */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain")
		, TEXT("Terrain_Com"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* 셰이더 */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Water")
		, TEXT("Shader_Com"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* 텍스처 */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Water_Texture")
		, TEXT("Texture_Com"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;



	return S_OK;
}

HRESULT CWater::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;



#pragma region 빛연산 
	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	/*파도 관련*/

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAmplitude", &m_famplitude, sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTime", &m_faccumulatedTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fWavelength", &m_fwavelength, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fSpeed", &m_fSpeed, sizeof(_float))))
		return E_FAIL;

	/**/

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fTimeDelta, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;


#pragma endregion



	return S_OK;
}

CWater* CWater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 여기서 부터 다시 작성*/
	CWater* pInstance = new CWater(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWater::Clone(void* pArg)
{
	CWater* pInstance = new CWater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWater::Free()
{
	__super::Free();

	/*11월 15일 임시*/
	Safe_Release(m_pTexture2D);
	////////////////////////

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}
