#include "pch.h"
#include "Imgui_ObjectTool.h"   
#include "ImguiManager.h"   
#include "GameObject.h"
#include "Layer.h"
#include "Mesh.h"



using namespace TriangleTests;


_float3 GetEulerAnglesFromQuaternion(const XMVECTOR& quaternion, float& pitch, float& yaw, float& roll) {
    // 쿼터니언을 회전 행렬로 변환
    XMMATRIX RotationMatrix = XMMatrixRotationQuaternion(quaternion);

    // 행렬에서 회전 각도를 추출
    pitch = asinf(-RotationMatrix.r[2].m128_f32[1]); // X축 회전 (Pitch)

    if (cosf(pitch) > 0.0001f) {
        yaw = atan2f(RotationMatrix.r[2].m128_f32[0], RotationMatrix.r[2].m128_f32[2]); // Y축 회전 (Yaw)
        roll = atan2f(RotationMatrix.r[0].m128_f32[1], RotationMatrix.r[1].m128_f32[1]); // Z축 회전 (Roll)
    }
    else {
        yaw = 0.0f;
        roll = atan2f(-RotationMatrix.r[1].m128_f32[0], RotationMatrix.r[0].m128_f32[0]);
    }

    // 각도를 라디안에서 도로 변환
    pitch = XMConvertToDegrees(pitch);
    yaw = XMConvertToDegrees(yaw);
    roll = XMConvertToDegrees(roll);

    return _float3(pitch, yaw, roll);
}

float CalculateRotationAngle(const XMVECTOR& vector1, const XMVECTOR& vector2, const XMVECTOR& rotationAxis)
{
    // 벡터를 정규화
    XMVECTOR v1 = XMVector3Normalize(vector1);
    XMVECTOR v2 = XMVector3Normalize(vector2);
    XMVECTOR axis = XMVector3Normalize(rotationAxis);

    // 벡터 사이의 각도 계산
    float dot = XMVectorGetX(XMVector3Dot(v1, v2)); // v1 · v2
    float angle = acosf(dot); // 라디안 단위의 각도

    // 방향 확인 (v1 × v2)
    XMVECTOR cross = XMVector3Cross(v1, v2);
    float direction = XMVectorGetX(XMVector3Dot(cross, axis)); // 축과 비교

    // 방향에 따라 각도 부호 조정
    if (direction < 0.0f) {
        angle = -angle; // 반대 방향으로 회전
    }

    // 라디안을 도 단위로 변환
    return XMConvertToDegrees(angle);
}

CImgui_ObjectTool::CImgui_ObjectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(CImguiManager::GetInstance()->Get_Device())
    ,m_pContext(CImguiManager::GetInstance()->Get_DeviceContext())
    ,m_pGameInstance(CImguiManager::GetInstance()->Get_GameInstance())
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CImgui_ObjectTool::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    /* 여기서 텍스처 세팅하기 */
    m_Textures.reserve(100);

    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/AltarBase.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Plant/Mushroom.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_Elemental_Boss_02.dds"), 1);  
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/Temple_Ground.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SM_TempleBrick_04.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/Temple_Block.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_Steps_A_Mod.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_Corner_D_Mod.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/M_GuardianP_POSE_A_SC_Rock_01.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/M_GuardianP_POSE_A_SC_Rock_02.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/M_GuardianP_POSE_A_SC_Rock_01_Weapon.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_Brick_v1.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SM_TempleBrick_10.dds"), 1); 
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SM_Brick_W_B_Mod2.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Water/Water.dds"), 1);  
    LoadTexture(TEXT("../Bin/Resources/Textures/Monster/M_Guardian_Model_C.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Plant/SM_LotusFlower_07a1.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/ST_LiftTable_a2_sui_00.dds"), 1);    
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SM_TempleBrick_01.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_Brick_W_B_Mod.dds"), 1);  
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_Brick_W_B_Mod_10.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Monster/M_Guardian_Model_D.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SM_Templelamp05.dds"), 1);   
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SM_Templelamp06.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_FlapTrap_03.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_Stone_HeavyBOX_A_Mod.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_TurnTable_B.dds"), 1);    
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/TurnTable_Head.dds"), 1);    
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/Box_Lift.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/LeverButton.dds"), 1);   
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_TempleDoor_05.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SM_TempleFloor_02.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_Object_StoneBall.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Npc/Strong_Man.dds"), 1);   
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_StepBox.dds"), 1);    
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_TotemB.dds"), 1); 
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SM_Pillar_01.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_WaterTemple_Parts_b.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_RuneStatue.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_Elemental_Boss_04.dds"), 1);  
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SC_Elemental_Boss_05.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/Wind_EleTower_09.dds"), 1);  
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/Wind_Elementak_Boss.dds"), 1);   
    LoadTexture(TEXT("../Bin/Resources/Textures/Monster/Boss.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/S_HpFlower.dds"), 1);
    LoadTexture(TEXT("../Bin/Resources/Textures/Temple/SM_MushroomA_L.dds"), 1);    
    
    
    m_pLayer = m_pGameInstance->Get_Layers();  

    return S_OK;
}

HRESULT CImgui_ObjectTool::Update()
{

	if (ImGui::BeginMenuBar())
	{   
		if (ImGui::BeginMenu("Object"))
		{
			if (ImGui::MenuItem("Obejct Tool Open", nullptr, m_bObjectWindowOnOff))
			{

				m_bObjectWindowOnOff = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

#pragma region 오브젝트 툴 창 

    if (m_bObjectWindowOnOff)
    {
        if (ImGui::Begin("Object Tool Window", &m_bObjectWindowOnOff))
        {

            
            if (ImGui::Button("Obejct_Select")) 
            {
                m_iCurrentView = 1; // 상태 업데이트
                //ObjectWindow_View_1();  
                
            }
            ImGui::SameLine(); // 버튼을 같은 줄에 배치

            if (ImGui::Button("Installed_Object")) 
            {
                m_iCurrentView = 2; // 상태 업데이트
            }

           /* ImGui::Text("This is the Object Tool Window.");
            ImGui::Button("Example Button");*/

            ImGui::Checkbox("Install_Object", &m_bInstallObject);
            ImGui::SameLine();  
            ImGui::Checkbox("Picking_Object", &m_bPickingObject);
            ImGui::SameLine();
            ImGui::Checkbox("Picking_Install_From_Mesh", &m_bPickingObjectFromMesh);
            ImGui::SameLine();
            ImGui::Checkbox("Navigation_Picking_From_Mesh", &m_bNavigationPickingFromMesh);

            if (m_bPickingObject && ImGui::IsMouseReleased(0) && !ImGui::IsWindowHovered() && !isMouseOnObjectInformationWindow)
            {
                Picking_Object();    // 여기 트랜스폼 창 만지다 피킹들어감 문제가좀 있음.
            }

            if (m_bPickingObjectFromMesh && ImGui::IsMouseReleased(0) && !ImGui::IsWindowHovered() && !isMouseOnObjectInformationWindow)
            {

                /* 피킹 조건도 필요하네 */
                if(Picking_Object())
                {
                    m_pRealTransform = nullptr;
                    m_bObjectTransformViewOnOff = false;
                    InstallFromObject(m_iCurrentClickObject);
                    

                    /* 여기 피킹 했는지 안했는지*/
                    m_bRealPickingSuccess = true;
                }
            
                    
                
            }
            else
            {
                m_bRealPickingSuccess = false;
            }
        }

        switch (m_iCurrentView)
        {
        case Imgui_Object_Window::Object_Texture_View:
        {
            ObjectWindow_View_1();
        }
        break;
        case Imgui_Object_Window::Object_Installed_View:
        {
            ObjectWindow_View_2();  
        }
        break;
      
        default:
            break;
        }

        ImGui::End(); // 항상 호출해야 함!
    }


 
    
    if(m_bInstallObject)
    {
       //if((GetAsyncKeyState(VK_LBUTTON) & 0x8000))
       if(CImguiManager::GetInstance()->Get_MousePicking())
       {
            //if (!ImGui::IsMouseClicked(0))  // ImGui에서 클릭이 아닌지 확인
            //{
                InstallObject(m_iCurrentClickObject);
                CImguiManager::GetInstance()->Set_MousePicking(false);
            //}
           /* 지금 문제가 m_pGameObject 포인터를 가져올 방안이 없음 */
           /* 1. 오브젝트 매니저에서 가져온다. */
           /* 2. 레이어에서 게임 오브젝트에 이름을 부여하여 찾아온다. */

       }
    }



#pragma endregion 

    //if(m_bPickingObject && ImGui::IsMouseReleased(0) && !ImGui::IsWindowHovered())  
    //{
    //    Picking_Object();
    //}


#pragma region ObjectTransformView

    if(m_bObjectTransformViewOnOff)
    {
        ImGui::Begin("Object Information",&m_bObjectTransformViewOnOff);
        {

            if (m_pRealTransform != nullptr && m_bPickingObject)
            {
                /* 마우스가 현재 이 창에 있는지 검사하는거 */
                isMouseOnObjectInformationWindow = ImGui::IsWindowHovered();

                _vector position = m_pRealTransform->Get_State(CTransform::STATE_POSITION);

                _float3 scale = m_pRealTransform->Get_Scaled();   // 여기서 문제발생

                /* 기저 벡터*/
                _vector X_Axis = { 1.f,0.f,0.f,0.f };
                _vector Y_Axis = { 0.f,1.f,0.f,0.f };
                _vector Z_Axis = { 0.f,0.f,1.f,0.f };

                _vector Object_X_Axis = XMVector3Normalize(m_pRealTransform->Get_State(CTransform::STATE_RIGHT));
                _vector Object_Y_Axis = XMVector3Normalize(m_pRealTransform->Get_State(CTransform::STATE_UP));
                _vector Object_Z_Axis = XMVector3Normalize(m_pRealTransform->Get_State(CTransform::STATE_LOOK));

                //XMMatrixRotationAxis(Rotation_X, XMConvertToRadians(90.f)); 

                


                float scale_x = scale.x;
                float scale_y = scale.y;
                float scale_z = scale.z;

                /* 문제가 있음 가지고 올 때 몇도 회전되어있는 상태인지를 체크를 못해주고 0으로 초기화 작업이 됨*/
               /* float Angle_X = CalculateRotationAngle(X_Axis, Object_X_Axis, X_Axis);
                float Angle_Y = CalculateRotationAngle(Y_Axis, Object_Y_Axis, Y_Axis);
                float Angle_Z = CalculateRotationAngle(Z_Axis, Object_Z_Axis, Z_Axis);*/

                _vector Scale = {}; 
                _vector RotationQuat = {};  
                _vector Translation = {};   

                //XMMatrixDecompose(&Scale, &RotationQuat, &Translation, m_pRealTransform->Get_WorldMatrix());    

                _float3 Degree = {};

                // 쿼터니언을 오일러 각도로 변환
                Degree = GetEulerAnglesFromQuaternion(RotationQuat, Degree.x, Degree.y, Degree.z);  

                float Angle_X = m_Pre_Angle_X;   
                float Angle_Y = m_Pre_Angle_Y;
                float Angle_Z = m_Pre_Angle_Z;
              


                float poistion_x = XMVectorGetX(position);
                float poistion_y = XMVectorGetY(position);
                float poistion_z = XMVectorGetZ(position);



                ImGui::PushItemWidth(100.0f); // 슬라이더 폭을 200픽셀로 설정  

                /* 크기 */
                ImGui::Text("Scale");
                ImGui::SameLine();
                ImGui::Dummy(ImVec2(32.0f, 0.0f));
                ImGui::SameLine();
                 
                ImGui::DragFloat("##Scale_X", &scale.x, 0.001f, 0.001f, 5.0f);
                scale.x = scale.x < 0.001f ? 0.001f : scale.x;

                ImGui::SameLine();
         
                ImGui::DragFloat("##Scale_Y", &scale.y, 0.001f, 0.001f, 5.0f);
                scale.y = scale.y < 0.001f ? 0.001f : scale.y;

                ImGui::SameLine();
               
                ImGui::DragFloat("##Scale_Z", &scale.z, 0.001f, 0.001f, 5.0f);
                scale.z = scale.z < 0.001f ? 0.001f : scale.z;
                //

                /* 회전 */
                ImGui::Text("Rotation");
                ImGui::SameLine();
                ImGui::Dummy(ImVec2(12.0f, 0.0f));
                ImGui::SameLine();
                ImGui::InputFloat("##Rotation_X", &Angle_X);    
              
                //ImGui::IsKeyPressed(ImGuiKey_Enter)
                ImGui::SameLine();
                ImGui::InputFloat("##Rotation_Y", &Angle_Y);     
                
     
                //ImGui::InputFloat("##Rotation_Y", &m_Angle_Y);
                ImGui::SameLine();
                ImGui::InputFloat("##Rotation_Z", &Angle_Z);        

                //ImGui::InputFloat("##Rotation_Z", &m_Angle_Z);
                //



                /* 이동 */
                ImGui::Text("Translation");
                ImGui::SameLine();
                ImGui::SliderFloat("##Translation_X", &poistion_x, 0.0f, 129.0f);

                ImGui::SameLine();
                ImGui::SliderFloat("##Translation_Y", &poistion_y, 0.0f, 500.0f);

                ImGui::SameLine();
                ImGui::SliderFloat("##Translation_Z", &poistion_z, 0.0f, 129.0f);
                //










                if (GetAsyncKeyState(VK_UP) & 0x8000)
                {
                    poistion_z += 10.f * m_pGameInstance->Get_TimeDelta(TEXT("Timer_60"));
                }

                if (GetAsyncKeyState(VK_DOWN) & 0x8000)
                {
                    poistion_z -= 10.f * m_pGameInstance->Get_TimeDelta(TEXT("Timer_60"));
                    //m_pRealTransform->Go_BackWard(m_pGameInstance->Get_TimeDelta(TEXT("Timer_60")));
                }


                if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
                {
                    poistion_x += 10.f * m_pGameInstance->Get_TimeDelta(TEXT("Timer_60"));
                }

                if (GetAsyncKeyState(VK_LEFT) & 0x8000)
                {
                    poistion_x -= 10.f * m_pGameInstance->Get_TimeDelta(TEXT("Timer_60"));
                    //m_pRealTransform->Go_Left(m_pGameInstance->Get_TimeDelta(TEXT("Timer_60")));    
                }

                if (GetAsyncKeyState('O') & 0x8000)
                {
                    poistion_y += 10.f * m_pGameInstance->Get_TimeDelta(TEXT("Timer_60"));
                    //m_pRealTransform->Go_Left(m_pGameInstance->Get_TimeDelta(TEXT("Timer_60")));    
                }

                if (GetAsyncKeyState('P') & 0x8000)
                {
                    poistion_y -= 10.f * m_pGameInstance->Get_TimeDelta(TEXT("Timer_60"));
                    //m_pRealTransform->Go_Left(m_pGameInstance->Get_TimeDelta(TEXT("Timer_60")));    
                }




                m_pRealTransform->Set_Scale(scale.x, scale.y, scale.z);

                

                    
                
                if (ImGui::IsKeyPressed(ImGuiKey_Enter, false) && m_Pre_Angle_X != Angle_X) {
                    m_pRealTransform->Turn_X(-m_Pre_Angle_X);   
                    m_pRealTransform->Turn_X(Angle_X);  // 입력된 X 회전 각도를 반영    
                    m_Pre_Angle_X = Angle_X;  
                    
                }

                if (ImGui::IsKeyPressed(ImGuiKey_Enter, false) && m_Pre_Angle_Y != Angle_Y) {   
                    m_pRealTransform->Turn_Y(-m_Pre_Angle_Y);       
                    m_pRealTransform->Turn_Y(Angle_Y);  // 입력된 Y 회전 각도를 반영    
                    m_Pre_Angle_Y = Angle_Y;      
                }

                if (ImGui::IsKeyPressed(ImGuiKey_Enter, false)&& m_Pre_Angle_Z != Angle_Z) {
                    m_pRealTransform->Turn_Z(-m_Pre_Angle_Z);
                    m_pRealTransform->Turn_Z(Angle_Z);  // 입력된 Z 회전 각도를 반영        
                    m_Pre_Angle_Z = Angle_Z;  
                }

                  
            
                m_pRealTransform->Set_State(CTransform::STATE_POSITION, _fvector{ poistion_x,poistion_y,poistion_z,1.f });
                //m_pRealTransform->Set_State(CTransform::STATE_RIGHT, m_pRealTransform->Get_State(CTransform::STATE_RIGHT));



                ImVec2 buttonSize = ImVec2(100.f, 30.f);
                float windowWidth = ImGui::GetWindowSize().x;
                float winodwHeight = ImGui::GetWindowSize().y;
                float buttonPosX = windowWidth - buttonSize.x - 10.f; // 오른쪽 끝     
                float buttonPosY = winodwHeight - 40.f;
                ImGui::SetCursorPos(ImVec2(buttonPosX, buttonPosY));



                if (ImGui::Button(u8"삭제", buttonSize) || ImGui::IsKeyReleased(ImGuiKey_X))
                {
                    /* 삭제 버튼 눌릴 시 */
                    list<CGameObject*>& list = m_pCurrentLayer->Get_GameObject_List();

                    for (auto iter = list.begin(); iter != list.end(); iter++)
                    {

                        if (*iter == m_pCurrentGameObject)
                        {
                            list.erase(iter);
                            Safe_Release(m_pCurrentGameObject);
                            m_bObjectTransformViewOnOff = false;
                            break;
                        }

                    }

                }
            }

        }
        ImGui::End();
       
    }
    else
    {
        isMouseOnObjectInformationWindow = false;
    }
    

#pragma endregion


	return S_OK;
}



void CImgui_ObjectTool::ObjectWindow_View_2()
{
    ImGui::Text("Installed_Object");

   
    /* GamePlay_Level 이 인덱스 "3" 임*/
    for (auto& Pair : m_pLayer[3])
    {
        if (nullptr != Pair.second) 
        {
            // 왼쪽 자식 구역
            //ImGui::Text("Layers");
            ImGui::BeginChild("Layers", ImVec2(300, 0), true);
            char multiByteStr[MAX_PATH];
            WideCharToMultiByte(CP_ACP, 0, Pair.first.c_str(), -1, multiByteStr, sizeof(multiByteStr), NULL, NULL);
            if(ImGui::Selectable(multiByteStr))
            {
                /* 여기서 이제 해당 레이어의 물체들 현황보여주기*/
                //ImGui::Text("hi");
                // 게임오브젝트에 이름을 할당하는게 편할듯.
                //Pair.second->Find_GameObject(Pair.first); 
                //strcpy(m_szCurrentLayerName, multiByteStr);
                lstrcpy(m_szCurrentLayerName, Pair.first.c_str());  
    
            }
            ImGui::EndChild();

        }
    }

    ImGui::SameLine();  
    if (m_szCurrentLayerName != TEXT(""))
    {

        ImGui::BeginChild("GameObejct", ImVec2(600, 0), true);
        auto& iter = find_if(m_pLayer[3].begin(), m_pLayer[3].end(), [this](const auto& Pair) {
            return lstrcmp(m_szCurrentLayerName, Pair.first.c_str()) == 0; });

        if (iter != m_pLayer[3].end())
        {
            for (auto GameObject : iter->second->Get_GameObject_List())
            {
                /* 변환이 잘못됫나?*/
                //ImGui::Text("hi"); // 여기서문제가 발생하네
                const char* test = GameObject->Get_Name();
                if(ImGui::Selectable(test))
                {
                   /* 지금 해당 객체를 삭제하는 작업을 해야하니깐 여기서는 */
                   /* 객체 삭제 작업을 진행해보자면. */
                 
                    
                }
                //ImGui::Text(GameObject->Get_Name());
            }

        }
        ImGui::EndChild();
    }

}

void CImgui_ObjectTool::ObjectWindow_View_Transform()
{
}

bool CImgui_ObjectTool::Picking_Object()
{
    m_pRealTransform = nullptr;
    m_pCurrentGameObject = nullptr;

    _float3 mouseRayPos = m_pGameInstance->Get_WorldRayPos();   
    _float3 mouseRayDir = m_pGameInstance->Get_WorldRayDir();   

    FXMVECTOR mousePos = { mouseRayPos.x,mouseRayPos.y,mouseRayPos.z,1.f }; 
    FXMVECTOR mouseRay = { mouseRayDir.x,mouseRayDir.y,mouseRayDir.z,0.f }; 

    for(auto& Layers : m_pLayer[3])
    {
        for(auto& GameObject :Layers.second->Get_GameObject_List())
        {
            /* 여기가 이제 게임 오브젝트임*/
            //CModel* pModel = dynamic_cast<CModel*>(
            //m_pGameInstance->Find_Component(LEVEL_GAMEPLAY, Layers.first, TEXT("Com_Model")));
            CModel* pModel = dynamic_cast<CModel*>(GameObject->Find_Component(TEXT("Com_Model")));
            /*이제 그러면 pVIBuffer를 가져오자 */
            /*근데 이제 매쉬는 월드 스페이스 니깐. 해당 버퍼의 월드스페이스기준으로 피킹해줘야함.*/


            /* 터레인 버퍼 피킹 */
            //CVIBuffer_Terrain* pTerrain = dynamic_cast<CVIBuffer_Terrain*>(GameObject->Find_Component(TEXT("Terrain_Com")));
            //if(pTerrain != nullptr)
            //{
            //    D3D11_MAPPED_SUBRESOURCE mapped_VB_Resource;
            //    ZeroMemory(&mapped_VB_Resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

            //    D3D11_MAPPED_SUBRESOURCE mapped_IB_Resource;
            //    ZeroMemory(&mapped_IB_Resource, sizeof(D3D11_MAPPED_SUBRESOURCE));


            //    HRESULT hr = m_pContext->Map(pTerrain->Get_VB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_VB_Resource);
            //    HRESULT hr2 = m_pContext->Map(pTerrain->Get_IB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_IB_Resource);



            //    countA++;
            //    VTXNORTEX* pVertices = nullptr; 
            //    _uint* pIndices = nullptr;  
            //    //정점버퍼
            //    pVertices = reinterpret_cast<VTXNORTEX*>(mapped_VB_Resource.pData); 

            //    //인덱스버퍼
            //    pIndices = reinterpret_cast<_uint*>(mapped_IB_Resource.pData);

            //    _uint m_iTotaiTriangle = pTerrain->Get_NumIndices() / 3; //여기서 오류발생하네 

            //    float dist = 0.f;
            //    _uint index = { 0 };

            //    /* 그러면 현재 마우스 레이가 월드인데 포지션은 다르니깐 곱해주면될듯한데*/
            //    m_pTransform = dynamic_cast<CTransform*>(GameObject->Find_Component(TEXT("Com_Transform")));
            //    m_pTransform->Get_WorldMatrix();

            //    for (size_t i = 0; i < m_iTotaiTriangle; i++)
            //    {
            //        bool PickingOnOff = Intersects(mousePos, mouseRay,
            //            XMVector4Transform(XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f), m_pTransform->Get_WorldMatrix()),
            //            XMVector4Transform(XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f), m_pTransform->Get_WorldMatrix()),
            //            XMVector4Transform(XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f), m_pTransform->Get_WorldMatrix()),
            //            dist);

            //        if (PickingOnOff == true)
            //        {
            //            /* 여기서 그럼 해당 GameObject의 transform 가져와서 SRT변환 해야하는데 흠.*/
            //            //m_pRealTransform = m_pTransform;    
            //            m_bObjectTransformViewOnOff = true;
            //            //m_pCurrentGameObject = GameObject;  
            //            m_pCurrentLayer = Layers.second;
            //            

            //            m_pRealTransform = m_pTransform;


            //            return true;  // 여기다가 중단점찍으면 바다피킹가능 바다 관련 찾기 검색.
            //           


            //        }


            //    }

            //    countB++;
            //    m_pContext->Unmap(pTerrain->Get_VB_Buffer(), 0);
            //    m_pContext->Unmap(pTerrain->Get_IB_Buffer(), 0);
            //}

            /* 터레인버퍼피킹 */
            if (pModel == nullptr)
                int a = 4;

            if(pModel != nullptr)
            {
                /* vector<class CMesh*>  */

                //CMesh* pMesh = (pModel->Get_Meshes().front()); // 
                // 
                //pMesh->Get_VB_Buffer
                for (auto& pMesh : pModel->Get_Meshes())
                {

                    D3D11_MAPPED_SUBRESOURCE mapped_VB_Resource;
                    ZeroMemory(&mapped_VB_Resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

                    D3D11_MAPPED_SUBRESOURCE mapped_IB_Resource;
                    ZeroMemory(&mapped_IB_Resource, sizeof(D3D11_MAPPED_SUBRESOURCE));


                    HRESULT hr = m_pContext->Map(pMesh->Get_VB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_VB_Resource);
                    HRESULT hr2 = m_pContext->Map(pMesh->Get_IB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_IB_Resource);

                    

                    countA++;
                    VTXMESH* pVertices = nullptr;
                    _uint* pIndices = nullptr;
                    //정점버퍼
                    pVertices = static_cast<VTXMESH*>(mapped_VB_Resource.pData); //여기 애니메쉬는 제한걸어줘야함 
                    if (pModel->Get_ModelType() == CModel::MODEL_ANIM)  
                        VTXANIMMESH* pVertices = static_cast<VTXANIMMESH*>(mapped_VB_Resource.pData);  
                    
                    //인덱스버퍼
                    pIndices = reinterpret_cast<_uint*>(mapped_IB_Resource.pData);
                    
                    _uint m_iTotaiTriangle = pMesh->Get_NumIndices() / 3; //여기서 오류발생하네 
                    
                    float dist = 0.f;
                    _uint index = { 0 };
                    
                    /* 그러면 현재 마우스 레이가 월드인데 포지션은 다르니깐 곱해주면될듯한데*/
                    m_pTransform = dynamic_cast<CTransform*>(GameObject->Find_Component(TEXT("Com_Transform")));
                    m_pTransform->Get_WorldMatrix();
                    
                    for (size_t i = 0; i < m_iTotaiTriangle; i++)
                    {
                        bool PickingOnOff = Intersects(mousePos, mouseRay,
                            XMVector4Transform(XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f), m_pTransform->Get_WorldMatrix()),
                            XMVector4Transform(XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f), m_pTransform->Get_WorldMatrix()),
                            XMVector4Transform(XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f), m_pTransform->Get_WorldMatrix()),
                            dist);
                    
                        if (PickingOnOff == true)
                        {
                            /* 여기서 그럼 해당 GameObject의 transform 가져와서 SRT변환 해야하는데 흠.*/
                            //m_pRealTransform = m_pTransform;    
                            m_bObjectTransformViewOnOff = true; 
                            //m_pCurrentGameObject = GameObject;  
                            m_pCurrentLayer = Layers.second;        
                    
                            //Compute_Final_MeshPicking(GameObject,pMesh,pVertices,pIndices, mousePos, mouseRay); 
                            //m_fMeshPickingPoint = intersection;
                            // 
                            /* 테스트 주석 */
                            // 여기주석풀기

                             
                            


                            Picked_Object_Info Info = {};   
                            Info.pGamObject = GameObject;   
                            Info.Indices = pIndices;        
                            Info.pVertices = pVertices;     
                            Info._pMesh = pMesh;        
                            Info.pTransform = m_pTransform; 
                            
                            countC++;

                             
                            m_mapPickedMesh.emplace(dist, Info);

                    
                        }
                     
                    
                    }
                    
                    /* 여기서 모든 삼각형이랑 레이가 충돌을 안했으니 unmap해주기*/
                    /*  if(m_pRealTransform == nullptr)
                    {
                        m_pContext->Unmap(pMesh->Get_VB_Buffer(), 0);   
                        m_pContext->Unmap(pMesh->Get_IB_Buffer(), 0);  
                        countB++;   
                    }*/
                    countB++;
                    m_pContext->Unmap(pMesh->Get_VB_Buffer(), 0);
                    m_pContext->Unmap(pMesh->Get_IB_Buffer(), 0);
                    
                }
              

            }
 
            
        }
    }


    if(m_mapPickedMesh.size() > 0 )
    {
        Compute_Final_MeshPicking(m_mapPickedMesh, mousePos, mouseRay);
        m_bPickingSuccess = true;
        m_pRealTransform = m_mapPickedMesh.begin()->second.pTransform;      
        m_pCurrentGameObject = m_mapPickedMesh.begin()->second.pGamObject;  
       
        m_mapPickedMesh.clear();    
    }

    else
    {
        m_bPickingSuccess = false;

    }
       
  
    
    /* 메모리 해제 */
   
    int n = 10; 
     
    return m_bPickingSuccess; // 이게 문제내 
}



HRESULT CImgui_ObjectTool::LoadTexture(const _tchar* strTextureFilePath, _uint iNumTextures)
{
    
    _tchar  szTextureFilePath[MAX_PATH] = TEXT("");
    _tchar  szEXT[MAX_PATH] = TEXT("");
    
    //m_Textures.reserve(iNumTextures);
    
    //m_iNumTextures = iNumTextures;
    
    for (size_t i = 0; i < iNumTextures; i++)
    {
        /*..\Bin\Resource\Textures\Default0.jpg*/
        wsprintf(szTextureFilePath, strTextureFilePath, i);
    
        wsprintf(m_szTextureFilePath, szTextureFilePath);
        /* 파일 확장자 코드 가져오는 함수*/
        _wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);
    
        ID3D11ShaderResourceView* pSRV = { nullptr };
    
        HRESULT			hr{};
    
        if (false == lstrcmp(szEXT, TEXT(".dds")))
            hr = CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);    
    
        else
            hr = CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
    
        if (FAILED(hr))
            return E_FAIL;
    
        m_Textures.push_back(pSRV);
    }

    return S_OK;
}

CImgui_ObjectTool* CImgui_ObjectTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CImgui_ObjectTool* pInstance = new CImgui_ObjectTool(pDevice,pContext);

    if(FAILED(pInstance->Init(pDevice,pContext)))
    {
        MSG_BOX("Failed to Created : ObjectTool");
        Safe_Release(pInstance);
    }
    return pInstance;
}



bool CImgui_ObjectTool::Compute_Final_MeshPicking(map<_float, Picked_Object_Info> _mapPickedMesh, FXMVECTOR mousePos, FXMVECTOR mouseRay)   
{
    Picked_Object_Info PickedMesh = {}; /* 이게 맵에서 정렬해줘서 오름차순으로 가장 가까운 거리에서 피킹된 물체 */
    PickedMesh = _mapPickedMesh.begin()->second;   
    
    

    CTransform* pTransform =
        dynamic_cast<CTransform*>(PickedMesh.pGamObject->Find_Component(TEXT("Com_Transform")));

    
    _uint iTotalTriangle = PickedMesh._pMesh->Get_NumIndices() / 3;
    
    _uint index = {};
    float dist = {};

    for (size_t i = 0; i < iTotalTriangle; i++)
    {
        bool PickingOnOff = Intersects(mousePos, mouseRay,
            XMVector4Transform(XMVectorSetW(XMLoadFloat3(&PickedMesh.pVertices[PickedMesh.Indices[index++]].vPosition), 1.f), pTransform->Get_WorldMatrix()),
            XMVector4Transform(XMVectorSetW(XMLoadFloat3(&PickedMesh.pVertices[PickedMesh.Indices[index++]].vPosition), 1.f), pTransform->Get_WorldMatrix()),
            XMVector4Transform(XMVectorSetW(XMLoadFloat3(&PickedMesh.pVertices[PickedMesh.Indices[index++]].vPosition), 1.f), pTransform->Get_WorldMatrix()),
            dist);

        if (PickingOnOff)
        {
            _fvector ThreePoint[3] =
            {
                XMVector4Transform(XMVectorSetW(XMLoadFloat3(&PickedMesh.pVertices[PickedMesh.Indices[index - 3]].vPosition), 1.f), pTransform->Get_WorldMatrix()),
                XMVector4Transform(XMVectorSetW(XMLoadFloat3(&PickedMesh.pVertices[PickedMesh.Indices[index - 2]].vPosition), 1.f), pTransform->Get_WorldMatrix()),
                XMVector4Transform(XMVectorSetW(XMLoadFloat3(&PickedMesh.pVertices[PickedMesh.Indices[index - 1]].vPosition), 1.f), pTransform->Get_WorldMatrix()),
            };

            XMFLOAT3 point1, point2, point3;

            XMStoreFloat3(&point1, ThreePoint[0]);
            XMStoreFloat3(&point2, ThreePoint[1]);
            XMStoreFloat3(&point3, ThreePoint[2]);


            _fvector NormalVector =
                XMPlaneFromPoints(
                    XMVectorSetW(XMLoadFloat3(&point1),1.f),
                    XMVectorSetW(XMLoadFloat3(&point2),1.f), 
                    XMVectorSetW(XMLoadFloat3(&point3),1.f));

            XMFLOAT3 result_Normal; 
            result_Normal.x = XMVectorGetX(NormalVector);   
            result_Normal.y = XMVectorGetY(NormalVector);   
            result_Normal.z = XMVectorGetZ(NormalVector);

            _float3   Result_Noraml = {};   
            Result_Noraml = { result_Normal.x,result_Normal.y,result_Normal.z};     

            _float3 intersection = m_pGameInstance->Compute_PickingPoint(
                point1, point2, point3
            );


            finalPicking.emplace(dist, intersection);
            /* 여기다가 법선벡터 하나 더 저장하자 시발 그냥 */
            finalNoramlVector.emplace(dist, Result_Noraml); 

        }

    }
    // 여기서 이제 해당 dist가 가장 가까운걸 리턴해주면 될듯
    // 근데 map은 자동으로 오름차순이므로 첫번째 요소의 first를 반환해주면 될거임 
    // 

    
    _float3 normal = {};
    m_fMeshPickingPoint = finalPicking.begin()->second; // 이건 나중에 네비게이션 피킹용으로 쓰자 우
    normal = finalNoramlVector.begin()->second;

    //_float3 scale = pTransform->Get_Scaled();

    /* 여기서 다뤄야할듯 검사하는거 어디다 설치할지 */


    
    /* 이제 여기서 어떤기준으로 동서남북 위아래 나눠야서 설치 */


    if (m_bNavigationPickingFromMesh == false)
    {

        /* 해당 물체보다 오른쪽에 있으면서 높이는 그 물체와 같다. */
        if (normal.x < 0.f)
        {
            float installed_position_x = (m_fMeshPickingPoint.x - pTransform->Get_WorldFloat4X4()->_41) + m_fMeshPickingPoint.x;
            float installed_position_y = pTransform->Get_WorldFloat4X4()->_42;
            float installed_position_z = pTransform->Get_WorldFloat4X4()->_43;
            m_fMeshPickingPoint = { installed_position_x ,installed_position_y,installed_position_z };

        }

        /* 해당 물체보다 왼쪽에 있으면서 높이는 그 물체와 같다. */
        else if (normal.x > 0.f)
        {

            float installed_position_x = m_fMeshPickingPoint.x - (pTransform->Get_WorldFloat4X4()->_41 - m_fMeshPickingPoint.x);
            float installed_position_y = pTransform->Get_WorldFloat4X4()->_42;
            float installed_position_z = pTransform->Get_WorldFloat4X4()->_43;
            m_fMeshPickingPoint = { installed_position_x ,installed_position_y,installed_position_z };

        }


        else if (normal.y == 1.f)
        {

            float installed_position_x = pTransform->Get_WorldFloat4X4()->_41;
            float installed_position_y = (m_fMeshPickingPoint.y - pTransform->Get_WorldFloat4X4()->_42) + m_fMeshPickingPoint.y;
            float installed_position_z = pTransform->Get_WorldFloat4X4()->_43;
            m_fMeshPickingPoint = { installed_position_x ,installed_position_y,installed_position_z };

        }

        else if (normal.y == -1.f)
        {

            float installed_position_x = pTransform->Get_WorldFloat4X4()->_41;
            float installed_position_y = m_fMeshPickingPoint.y - (pTransform->Get_WorldFloat4X4()->_42 - m_fMeshPickingPoint.y);
            float installed_position_z = pTransform->Get_WorldFloat4X4()->_43;
            m_fMeshPickingPoint = { installed_position_x ,installed_position_y,installed_position_z };

        }

        /* 해당 설치할 물체가 앞에 있으면서 높이는 그 물체와 같다. */

        else if (normal.z == -1.f)
        {

            float installed_position_x = pTransform->Get_WorldFloat4X4()->_41;
            float installed_position_y = pTransform->Get_WorldFloat4X4()->_42;
            float installed_position_z = m_fMeshPickingPoint.z - (pTransform->Get_WorldFloat4X4()->_43 - m_fMeshPickingPoint.z);
            m_fMeshPickingPoint = { installed_position_x ,installed_position_y,installed_position_z };

        }

        /* 해당 설치할 물체가 뒤에 있으면서 높이는 그 물체와 같다. */
        else if (normal.z == 1.f)
        {

            float installed_position_x = pTransform->Get_WorldFloat4X4()->_41;
            float installed_position_y = pTransform->Get_WorldFloat4X4()->_42;
            float installed_position_z = m_fMeshPickingPoint.z + (m_fMeshPickingPoint.z - pTransform->Get_WorldFloat4X4()->_43);
            m_fMeshPickingPoint = { installed_position_x ,installed_position_y,installed_position_z };

        }
    }
 

    m_fMeshPickingScale = pTransform->Get_Scaled();     
    XMStoreFloat4x4(&m_PickingMeshRotationMatrix , pTransform->Get_RotationMatrixFromWorld(pTransform->Get_WorldMatrix()));       

    finalNoramlVector.clear();  
    finalPicking.clear();


    return true;

}

void CImgui_ObjectTool::ObjectWindow_View_1()
{
    //ID3D11ShaderResourceView* g_pTextureSRV = nullptr;
    ImGui::Text("Object");
    ImVec2 buttonSize = ImVec2(100, 100);

    if (ImGui::ImageButton("AltarBase", m_Textures[0], buttonSize))
    {
        m_iCurrentClickObject = 0;
    }
    ImGui::SameLine();
    if (ImGui::ImageButton("Mushroom", m_Textures[1], buttonSize))
    {
        m_iCurrentClickObject = 1;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SC_Elemental_Boss_02", m_Textures[2], buttonSize))
    {
        m_iCurrentClickObject = 2;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("Temple_Ground", m_Textures[3], buttonSize))
    {
        m_iCurrentClickObject = 3;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SM_TempleBrick_04", m_Textures[4], buttonSize))
    {
        m_iCurrentClickObject = 4;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("Temple_Block", m_Textures[5], buttonSize))
    {
        m_iCurrentClickObject = 5;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SC_Steps_A_Mod", m_Textures[6], buttonSize))
    {
        m_iCurrentClickObject = 6;
    }



    ImGui::SameLine();
    if (ImGui::ImageButton("SC_Corner_D_Mod", m_Textures[7], buttonSize))
    {
        m_iCurrentClickObject = 7;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("M_GuardianP_POSE_A_SC_Rock_01", m_Textures[8], buttonSize))
    {
        m_iCurrentClickObject = 8;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("M_GuardianP_POSE_A_SC_Rock_02", m_Textures[9], buttonSize))
    {
        m_iCurrentClickObject = 9;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("M_GuardianP_POSE_A_SC_Rock_01_Weapon", m_Textures[10], buttonSize))
    {
        m_iCurrentClickObject = 10;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SC_Brick_v1", m_Textures[11], buttonSize))
    {
        m_iCurrentClickObject = 11;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SC_TempleBrick_10", m_Textures[12], buttonSize))
    {
        m_iCurrentClickObject = 12;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SM_Brick_W_B_Mod2", m_Textures[13], buttonSize))
    {
        m_iCurrentClickObject = 13;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("Water", m_Textures[14], buttonSize))
    {
        m_iCurrentClickObject = 14;
    }


    ImGui::SameLine();
    if (ImGui::ImageButton("M_Guardian_Model_C", m_Textures[15], buttonSize))
    {
        m_iCurrentClickObject = 15;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SM_LotusFlower_07a1", m_Textures[16], buttonSize))
    {
        m_iCurrentClickObject = 16;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("ST_LiftTable_a2_sui_00", m_Textures[17], buttonSize))
    {
        m_iCurrentClickObject = 17;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SM_TempleBrick_01", m_Textures[18], buttonSize))
    {
        m_iCurrentClickObject = 18;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SC_Brick_W_B_Mod", m_Textures[19], buttonSize))
    {
        m_iCurrentClickObject = 19;
    }


    ImGui::SameLine();
    if (ImGui::ImageButton("SC_Brick_W_B_Mod_10", m_Textures[20], buttonSize))
    {
        m_iCurrentClickObject = 20;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("M_Guardian_Model_D", m_Textures[21], buttonSize))
    {
        m_iCurrentClickObject = 21;
    }
   
    ImGui::SameLine();
    if (ImGui::ImageButton("SM_Templelamp05", m_Textures[22], buttonSize))
    {
        m_iCurrentClickObject = 22;
    }


    ImGui::SameLine();
    if (ImGui::ImageButton("SM_Templelamp06", m_Textures[23], buttonSize))
    {
        m_iCurrentClickObject = 23;
    }


    ImGui::SameLine();
    if (ImGui::ImageButton("SC_FlapTrap_03", m_Textures[24], buttonSize))
    {
        m_iCurrentClickObject = 24;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SC_Stone_HeavyBOX_A_Mod", m_Textures[25], buttonSize))
    {
        m_iCurrentClickObject = 25;
    }


    ImGui::SameLine();
    if (ImGui::ImageButton("TurnTable_Body", m_Textures[26], buttonSize))
    {
        m_iCurrentClickObject = 26;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("TurnTable_Head", m_Textures[27], buttonSize))
    {
        m_iCurrentClickObject = 27;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("Box_Lift", m_Textures[28], buttonSize))
    {
        m_iCurrentClickObject = 28;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("LeverButton", m_Textures[29], buttonSize))
    {
        m_iCurrentClickObject = 29;
    }


    ImGui::SameLine();
    if (ImGui::ImageButton("SC_TempleDoor_05", m_Textures[30], buttonSize))
    {
        m_iCurrentClickObject = 30;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SM_TempleFloor_02", m_Textures[31], buttonSize))
    {
        m_iCurrentClickObject = 31;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SC_Object_StoneBall", m_Textures[32], buttonSize))
    {
        m_iCurrentClickObject = 32;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("Strong_Man", m_Textures[33], buttonSize))   
    {
        m_iCurrentClickObject = 33;
    }

    if (ImGui::ImageButton("SC_StepBox", m_Textures[34], buttonSize))   
    {
        m_iCurrentClickObject = 34;
    }

    ImGui::SameLine();  
    if (ImGui::ImageButton("SC_TotemB", m_Textures[35], buttonSize))    
    {
        m_iCurrentClickObject = 35;
    }


    ImGui::SameLine();
    if (ImGui::ImageButton("SM_Pillar_01", m_Textures[36], buttonSize))
    {
        m_iCurrentClickObject = 36;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SC_WaterTemple_Parts_b", m_Textures[37], buttonSize))
    {
        m_iCurrentClickObject = 37;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SC_RuneStatue", m_Textures[38], buttonSize))
    {
        m_iCurrentClickObject = 38;
    }

    ImGui::SameLine();      
    if (ImGui::ImageButton("SC_Elemental_Boss_04", m_Textures[39], buttonSize)) 
    {
        m_iCurrentClickObject = 39; 
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("SC_Elemental_Boss_05", m_Textures[40], buttonSize))
    {
        m_iCurrentClickObject = 40; 
    }


    ImGui::SameLine();
    if (ImGui::ImageButton("Wind_EleTower_09", m_Textures[41], buttonSize))
    {
        m_iCurrentClickObject = 41;
    }


    ImGui::SameLine();
    if (ImGui::ImageButton("Wind_Elementak_Boss", m_Textures[42], buttonSize))  
    {
        m_iCurrentClickObject = 42; 
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("Boss", m_Textures[43], buttonSize))
    {
        m_iCurrentClickObject = 43;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("S_HpFlower", m_Textures[44], buttonSize))
    {
        m_iCurrentClickObject = 44;
    }

    ImGui::SameLine();
    if (ImGui::ImageButton("M_MushroomA_L", m_Textures[45], buttonSize))
    {
        m_iCurrentClickObject = 45;
    }



    
   
}


HRESULT CImgui_ObjectTool::InstallObject(_int _CurrentClickObject)
{
    CGameObject::GAMEOBJECT_DESC pDesc{};
    pDesc.fPostion = CImguiManager::GetInstance()->Get_Object_Picking_Point();

    if (_CurrentClickObject == 0)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_AlterBase"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : AlterBase");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 1)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mushroom"), LEVEL_GAMEPLAY, TEXT("Layer_Plant"), &pDesc)))
        {
            MSG_BOX("Failed to install : Mushroom");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 2)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Elemental_Boss_02"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SC_Elemental_Boss_02");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 3)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Temple_Ground"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Temple_Ground");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 4)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleBrick_04"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SM_TempleBrick_04 ");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 5)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Temple_Block"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Temple_Block");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 6)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Steps_A_Mod"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SC_Steps_A_Mod");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 7)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Corner_D_Mod"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SC_Corner_D_Mod");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 8)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_01"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : M_GuardianP_POSE_A_SC_Rock_01");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 9)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_02"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : M_GuardianP_POSE_A_SC_Rock_02");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 10)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_01_Weapon"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : M_GuardianP_POSE_A_SC_Rock_01_Weapon");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 11) 
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Brick_v1"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SC_Brick_v1");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 12)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleBrick_10"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SM_TempleBrick_10");
            return E_FAIL;  
        }
    }

    else if (_CurrentClickObject == 13) 
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Brick_W_B_Mod2"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SM_Brick_W_B_Mod2");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 14)
    {
    
        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Water"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_Water");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 15)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_Guardian_Model_C"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_M_Guardian_Model_C");
            return E_FAIL;
        }
    }

    return S_OK;
}



HRESULT CImgui_ObjectTool::InstallFromObject(_int _CurrentClickObject)
{
    CGameObject::GAMEOBJECT_DESC pDesc{};
  
    pDesc.fPostion = m_fMeshPickingPoint;  
    /* 크기를 설정해야하는데 ;;*/
    /**/


    if (_CurrentClickObject == 0)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_AlterBase"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : AlterBase");
            return E_FAIL;
        }

    }

    else if (_CurrentClickObject == 1)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mushroom"), LEVEL_GAMEPLAY, TEXT("Layer_Plant"), &pDesc)))
        {
            MSG_BOX("Failed to install : Mushroom");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 2)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Elemental_Boss_02"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SC_Elemental_Boss_02");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 3)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Temple_Ground"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Temple_Ground");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 4)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleBrick_04"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SM_TempleBrick_04");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 5)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Temple_Block"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Temple_Block");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 6)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Steps_A_Mod"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SC_Steps_A_Mod");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 7)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Corner_D_Mod"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SC_Corner_D_Mod");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 8)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_01"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : M_GuardianP_POSE_A_SC_Rock_01");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 9)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_02"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : M_GuardianP_POSE_A_SC_Rock_02");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 10)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_01_Weapon"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : M_GuardianP_POSE_A_SC_Rock_01_Weapon");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 11)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Brick_v1"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SC_Brick_v1"); 
            return E_FAIL;  
        }
    }


    else if (_CurrentClickObject == 12) 
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleBrick_10"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : SM_TempleBrick_10");   
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 13)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Brick_W_B_Mod2"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc))) 
        {
            MSG_BOX("Failed to install : SM_Brick_W_B_Mod2 ");   
            return E_FAIL;  
        }
    }
   
    else if (_CurrentClickObject == 14)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Water"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Water ");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 15)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_Guardian_Model_C"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))    
        {
            MSG_BOX("Failed to install : Prototype_GameObject_M_Guardian_Model_C"); 
            return E_FAIL;
        }
    }



    else if (_CurrentClickObject == 16)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_LotusFlower_07a1"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SM_LotusFlower_07a1");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 17)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_ST_LiftTable_a2_sui_00");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 18)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleBrick_01"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SM_TempleBrick_01");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 19)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Brick_W_B_Mod"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))  
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SC_Brick_W_B_Mod");   
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 20)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Brick_W_B_Mod_10"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SC_Brick_W_B_Mod_10");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 21)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_Guardian_Model_D"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_M_Guardian_Model_D");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 22)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Templelamp05"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SM_Templelamp05");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 23)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Templelamp06"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SM_Templelamp06");
            return E_FAIL;
        }
    }



    else if (_CurrentClickObject == 24)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_FlapTrap_03"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SC_FlapTrap_03");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 25)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Stone_HeavyBOX_A_Mod"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SC_Stone_HeavyBOX_A_Mod");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 26)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TurnTable_Body"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_TurnTable_Body");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 27)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TurnTable_Head"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_TurnTable_Head");
            return E_FAIL;
        }
    }



    else if (_CurrentClickObject == 28)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Box_Lift"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_Box_Lift");   
            return E_FAIL;  
        }
    }


    else if (_CurrentClickObject == 29)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_LeverButton"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_LeverButton");
            return E_FAIL;  
        }
    }




    else if (_CurrentClickObject == 30)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_TempleDoor_05"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SC_TempleDoor_05");
            return E_FAIL;
        }
    }




    else if (_CurrentClickObject == 31)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleFloor_02"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SM_TempleFloor_02");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 32)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SC_Object_StoneBall");
            return E_FAIL;
        }
    }



    else if (_CurrentClickObject == 33)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_StrongMan"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc))) 
        {
            MSG_BOX("Failed to install : Prototype_GameObject_StrongMan");
            return E_FAIL;
        }
    }



    else if (_CurrentClickObject == 34)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_StepBox"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SC_StepBox");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 35)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_TotemB"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SC_TotemB");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 36)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Pillar_01"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SM_Pillar_01");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 37)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_WaterTemple_Parts_b"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SC_WaterTemple_Parts_b");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 38)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_RuneStatue"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SC_RuneStatue");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 39)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Elemental_Boss_04"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SC_Elemental_Boss_04");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 40)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Elemental_Boss_05"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SC_Elemental_Boss_05");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 41)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Wind_EleTower_09"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_Wind_EleTower_09");
            return E_FAIL;
        }
    }
    
    
    else if (_CurrentClickObject == 42)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Wind_Elementak_Boss"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_Wind_Elementak_Boss");
            return E_FAIL;
        }
    }


    else if (_CurrentClickObject == 43)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_Boss");
            return E_FAIL;
        }
    }
    
    else if (_CurrentClickObject == 44)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_S_HpFlower"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_S_HpFlower");
            return E_FAIL;
        }
    }

    else if (_CurrentClickObject == 45)
    {

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_MushroomA_L"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &pDesc)))
        {
            MSG_BOX("Failed to install : Prototype_GameObject_SM_MushroomA_L");
            return E_FAIL;
        }
    }


    return S_OK;
}


void CImgui_ObjectTool::Free()  
{
    __super::Free();    

    for (auto& pSRV : m_Textures)   
        Safe_Release(pSRV);

    for (auto& pMap : m_mapPickedMesh)
    {
        delete[] pMap.second.pVertices;
        delete[] pMap.second.Indices;
    }

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pGameInstance);
}