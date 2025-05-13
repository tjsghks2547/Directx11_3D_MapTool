#include "pch.h"
#include "Imgui_TerrainTool.h"

CImgui_TerrainTool::CImgui_TerrainTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice)
	,m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CImgui_TerrainTool::Init()
{

	


	return S_OK;
}

HRESULT CImgui_TerrainTool::Update()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Terrain"))
		{
			if (ImGui::MenuItem("Terrain Tool Open", nullptr, m_bTerrainWindowOnOff))
			{

				m_bTerrainWindowOnOff = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}


	if (m_bTerrainWindowOnOff)
	{
		if (ImGui::Begin("Terrain Tool Window", &m_bTerrainWindowOnOff))
		{
			//ImGui::Text("Terrain 설정.");
			//ImGui::Button("Example Button");

			ImGui::Checkbox("Terrain_Picking", &m_bPickingTile);

		}

		ImGui::End(); // 항상 호출해야 함!
	}

	return S_OK;	
}

CImgui_TerrainTool* CImgui_TerrainTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CImgui_TerrainTool* pInstance = new CImgui_TerrainTool(pDevice, pContext);

	if(FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed to Created : Imgui_TerrainTool");
		Safe_Release(pInstance);
	}

	return pInstance;	
}

void CImgui_TerrainTool::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
