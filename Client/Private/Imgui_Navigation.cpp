#include "pch.h"
#include "Imgui_Navigation.h"
#include "Imgui_ObjectTool.h"
#include "Navigation.h"
#include "Collider.h"   
#include "GameInstance.h"
#include "Cell.h"
#include "Cube.h"
#include "VIBuffer_Cell.h"

using namespace TriangleTests;

bool EnsureClockwiseOrder(XMVECTOR& A, XMVECTOR& B, XMVECTOR& C, const XMVECTOR& cameraDirection) 
{
    // 벡터 계산
    XMVECTOR AB = XMVectorSubtract(B, A);
    XMVECTOR AC = XMVectorSubtract(C, A);

    // 법선 벡터 계산
    XMVECTOR normal = XMVector3Cross(AB, AC);

    // 법선 벡터와 카메라 방향의 내적
    float dot = XMVectorGetX(XMVector3Dot(normal, cameraDirection));

    // 반시계 방향이면 B와 C를 스왑하여 시계방향으로 변경
    if (dot < 0) {
        //std::swap(B, C);
        return false;
    }

    return true; 
}


CImgui_Navigation::CImgui_Navigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CImgui_ObjectTool* pObjectTool)
    :m_pDevice(pDevice)
    ,m_pContext(pContext)
    ,m_pObjectTool(pObjectTool)
    ,m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pObjectTool);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CImgui_Navigation::Initialize() 
{
    /* 여기서 기초 작업 */
    m_pNavigation = CNavigation::Create(m_pDevice, m_pContext);
    //m_pNavigation->Initialize();
  


    return S_OK;
}

void CImgui_Navigation::Update()
{
    _float3 mouseRayPos = m_pGameInstance->Get_WorldRayPos();   
    _float3 mouseRayDir = m_pGameInstance->Get_WorldRayDir();   

    FXMVECTOR mousePos = { mouseRayPos.x,mouseRayPos.y,mouseRayPos.z,1.f }; 
    FXMVECTOR mouseRay = { mouseRayDir.x,mouseRayDir.y,mouseRayDir.z,0.f }; 

    /* 여기서 창관리 */
    if (ImGui::BeginMenuBar())  
    {
        if (ImGui::BeginMenu("Navigation")) 
        {
            if (ImGui::MenuItem("Navigation Tool", nullptr)) 
            {
            
                m_bNavigationTool = true; 
                
            }
  
            ImGui::EndMenu();

        }
        ImGui::EndMenuBar();
    }

    

#pragma region 네비게이션 툴 창

    if(m_bNavigationTool)
    {
        if (ImGui::Begin("Navigation Tool Window",&m_bNavigationTool))
        {
            if (m_pObjectTool->Get_Picking_State() && ImGui::IsMouseReleased(0) && !m_bTransformMesh) // 이거 지금 연속으로 들어오네;
            {

                /* 이거 조건하나 더 필요할거 같음 (매쉬클릭하고 싶을때 콜리더 박스와 레이가 충돌하면 이게됨 )*/
                /* 수정해야함. */
                if(ChecKIntersect())
                {
                    int a = 4; 
                }

                else
                {
                    m_vecPickingPoint.push_back(m_pObjectTool->Get_MeshPicking_Point());    
                }
     
            }

            ImGui::SameLine(); // 버튼을 같은 줄에 배치


        }

        if (m_vecPickingPoint.size() > 0)
        {
            ImGui::Text("Number : %f", m_vecPickingPoint.back().x);
            ImGui::Text("Number : %f", m_vecPickingPoint.back().y);
            ImGui::Text("Number : %f", m_vecPickingPoint.back().z);
        }

        ImGui::Checkbox("Delete_Cell",    &m_bDeleteCellTool);
        ImGui::Checkbox("Delete_Cube",    &m_bDeleteCube);
        ImGui::Checkbox("Transform_Mesh", &m_bTransformMesh);
        ImGui::Checkbox("WireFrameCellOnOff", &m_bWireFrameCellOnOff);
        ImGui::Checkbox("WireFrameCubeOnOff", &m_bWireFrameCubeOnOff);

        ImGui::End();

    }
#pragma endregion 


#pragma region 네비게이션 Cell 제작 
    /* */
    if (m_vecPickingPoint.size() == 3)
    {
        if (m_vecPickingPoint.size() % 3 == 0 && m_vecPickingPoint.size() != 0)
        {

            /* true가 반환되면 */
                if(EnsureClockwiseOrder(XMVectorSetW(XMLoadFloat3(&m_vecPickingPoint[0]), 1.f)
                , XMVectorSetW(XMLoadFloat3(&m_vecPickingPoint[1]), 1.f)
                , XMVectorSetW(XMLoadFloat3(&m_vecPickingPoint[2]), 1.f)
                , mouseRay))
            {
                _float3  point1 = m_vecPickingPoint[1];
                _float3  point2 = m_vecPickingPoint[2];

                m_vecPickingPoint[2] = point1;
                m_vecPickingPoint[1] = point2;
            }

            
            m_pNavigation->Add_Cell(&(m_vecPickingPoint.front()));
            /* 여기서 그러면 구충돌 구를 줘야겠다.*/
           
            m_vecPickingPoint.clear();
        }
    }
#pragma endregion 
    /* 여기서 일단 구 충돌 검사를 해야할거같음 */


#pragma region 셀(Cell) 삭제 기능

    if(m_bDeleteCellTool)
    {
        if(ImGui::IsMouseReleased(0))
        {
            
            for(auto& iter : m_pNavigation->Get_VecCell())
            {
                IntersectCell(iter);    
            }

            if(m_mapCell.size() > 0)
            {
                for(auto pCells = m_pNavigation->Get_VecCell().begin(); pCells != m_pNavigation->Get_VecCell().end(); ++pCells)
                {
                    if (m_mapCell.begin()->second == *pCells)    
                    {
                        m_pNavigation->Get_VecCell().erase(pCells); 
                        break;  
                    }
                }

            }

            for (auto& iter : m_mapCell)
                Safe_Release(iter.second);

            m_mapCell.clear();
        }

    }
#pragma endregion 


#pragma region 큐브(Cube) 삭제 기능 
    if(m_bDeleteCube)
    {
        if(ImGui::IsMouseReleased(0))
        {
            for (auto& iter : m_pNavigation->Get_VecCube()) 
            {
                IntersectRay(iter);     
            }       


            if(m_mapCube.size() > 0)
            {
                for (auto pCube = m_pNavigation->Get_VecCube().begin(); pCube != m_pNavigation->Get_VecCube().end(); pCube++)
                {
                    if (m_mapCube.begin()->second == *pCube)
                    {
                        m_pNavigation->Get_VecCube().erase(pCube);
                        break;
                    }
                }

                
            }

     

            for (auto& iter : m_mapCube)    
                Safe_Release(iter.second);  

            m_mapCube.clear();
        }
    }

#pragma endregion 
     
#pragma region 매쉬 이동 기능
    if(m_bTransformMesh)
    {
        if (ImGui::IsMouseReleased(0) && m_pCurrentSelectedPoint == nullptr)    
        {
            FindPoint(m_pObjectTool->Get_MeshPicking_Point());
        }

        if(m_pCurrentSelectedPoint)   
        {
            if (GetAsyncKeyState(VK_UP) & 0x8000)
            {
                m_pCurrentSelectedPoint->vPosition.z += 0.1f;
                m_pCurrentSelectedCube->Get_WorldMatrix()->_43 += 0.1f; 
            }
            if (GetAsyncKeyState(VK_DOWN) & 0x8000)
            {
                m_pCurrentSelectedPoint->vPosition.z -= 0.1f;
                m_pCurrentSelectedCube->Get_WorldMatrix()->_43 -= 0.1f;
            }

            if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            {
                m_pCurrentSelectedPoint->vPosition.x += 0.1f;
                m_pCurrentSelectedCube->Get_WorldMatrix()->_41 += 0.1f;
            }
            if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            {
                m_pCurrentSelectedPoint->vPosition.x -= 0.1f;
                m_pCurrentSelectedCube->Get_WorldMatrix()->_41 -= 0.1f; 
            }


            if (GetAsyncKeyState('O') & 0x8000)
            {
                m_pCurrentSelectedPoint->vPosition.y += 0.1f;
                m_pCurrentSelectedCube->Get_WorldMatrix()->_42 += 0.1f;
            }
            if (GetAsyncKeyState('P') & 0x8000)
            {
                m_pCurrentSelectedPoint->vPosition.y -= 0.1f;
                m_pCurrentSelectedCube->Get_WorldMatrix()->_42 -= 0.1f;
            }
                // 계속 들어오긴하는데 이게 피킹이 Released를 해야지만 움직이니깐 좀 안맞네. 
                /* 여기서 이제 끝났으면 버퍼*/
                /* 그러면 큐브도 같이 움직여야하네;;..*/
              
        }

    }

    else
    {
        if (m_pCurrentSelectedPoint)
        {
            m_pContext->Unmap(m_pCurrentSelectedCellBuffer, 0);
            m_pCurrentSelectedPoint = nullptr; 
            m_pCurrentSelectedCube = nullptr;
        }
    }

    



#pragma endregion   

#pragma region WireFrameCellOnOFF
    if (m_bWireFrameCellOnOff)
    {
        m_pNavigation->Setting_Shader_Pass(1); // 패스 1번의 렌더스테이트는 Wireframe.
        
            
    }
    else
    {
        m_pNavigation->Setting_Shader_Pass(0); // 패스 0번의 렌더스테이트는 Default. 
   
    }
#pragma endregion 


#pragma region WireFrameCellOnOFF
    if (m_bWireFrameCubeOnOff)
    {
        for (auto& iter : m_pNavigation->Get_VecCube()) 
            iter->Setting_Shader_CubePass(1);   
    }

    else 
    {
        for (auto& iter : m_pNavigation->Get_VecCube())
            iter->Setting_Shader_CubePass(0);
    }

#pragma endregion 

}

HRESULT CImgui_Navigation::Render()
{
    if (m_pNavigation->Get_VecCell().size() > 0 || m_pNavigation->Get_VecCube().size() > 0)
    {
        /* 렌더그룹에 안넣어줘서그런가. */
        m_pNavigation->Render();    
    }   

    return S_OK;
}



void CImgui_Navigation::IntersectRay(CCube* _pCube)
{
    _float3 mouseRayPos = m_pGameInstance->Get_WorldRayPos();   
    _float3 mouseRayDir = m_pGameInstance->Get_WorldRayDir();   

    FXMVECTOR mousePos = { mouseRayPos.x,mouseRayPos.y,mouseRayPos.z,1.f }; 
    FXMVECTOR mouseRay = { mouseRayDir.x,mouseRayDir.y,mouseRayDir.z,0.f }; 
    

    D3D11_MAPPED_SUBRESOURCE mapped_VB_Resource;    
    ZeroMemory(&mapped_VB_Resource, sizeof(D3D11_MAPPED_SUBRESOURCE));  

    D3D11_MAPPED_SUBRESOURCE mapped_IB_Resource;    
    ZeroMemory(&mapped_IB_Resource, sizeof(D3D11_MAPPED_SUBRESOURCE));  


    

    HRESULT hr = m_pContext->Map(_pCube->Get_Buffer()->Get_VB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_VB_Resource);   
    HRESULT hr2 = m_pContext->Map(_pCube->Get_Buffer()->Get_IB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_IB_Resource);  



    VTXCUBE* pVertices = nullptr;
    _ushort* pIndices = nullptr;

    //정점버퍼
    pVertices = reinterpret_cast<VTXCUBE*>(mapped_VB_Resource.pData); 

    //인덱스버퍼
    pIndices = reinterpret_cast<_ushort*>(mapped_IB_Resource.pData);


    _uint m_iTotaiTriangle = _pCube->Get_Buffer()->Get_NumIndices() / 3;    


    float dist   = 0.f;   
    _uint index  = { 0 };    
    _bool PickingOnOff = false;



    for(size_t i =0 ; i< m_iTotaiTriangle; i++)
    {
       PickingOnOff = Intersects(mousePos, mouseRay,
            XMVector4Transform(XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f), XMLoadFloat4x4(_pCube->Get_WorldMatrix())),
            XMVector4Transform(XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f), XMLoadFloat4x4(_pCube->Get_WorldMatrix())),
            XMVector4Transform(XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f), XMLoadFloat4x4(_pCube->Get_WorldMatrix())),
            dist);

        if(true == PickingOnOff)
        {
            /* dist가 가장 가까운거 찍어야하니깐*/
            m_mapCube.emplace(dist, _pCube);
           
        }

    }


    m_pContext->Unmap(_pCube->Get_Buffer()->Get_VB_Buffer(), 0);    
    m_pContext->Unmap(_pCube->Get_Buffer()->Get_IB_Buffer(), 0);          
}

_bool CImgui_Navigation::IntersectCell(CCell* _pCell)
{
    _float3 mouseRayPos = m_pGameInstance->Get_WorldRayPos();
    _float3 mouseRayDir = m_pGameInstance->Get_WorldRayDir();

    FXMVECTOR mousePos = { mouseRayPos.x,mouseRayPos.y,mouseRayPos.z,1.f };
    FXMVECTOR mouseRay = { mouseRayDir.x,mouseRayDir.y,mouseRayDir.z,0.f };


    D3D11_MAPPED_SUBRESOURCE mapped_VB_Resource;
    ZeroMemory(&mapped_VB_Resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    D3D11_MAPPED_SUBRESOURCE mapped_IB_Resource;
    ZeroMemory(&mapped_IB_Resource, sizeof(D3D11_MAPPED_SUBRESOURCE));


   

    HRESULT hr = m_pContext->Map(_pCell->Get_Buffer()->Get_VB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_VB_Resource);
    HRESULT hr2 = m_pContext->Map(_pCell->Get_Buffer()->Get_IB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_IB_Resource);



    VTXCUBE* pVertices = nullptr;
    _ushort* pIndices = nullptr;

    //정점버퍼
    pVertices = reinterpret_cast<VTXCUBE*>(mapped_VB_Resource.pData);

    //인덱스버퍼
    pIndices = reinterpret_cast<_ushort*>(mapped_IB_Resource.pData);


    _uint m_iTotaiTriangle = _pCell->Get_Buffer()->Get_NumIndices() / 3;    


    float dist = 0.f;
    _uint index = { 0 };
    _bool PickingOnOff = false;


    for (size_t i = 0; i < m_iTotaiTriangle; i++)
    {
        PickingOnOff = Intersects(mousePos, mouseRay,
            XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f),   
            XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f),
            XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f),   
            dist);
    
        if (true == PickingOnOff)
        {
            m_mapCell.emplace(dist, _pCell);    
        }   
    
    }
    
    
    m_pContext->Unmap(_pCell->Get_Buffer()->Get_VB_Buffer(), 0);    
    m_pContext->Unmap(_pCell->Get_Buffer()->Get_IB_Buffer(), 0);    
    


    return PickingOnOff;


    return false;
}

_bool CImgui_Navigation::ChecKIntersect()
{
    for (auto pCubes : m_pNavigation->Get_VecCube())    
    {
        IntersectRay(pCubes);   
    }

    if(m_mapCube.size()>0)
    {
        _float3 point = { m_mapCube.begin()->second->Get_WorldMatrix()->_41 , m_mapCube.begin()->second->Get_WorldMatrix()->_42, m_mapCube.begin()->second->Get_WorldMatrix()->_43 };
        m_vecPickingPoint.push_back(point); 
        m_mapCube.clear(); 

        return true;
    }

    else
    {
        return false;
    }
    
}



VTXPOS* CImgui_Navigation::FindPoint(_float3 point)        
{
    /* 여기서는 그럼 해당 aabb 박스 선택해서 해당 박스의 worldmatrix 가져오고*/
    /* 가져온 worldmatirx 을 이용해서 cells 점과 비교해 찾는다. */

    for(auto& pCube : m_pNavigation->Get_VecCube())
    {
        IntersectRay(pCube);
    }
    /* 여기 결과로 m_mapCube에 결과가 쌓일거임. 거기서 beign() 꺼내자 */

    /* 여기서는 그럼 Cube의 월드매트릭스  */
    if (m_mapCube.size() > 0)
    {
        _vector vecpoint = XMLoadFloat4x4(m_mapCube.begin()->second->Get_WorldMatrix()).r[3];
        _float3  floatpoint = {};
        XMStoreFloat3(&floatpoint, vecpoint);

        for (auto& pCell : m_pNavigation->Get_VecCell())
        {
            /* 여기서 Cell의 점들을 하나씩 찾아야할듯;..*/


            D3D11_MAPPED_SUBRESOURCE mapped_VB_Resource;
            ZeroMemory(&mapped_VB_Resource, sizeof(D3D11_MAPPED_SUBRESOURCE));


            HRESULT hr = m_pContext->Map(pCell->Get_Buffer()->Get_VB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_VB_Resource);



            VTXPOS*  pVertices = nullptr;    
           

            //정점버퍼
            pVertices = reinterpret_cast<VTXPOS*>(mapped_VB_Resource.pData);    


            for (int i = 0; i < 3; i++)
            {
                XMVECTOR v1 = XMLoadFloat3(&pVertices[i].vPosition);
                XMVECTOR v2 = XMLoadFloat3(&floatpoint);    

                bool result = XMVector3Equal(v1, v2);

                if (result)
                {
                    //pVertices[i].vPosition = { 0.f,0.f,0.f };   /* 여기다 마우스가 클릭한 지점 눌르면될듯*/
                    m_pCurrentSelectedPoint = &pVertices[i];
                    m_pCurrentSelectedCellBuffer = pCell->Get_Buffer()->Get_VB_Buffer();   // 이건 Safe_Release 안해도됨 어차피 Cell 쪽에서 해제되니깐
                    m_pCurrentSelectedCube = m_mapCube.begin()->second; 
                    //m_pContext->Unmap(pCell->Get_Buffer()->Get_VB_Buffer(), 0);
                    m_mapCube.clear();
                    return pVertices;   
                }

            }

        }

        return nullptr; 
    }
}

CImgui_Navigation* CImgui_Navigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CImgui_ObjectTool* pObjectTool)
{
    CImgui_Navigation* pInstance = new CImgui_Navigation(pDevice, pContext, pObjectTool);

    if(FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : Imgui_Navigation");
        Safe_Release(pInstance);    
    }

    return pInstance;   
}

void CImgui_Navigation::Free()
{
    __super::Free();
    
  

    Safe_Release(m_pGameInstance); 
    Safe_Release(m_pNavigation); 
    Safe_Release(m_pObjectTool);
    Safe_Release(m_pContext);   
    Safe_Release(m_pDevice);    
}
