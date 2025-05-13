#include "Transform.h"
#include "Shader.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent(pDevice,pContext)
{
  
}


void CTransform::Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ)
{
    Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * fScaleX);
    Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * fScaleY);
    Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * fScaleZ);
}

HRESULT CTransform::Initialize_Prototype()
{
    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());    

    return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
    /* 그럼 여기서 fSpeedPerSec와 fRotationPerSec 값을 적용시킨다
     뭘로? -> pArg값을 받아와서 
     pArg값은 transform 헤더에 선언된 구조체 형태 아니면 
     Gaemobejct에서 관리할듯 */

    TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);
   

    if (nullptr == pDesc)
    {
        return S_OK;    
    }
       
    m_fSpeedPerSec    = pDesc->fSpeedPerSec; 
    m_fRotationPerSec = pDesc->fRotationPerSec;
    m_fPosition       = pDesc->fPostion;
 
    
    
    Set_State(STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_fPosition), 1.f));       
    
    return S_OK;
}

HRESULT CTransform::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
    pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);

    return S_OK;
}

_matrix CTransform::Get_RotationMatrixFromWorld(const _matrix& worldMatrix)
{
   // 월드 행렬의 각 축 벡터 추출
   _vector right = worldMatrix.r[0]; // Right 벡터
   _vector up = worldMatrix.r[1];    // Up 벡터
   _vector look = worldMatrix.r[2];  // Look 벡터

   // 각 축 벡터를 정규화하여 스케일 제거
   right = XMVector3Normalize(right);
   up = XMVector3Normalize(up);
   look = XMVector3Normalize(look);

   // 회전 행렬 생성
   _matrix rotationMatrix;
   rotationMatrix.r[0] = right; // 정규화된 Right 벡터
   rotationMatrix.r[1] = up;    // 정규화된 Up 벡터
   rotationMatrix.r[2] = look;  // 정규화된 Look 벡터
   rotationMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f); // 위치 제거

   return rotationMatrix;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
    /* 현재 위치 구하기*/
    _vector vPosition = Get_State(STATE_POSITION);
    _vector vLook = Get_State(STATE_LOOK);

    /* Look vector normalize 해주기*/
    vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;   
    
    Set_State(STATE_POSITION, vPosition);

}

void CTransform::Go_BackWard(_float fTimeDelta)
{
    /* 현재 위치 구하기*/
    _vector vPosition = Get_State(STATE_POSITION);  
    _vector vLook = Get_State(STATE_LOOK);  


    vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;    

    Set_State(STATE_POSITION, vPosition);   
}

void CTransform::Go_Left(_float fTimeDelta)
{
    /* 현재 위치 구하기*/
    _vector vPosition = Get_State(STATE_POSITION);
    _vector vRight = Get_State(STATE_RIGHT);


    vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

    Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
    /* 현재 위치 구하기*/
    _vector vPosition = Get_State(STATE_POSITION);  
    _vector vRight = Get_State(STATE_RIGHT);    


    vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

    Set_State(STATE_POSITION, vPosition);   
}

void CTransform::Turn(_vector vAxis, _float fTimeDelta)
{
    _vector vRight = Get_State(STATE_RIGHT);
    _vector vUp    = Get_State(STATE_UP);
    _vector vLook  = Get_State(STATE_LOOK);

    /*이게 하나의 축으로 회전하는거니깐 짐벌락 현상 없앤듯?*/

    if(m_fRotationPerSec == 0.0f)
    {
        m_fRotationPerSec = 5.f;
    }

    _matrix RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

    
    
    vRight = XMVector3TransformNormal(vRight, RotationMatrix);
    vUp    = XMVector3TransformNormal(vUp, RotationMatrix);
    vLook  = XMVector3TransformNormal(vLook, RotationMatrix);
    
    Set_State(STATE_RIGHT,vRight);
    Set_State(STATE_UP, vUp);
    Set_State(STATE_LOOK, vLook);

}

void CTransform::Turn_X(_float _fDegree)
{

    _vector vRight = Get_State(STATE_RIGHT);
    _vector vUp = Get_State(STATE_UP);
    _vector vLook = Get_State(STATE_LOOK);

    /*이게 하나의 축으로 회전하는거니깐 짐벌락 현상 없앤듯?*/

    _matrix RotationMatrix = XMMatrixRotationX(XMConvertToRadians(_fDegree));    


    vRight = XMVector3TransformNormal(vRight, RotationMatrix);
    vUp = XMVector3TransformNormal(vUp, RotationMatrix);
    vLook = XMVector3TransformNormal(vLook, RotationMatrix);

    Set_State(STATE_RIGHT, vRight);
    Set_State(STATE_UP, vUp);
    Set_State(STATE_LOOK, vLook);
}

void CTransform::Turn_Y(_float _fDegree)
{

    _vector vRight = Get_State(STATE_RIGHT);
    _vector vUp = Get_State(STATE_UP);
    _vector vLook = Get_State(STATE_LOOK);

    /*이게 하나의 축으로 회전하는거니깐 짐벌락 현상 없앤듯?*/

    _matrix RotationMatrix = XMMatrixRotationY(XMConvertToRadians(_fDegree));


    vRight = XMVector3TransformNormal(vRight, RotationMatrix);
    vUp = XMVector3TransformNormal(vUp, RotationMatrix);
    vLook = XMVector3TransformNormal(vLook, RotationMatrix);

    Set_State(STATE_RIGHT, vRight);
    Set_State(STATE_UP, vUp);
    Set_State(STATE_LOOK, vLook);
}

void CTransform::Turn_Z(_float _fDegree)
{

    _vector vRight = Get_State(STATE_RIGHT);    
    _vector vUp = Get_State(STATE_UP);
    _vector vLook = Get_State(STATE_LOOK);

    /*이게 하나의 축으로 회전하는거니깐 짐벌락 현상 없앤듯?*/

    _matrix RotationMatrix = XMMatrixRotationZ(XMConvertToRadians(_fDegree));   


    vRight = XMVector3TransformNormal(vRight, RotationMatrix);
    vUp = XMVector3TransformNormal(vUp, RotationMatrix);
    vLook = XMVector3TransformNormal(vLook, RotationMatrix);

    Set_State(STATE_RIGHT, vRight);
    Set_State(STATE_UP, vUp);
    Set_State(STATE_LOOK, vLook);
}



void CTransform::LookAt(_fvector vAt)
{
    _float3 vScale = Compute_Scaled();  

    _vector vLook  = vAt - Get_State(STATE_POSITION);
    _vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
    _vector vUp = XMVector3Cross(vLook, vRight);


    Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
    Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
    Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
    
   
}

_float3 CTransform::Compute_Scaled()
{
    /* 여기서 왜 X요소만 Get하냐면 XMVector3Length의 반환타입은
    XMVector이다 그래서 만약 길이가 2이면 (2,2,2,2)로 리턴해주기 때문에
    그 중 하나의 요소만 쓰기 위해 XMVectorGetX을 사용했다.*/
    return _float3(
        XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
        XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
        XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK)))
    );
}

void CTransform::Scaling(const _float3& vScale) 
{
    Set_State(STATE::STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * vScale.x);
    Set_State(STATE::STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * vScale.y);
    Set_State(STATE::STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * vScale.z);
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTransform* pInstance = new CTransform(pDevice, pContext);

    if(FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CTransform");
        Safe_Release(pInstance);
    }

    return pInstance;
}

//CComponent* CTransform::Clone(void* pArg)
//{
//    CTransform* pInstance = new CTransform(*this);
//    
//    if(FAILED(pInstance->Initialize(pArg)))
//    {
//        MSG_BOX("Failed to Cloned : CTransform");
//        Safe_Release(pInstance);    
//    }
//
//    return pInstance;
//}

void CTransform::Free()
{
    __super::Free();    
    

}
