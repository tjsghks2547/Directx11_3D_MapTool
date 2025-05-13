#include "pch.h"
#include "M_GuardianP_POSE_A_SC_Rock_02.h"    
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "ImguiManager.h"   


CM_GuardianP_POSE_A_SC_Rock_02::CM_GuardianP_POSE_A_SC_Rock_02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CM_GuardianP_POSE_A_SC_Rock_02::CM_GuardianP_POSE_A_SC_Rock_02(const CM_GuardianP_POSE_A_SC_Rock_02& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT CM_GuardianP_POSE_A_SC_Rock_02::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


}

HRESULT CM_GuardianP_POSE_A_SC_Rock_02::Initialize(void* pArg)
{
    strcpy_s(m_szName, "Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_02");

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(CImguiManager::GetInstance()->Get_ImguiObjectTool()->Get_Mesh_Scale());
    //m_pTransformCom->Scaling(_float3{ 0.1f,0.5f,0.1f });

    return S_OK;
}

void CM_GuardianP_POSE_A_SC_Rock_02::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CM_GuardianP_POSE_A_SC_Rock_02::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);
}

void CM_GuardianP_POSE_A_SC_Rock_02::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CM_GuardianP_POSE_A_SC_Rock_02::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint  iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        /*if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
            return E_FAIL;*/

        m_pShaderCom->Begin(0);

        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CM_GuardianP_POSE_A_SC_Rock_02::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_M_GuardianP_POSE_A_SC_Rock_02"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CM_GuardianP_POSE_A_SC_Rock_02::Bind_ShaderResources()
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

CM_GuardianP_POSE_A_SC_Rock_02* CM_GuardianP_POSE_A_SC_Rock_02::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CM_GuardianP_POSE_A_SC_Rock_02* pInstance = new CM_GuardianP_POSE_A_SC_Rock_02(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CM_GuardianP_POSE_A_SC_Rock_02");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CM_GuardianP_POSE_A_SC_Rock_02::Clone(void* pArg)
{
    CM_GuardianP_POSE_A_SC_Rock_02* pInstance = new CM_GuardianP_POSE_A_SC_Rock_02(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CM_GuardianP_POSE_A_SC_Rock_02");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CM_GuardianP_POSE_A_SC_Rock_02::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
