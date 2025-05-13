#include "pch.h"
#include "TempleBlcok.h"    
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "ImguiManager.h"   


CTempleBlock::CTempleBlock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CTempleBlock::CTempleBlock(const CTempleBlock& Prototype)   
    :CGameObject(Prototype) 
{

}

HRESULT CTempleBlock::Initialize_Prototype()    
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTempleBlock::Initialize(void* pArg)
{
    strcpy_s(m_szName, "Prototype_GameObject_Temple_Block");

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(CImguiManager::GetInstance()->Get_ImguiObjectTool()->Get_Mesh_Scale());
    //m_pTransformCom->Scaling(_float3{ 0.1f,0.5f,0.1f });

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
    return S_OK;
}

void CTempleBlock::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CTempleBlock::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);
}

void CTempleBlock::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CTempleBlock::Render()
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

HRESULT CTempleBlock::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Temple_Block"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CTempleBlock::Bind_ShaderResources()
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

CTempleBlock* CTempleBlock::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTempleBlock* pInstance = new CTempleBlock(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CTemple_Block");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTempleBlock::Clone(void* pArg)
{
    CTempleBlock* pInstance = new CTempleBlock(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CTemple_Block");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTempleBlock::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
