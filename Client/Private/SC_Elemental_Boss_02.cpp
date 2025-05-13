#include "pch.h"
#include "SC_Elemental_Boss_02.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"


CSC_Elemental_Boss_02::CSC_Elemental_Boss_02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CSC_Elemental_Boss_02::CSC_Elemental_Boss_02(const CSC_Elemental_Boss_02& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT CSC_Elemental_Boss_02::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSC_Elemental_Boss_02::Initialize(void* pArg)
{
    strcpy_s(m_szName, "Prototype_GameObject_SC_Elemental_Boss_02");    

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    //m_pTransformCom->Scaling(_float3{ 0.001f,0.001f,0.001f });

    return S_OK;
}

void CSC_Elemental_Boss_02::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CSC_Elemental_Boss_02::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);    
}

void CSC_Elemental_Boss_02::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this); 
}

HRESULT CSC_Elemental_Boss_02::Render()
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

HRESULT CSC_Elemental_Boss_02::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Elemental_Boss_02"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CSC_Elemental_Boss_02::Bind_ShaderResources()
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

CSC_Elemental_Boss_02* CSC_Elemental_Boss_02::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSC_Elemental_Boss_02* pInstance = new CSC_Elemental_Boss_02(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : SC_Elemental_Boss_02");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSC_Elemental_Boss_02::Clone(void* pArg)
{
    CSC_Elemental_Boss_02* pInstance = new CSC_Elemental_Boss_02(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : SC_Elemental_Boss_02");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSC_Elemental_Boss_02::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
