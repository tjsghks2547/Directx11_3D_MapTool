#include "pch.h"
#include "ImguiManager.h"
#include "Client_Defines.h"
#include "Imgui_File.h"
#include "Imgui_Navigation.h"





IMPLEMENT_SINGLETON(CImguiManager)


CImguiManager::CImguiManager()
{
}

CImguiManager::~CImguiManager()
{
}

void CImguiManager::Init(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pGraphicDeviceContext)
{
	{
		m_pDevice = pGraphicDev;	
		m_pContext = pGraphicDeviceContext;	
		m_pGameInstance = CGameInstance::GetInstance();

		Safe_AddRef(m_pDevice);	
		Safe_AddRef(m_pContext);	
		Safe_AddRef(m_pGameInstance);
	}
	
	IMGUI_CHECKVERSION();					
	ImGui::CreateContext();			
	ImGuiIO& io = ImGui::GetIO(); (void)io;			
	
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls	
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();	
	
	ImGui_ImplWin32_Init(g_hWnd);	
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	/* 폰트파일이 없다고 뜸 */
	io.Fonts->AddFontFromFileTTF("../TTF/Pretendard-Regular.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());	

	ImGui::SetNextWindowSize(ImVec2(800, 800)); // Imgui 창 크기	
	ImGui::SetNextWindowPos(ImVec2(0, 0));	

	ZeroMemory(&m_RasterDesc, sizeof(D3D11_RASTERIZER_DESC));	


#pragma region Imgui_File 객체 생성
	m_pImguiFileMenu = CImgui_File::Create();
	m_pImguiFileMenu->Init();
#pragma endregion 


#pragma region Object Tool 객체 생성
	m_pImguiObjectTool = CImgui_ObjectTool::Create(m_pDevice, m_pContext);
#pragma endregion 


#pragma region Terrain Tool 객체 생성
	m_pImguiTerrainTool = CImgui_TerrainTool::Create(m_pDevice, m_pContext);
#pragma endregion


#pragma region Navigation Tool 객체 생성 
	m_pImguiNavigation = CImgui_Navigation::Create(m_pDevice, m_pContext,m_pImguiObjectTool);
	//m_pImguiNavigation->Initialize();	
#pragma endregion 

}

void CImguiManager::Update()
{
	// DX11 IMGUI update 

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	

	if (ImGui::Begin("Map Editor", NULL, ImGuiWindowFlags_MenuBar))
	{
		// 여기에 윈도우 내부에 그릴 내용을 추가합니다.
		//ImGui::Text(u8"ㅋㅋㅋ 이새끼 또 딴생각하네 집중하라고"); // 텍스트 표시	
		

		/* Imgui 파일 메뉴*/
		m_pImguiFileMenu->Update();

		/* Imgui Object 메뉴*/
		m_pImguiObjectTool->Update();	

		/* Imgui Terrain 메뉴*/
		m_pImguiTerrainTool->Update();

		/* Imgui Navigation 메뉴*/
		m_pImguiNavigation->Update();			


		ImGui::Checkbox("WireFrameMode_On/Off", &WireFrameOnOff);
		if(WireFrameOnOff)
		{
			
			m_RasterDesc.FillMode = D3D11_FILL_WIREFRAME; // 와이어프레임 모드
			m_RasterDesc.CullMode = D3D11_CULL_BACK;      // 후면 컬링
			m_RasterDesc.DepthClipEnable = true;

			m_pDevice->CreateRasterizerState(&m_RasterDesc, &m_pRasterRiseState);

			// 3. 그래픽 장치 문맥에 설정
			m_pContext->RSSetState(m_pRasterRiseState);

			Safe_Release(m_pRasterRiseState);

		}
		else if (!WireFrameOnOff)
		{
			
			m_RasterDesc.FillMode = D3D11_FILL_SOLID;     // 와이어프레임 모드 해제
			m_RasterDesc.CullMode = D3D11_CULL_BACK;      // 후면 컬링
			m_RasterDesc.DepthClipEnable = true;

			m_pDevice->CreateRasterizerState(&m_RasterDesc, &m_pRasterRiseState);

			// 3. 그래픽 장치 문맥에 설정
			m_pContext->RSSetState(m_pRasterRiseState);

			Safe_Release(m_pRasterRiseState);
		}

		
	}
	//ImGui::Begin("Sample Window"); // 창 시작	
	//ImGui::Text("Hello, world!"); // 텍스트 표시	
	ImGui::End(); // 창 종료	

}

void CImguiManager::Render()
{
	ImGui::Render();		
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	m_pImguiNavigation->Render();
	ImGui::EndFrame();		
}

void CImguiManager::Free()	
{

	ImGui_ImplWin32_Shutdown();	
	ImGui_ImplDX11_Shutdown();	

	ImGui::DestroyContext();
	
	Safe_Release(m_pImguiFileMenu);
	Safe_Release(m_pImguiObjectTool);
	Safe_Release(m_pImguiTerrainTool);
	Safe_Release(m_pImguiNavigation);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);	
	Safe_Release(m_pContext);	
	
}
