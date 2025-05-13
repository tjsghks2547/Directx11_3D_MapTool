#include "pch.h"
#include "Imgui_File.h"
#include "Client_Defines.h"
#include "ImguiManager.h"
#include <Commdlg.h>
#include "Layer.h"
#include "GameObject.h"
#include "Imgui_Navigation.h"
#include "Navigation.h"
#include "Cell.h"



CImgui_File::CImgui_File()
	:m_pGameInstance(CGameInstance::GetInstance())
	,m_pDevice(CImguiManager::GetInstance()->Get_Device())
	,m_pContext(CImguiManager::GetInstance()->Get_DeviceContext())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CImgui_File::Init()
{
	m_pLayer = m_pGameInstance->Get_Layers();	

	return S_OK;
}

HRESULT CImgui_File::Update()
{
	if(ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem("Map Open", nullptr,m_bMapOpenMenu))
			{
				MapFile_Read();
			}

			if(ImGui::MenuItem("Map Save", nullptr, m_bMapOpenMenu))
			{
				MapFile_Save();	
			}

			if (ImGui::MenuItem("Navigation Open", nullptr, m_bMapOpenMenu))
			{
				NavigationFile_Read();	
			}

			if (ImGui::MenuItem("Navigation Save", nullptr, m_bMapOpenMenu))
			{
				NavigationFile_Save();
			}
			

			if(ImGui::MenuItem("Height Map Open", nullptr, m_bHeightMapOpenMenu))
			{

				//m_bFileOpenMenu = true;	
				HeightMapFile_Read();
				
				/* �븻�ʵ� ���� �о�;���*/
			
			}


			if (ImGui::MenuItem("Height Map Save ", nullptr, m_bHeightMapSaveMenu))
			{

				HeightMapFile_Save();

				/* �븻 �ʵ� ���� �����ؾ���*/

				//m_bFileOpenMenu = true;	
				//File_Read();

			}

			ImGui::EndMenu();
		}


		ImGui::EndMenuBar();
	}
	
	return S_OK;
}



HRESULT CImgui_File::MapFile_Read()
{
	OPENFILENAME ofn;       // OPENFILENAME ����ü ����	
	wchar_t szFile[MAX_PATH] = {};   // ���� �̸��� ������ ���� (�ִ� 260��)	

	// ����ü �ʱ�ȭ
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // �θ� ������ �ڵ�	
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';  // �ʱ� ���� �̸� (�� ���ڿ�)	
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Binary Files\0*.bin\0All Files\0*.*\0";// ���� ���� ����	
	ofn.nFilterIndex = 1;      // �⺻ ���� ���� ����
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"..\\";  // �ʱ� ���丮
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;


	if (GetOpenFileName(&ofn))
	{
		_tchar szEXT[MAX_PATH] = {};

		_ulong dwByte = {};
		/* bin �����Ͻ� �ƴҽ� ���ǹ� �ɱ� */
		_wsplitpath_s(szFile, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

	
			HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

			/* ���� �ϳ��ϳ� ������ ������*/
			
		while(true)
		{
			_tchar Layer_Name[MAX_PATH] = {}; //���̾� �̸�										
			ReadFile(hFile, Layer_Name, MAX_PATH, &dwByte, nullptr);		
			
			if (dwByte == 0)	
				break;	
			/* �̸� �۾� */
			_char   Prototype_Name[MAX_PATH] = {};			
			
			ReadFile(hFile, Prototype_Name, MAX_PATH, &dwByte, nullptr);				
			

			_float4x4 WorldMatrix = {};
			ReadFile(hFile, &WorldMatrix, sizeof(_float4x4), &dwByte, nullptr);		
			//int a = 4;

			_tchar Translate_wchar[MAX_PATH] = {};	
			MultiByteToWideChar(CP_ACP, 0, Prototype_Name, strlen(Prototype_Name), Translate_wchar, MAX_PATH);	
			
			/* ���� TRANSFORM�� �ǵ�� �ɵ���.*/
			//int b = 4;
			if(FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, Translate_wchar, LEVEL_GAMEPLAY, Layer_Name)))
				return E_FAIL;	
			
			//CTransform* pTrasnform = dynamic_cast<CTransform*>(
			//* Find GaemObject ������ �ҵ�
			// ������ �� �ִ� ����� �����غ����ҵ�.
			map<const _wstring, class CLayer*>* Level_Layers = m_pGameInstance->Get_Layers();
			
			auto& Level_GamePlay = Level_Layers[3];	
			
			for(auto& Layers : Level_GamePlay)
			{
				//auto& iter = find(Level_GamePlay.begin(), Level_GamePlay.end(), Layer_Name);	
				auto iter = Level_GamePlay.find(Layer_Name);	

				if ( iter == Level_GamePlay.end())
					return E_FAIL;
					
				else 
				{
					CTransform* pTranform  = dynamic_cast<CTransform*>(
					iter->second->Get_GameObject_List().back()->Find_Component(TEXT("Com_Transform")));

					pTranform->Set_WorldMatrix(WorldMatrix);	
				}
			}
		}
			CloseHandle(hFile);	
	}
		

	return S_OK;
	
}

HRESULT CImgui_File::MapFile_Save()
{

	OPENFILENAME ofn;           // OPENFILENAME ����ü ����		
	wchar_t szFile[MAX_PATH] = {};   // ���� �̸��� ������ ���� (�ִ� 260��)		

	// ����ü �ʱ�ȭ
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // �θ� ������ �ڵ�		
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';  // �ʱ� ���� �̸� (�� ���ڿ�)		
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Binary Files\0*.bin\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;      // �⺻ ���� ���� ����
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"..\\";  // �ʱ� ���丮
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;


	if (GetSaveFileName(&ofn))
	{
		/* ���� �����ҷ��� �̸��� �ʿ�*/
		/* �ϴ� Ȯ���� �˻��ϱ� Height ���� bmp���Ϸ� �����ؾ� �ϴϱ�*/
	
		HANDLE hFile = CreateFile(ofn.lpstrFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		_ulong dwByte = {};	

		/* ���⼭�� �׷��� �ش� �̸��� transformMatrix�� �����ؾ��Ѵ�.*/

		/* �ϴ� �׷��� GaemPlay ���� �������� */
		auto& Layer = m_pLayer[3];

		/* ���� �ش� GamePlay�ȿ� ���̾�� �ȿ� �ִ� ��ü���� ��ȯ�ϸ鼭 ����*/
		for (auto& Layers : m_pLayer[3])	// �̰� GAMEPLAY_LEVEL
		{
			/* ���̾� �̸� �����ϱ� */
			const _tchar* Layer_Name = Layers.first.c_str();			
			

			if (0 != lstrcmp(Layer_Name, TEXT("FreeCamera"))
				&& 0 != lstrcmp(Layer_Name, TEXT("Layer_Terrain"))		
				&& 0 != lstrcmp(Layer_Name, TEXT("Layer_Player")))// ���⼭ ���̾�( ī�޶�, �÷��̾� , �������� ���ֱ�)
			{
				for (auto& GameObject : Layers.second->Get_GameObject_List())
				{
					
					WriteFile(hFile, Layer_Name, MAX_PATH, &dwByte, nullptr);			

					//_tchar* Object_Name = TEXT("");	
					//MultiByteToWideChar(CP_ACP, 0, GameObject->Get_Name(), strlen(GameObject->Get_Name()), Object_Name, strlen(GameObject->Get_Name()) + 1);	
					
					_char  Object_Name[MAX_PATH];
					memcpy(&Object_Name, GameObject->Get_Name(), sizeof(_char)*MAX_PATH);		

					WriteFile(hFile, Object_Name, MAX_PATH, &dwByte, nullptr); // �̰� ������Ʈ Prototype_GameObject_Mushroom �̰���
			
					_float4x4 Worldmatrix = {}; // ��� �ɹ��� 0���� �ʱ�ȭ
					CTransform* pTransform = dynamic_cast<CTransform*>(GameObject->Find_Component(TEXT("Com_Transform")));
					memcpy(&Worldmatrix, pTransform->Get_WorldFloat4X4(), sizeof(_float4x4));
					WriteFile(hFile, &Worldmatrix, sizeof(_float4x4), &dwByte, nullptr);

					int a = 4;
				}
			}
		}

		CloseHandle(hFile);
		MSG_BOX("������ ���������� ����Ǿ����ϴ�.");
	}

	return S_OK;
}

HRESULT CImgui_File::HeightMapFile_Read()
{
	OPENFILENAME ofn;       // OPENFILENAME ����ü ����	
	wchar_t szFile[MAX_PATH] = {};   // ���� �̸��� ������ ���� (�ִ� 260��)	

	// ����ü �ʱ�ȭ
	ZeroMemory(&ofn, sizeof(ofn));	
	ofn.lStructSize = sizeof(ofn);	
	ofn.hwndOwner = g_hWnd;   // �θ� ������ �ڵ�	
	ofn.lpstrFile = szFile;	
	ofn.lpstrFile[0] = '\0';  // �ʱ� ���� �̸� (�� ���ڿ�)	
	ofn.nMaxFile = sizeof(szFile);	
	ofn.lpstrFilter = L"BMP Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";// ���� ���� ����
	ofn.nFilterIndex = 1;      // �⺻ ���� ���� ����
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"..\\";  // �ʱ� ���丮
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;





	if(GetOpenFileName(&ofn))
	{
		_tchar szEXT[MAX_PATH] = {};

		/* bmp �����Ͻ� �ƴҽ� ���ǹ� �ɱ� */
		_wsplitpath_s(szFile, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);
		
		if (false == lstrcmp(szEXT, TEXT(".bmp"))) /* BMP�� ������ FALSE�� ��ȯ��*/
		{
			/*���⼭ ���� �ϳ��ϳ� ���� ������*/
			
			HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			
			_ulong				dwByte = {};
			BITMAPFILEHEADER	fh{};
			ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, 0);

			BITMAPINFOHEADER	ih{};
			ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, 0);	
			
			_uint Total_Count = ih.biWidth * ih.biHeight;

			_uint* pPixel = new _uint[Total_Count];
			ReadFile(hFile, pPixel, sizeof(_uint) * Total_Count, &dwByte, nullptr);

			/* ���� �ȼ��� �� ������.*/
			/* �̰Ÿ� ���� ���̴��� �־����� */
			/* �ش� ���̴��� �����;���.*/
			
			CComponent* pComponents = m_pGameInstance->Find_Component(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), TEXT("Terrain_Com"));	
			
			if(pComponents != nullptr)
			{
				/*CShader* pShader = static_cast<CShader*>(pComponents);*/
				/* ���⼭ �ǵ���*/
				
				CVIBuffer_Terrain* pTerrain = static_cast<CVIBuffer_Terrain*>(pComponents);
				

#pragma region ���ؽ� ����  �� �ε��� ���� ���� 				
				/* pBuffer ���� ���� map , unmap�ؼ� �ٷ���*/
				D3D11_MAPPED_SUBRESOURCE mapped_VB_Resoruce;
				ZeroMemory(&mapped_VB_Resoruce, sizeof(D3D11_MAPPED_SUBRESOURCE));	

				D3D11_MAPPED_SUBRESOURCE mapped_IB_Resoruce;
				ZeroMemory(&mapped_IB_Resoruce, sizeof(D3D11_MAPPED_SUBRESOURCE));

				HRESULT hr = m_pContext->Map(pTerrain->Get_VB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_VB_Resoruce);
				if (FAILED(hr))
					return E_FAIL;	

				HRESULT hr2 = m_pContext->Map(pTerrain->Get_IB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_IB_Resoruce);
				if (FAILED(hr2))
					return E_FAIL;
				
				//��������
				VTXNORTEX* pVertices = reinterpret_cast<VTXNORTEX*>(mapped_VB_Resoruce.pData);	
				//�ε�������
				_uint*     pIndices = reinterpret_cast<_uint*>(mapped_IB_Resoruce.pData);


				/* ���ؽ� ���ۿ� ����(y)�� �ֱ�*/
				for(size_t i = 0; i<Total_Count; i++)
				{
					pVertices[i].vPosition.y = (pPixel[i] & 0x000000ff)/10.f;
				}

				/* Normal vector �����*/

				size_t dwCntZ = pTerrain->Get_Height_Z();
				size_t dwCntX = pTerrain->Get_Width_X();

				_ulong index = { 0 };

				for(size_t i = 0; i< dwCntZ -1; i++)
				{
					for (size_t j = 0; j < dwCntX - 1; j++) 
					{
						
						_uint iIndex = i * dwCntX + j;

						_uint iIndices[4] =
						{
							iIndex + dwCntX,       /* ���� �� */
							iIndex + dwCntX + 1,   /* ������ ��*/
							iIndex + 1,			   /* ������ �Ʒ�*/
							iIndex,                /* ���� �Ʒ�*/
						};

						_vector vSour, vDest, vNormal;

						pIndices[index++] = iIndices[0];
						pIndices[index++] = iIndices[1];
						pIndices[index++] = iIndices[2];

						vSour = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
						vDest = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
						vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

						XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
						XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
						XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);

						pIndices[index++] = iIndices[0];
						pIndices[index++] = iIndices[2];
						pIndices[index++] = iIndices[3];

						vSour = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
						vDest = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
						vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

						XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
						XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
						XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);

					}
				}

				for (size_t i = 0; i < Total_Count; i++)
				{
					XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
				}

				/* ���� ���⼭ ������� .*/
				m_pContext->Unmap(pTerrain->Get_VB_Buffer(), 0);
				m_pContext->Unmap(pTerrain->Get_IB_Buffer(), 0);

				Safe_Delete_Array(pPixel);


#pragma endregion ���ؽ� ���� ���� 

				//pShader->Bind_Matrix() 
			}


		}
		int a = 4;
	}

	return S_OK;	
}

HRESULT CImgui_File::HeightMapFile_Save()
{
	OPENFILENAME ofn;       // OPENFILENAME ����ü ����	
	wchar_t szFile[MAX_PATH] = {};   // ���� �̸��� ������ ���� (�ִ� 260��)	

	// ����ü �ʱ�ȭ
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // �θ� ������ �ڵ�	
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';  // �ʱ� ���� �̸� (�� ���ڿ�)	
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"BMP Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";// ���� ���� ����
	ofn.nFilterIndex = 1;      // �⺻ ���� ���� ����
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"..\\";  // �ʱ� ���丮
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;


	if (GetSaveFileName(&ofn))
	{
		/* ���� �����ҷ��� �̸��� �ʿ�*/
		/* �ϴ� Ȯ���� �˻��ϱ� Height ���� bmp���Ϸ� �����ؾ� �ϴϱ�*/
		_tchar szEXT[MAX_PATH] = {};
		_wsplitpath_s(ofn.lpstrFile, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);
		

		// ���� Ȯ���ڰ� bmp�� �ƴϸ�, Ȯ���ڸ� .bmp�� ����
		//if (_wcsicmp(szEXT, L".bmp") != 0)	
		//{
		//	
		//	wcscat_s(ofn.lpstrFile, L".bmp"); // Ȯ���� .bmp �߰�		
		//}

		HANDLE hFile = CreateFile(ofn.lpstrFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		

		BITMAPFILEHEADER FileHeader{};
		ZeroMemory(&FileHeader, sizeof(BITMAPFILEHEADER));
		
		BITMAPINFOHEADER InfoHeader{};
		ZeroMemory(&InfoHeader, sizeof(BITMAPINFOHEADER));

		FileHeader.bfType = 0x4D42; // 'BM' ���� Ÿ�� 
		FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // ���� �������� ���� ��ġ
		
		
		CComponent* pComponent = m_pGameInstance->Find_Component(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), TEXT("Terrain_Com"));
		CVIBuffer_Terrain* pTerrain = static_cast<CVIBuffer_Terrain*>(pComponent);

		// ��Ʈ�� ���� ����
		InfoHeader.biSize   = sizeof(BITMAPINFOHEADER);	
		InfoHeader.biWidth  = pTerrain->Get_Width_X(); 
		InfoHeader.biHeight = pTerrain->Get_Height_Z();
		InfoHeader.biPlanes = 1;
		InfoHeader.biBitCount = 32;
		InfoHeader.biCompression = BI_RGB;
		InfoHeader.biSizeImage = InfoHeader.biWidth * InfoHeader.biHeight * 4;  // ������ ũ�� ���

		// ��� �ۼ� 
		DWORD dwBytesWritten;
		WriteFile(hFile, &FileHeader, sizeof(FileHeader), &dwBytesWritten, nullptr);
		WriteFile(hFile, &InfoHeader, sizeof(InfoHeader), &dwBytesWritten, nullptr);


		_uint Total_Count = InfoHeader.biWidth * InfoHeader.biHeight;

		_uint* pPixelData = new _uint[Total_Count];	
		
		D3D11_MAPPED_SUBRESOURCE mappedResource; 
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HRESULT hr = m_pContext->Map(pTerrain->Get_VB_Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
			return E_FAIL;	
		
		VTXNORTEX* pVertices = reinterpret_cast<VTXNORTEX*>(mappedResource.pData);		
			
		for(size_t i =0; i<Total_Count; i++)
		{
			BYTE alpha = 255;  // Alpha �� (255�� ������)	
			BYTE red = static_cast<BYTE>(pVertices[i].vPosition.y * 10.f);  // Red ��	
			BYTE green = 0;  // Green �� (���÷� 0)	
			BYTE blue = 0;   // Blue �� (���÷� 0)	

			//pPixelData[i] = pVertices->vPosition.y * 20.f;
			pPixelData[i] = (alpha << 24) | (red << 16) | (red << 8) | red;	 /* rgb�� �������̴� �̷��� ��Ʈ�����Ѵ�.*/
		}

		
		m_pContext->Unmap(pTerrain->Get_VB_Buffer(), 0);
		
		WriteFile(hFile, pPixelData, Total_Count * sizeof(_float), &dwBytesWritten, nullptr);


		Safe_Delete_Array(pPixelData);
		CloseHandle(hFile);

		MSG_BOX("������ ���������� ����Ǿ����ϴ�.");
		/* ���⼭ ���� �ٽ� �ۼ�����!*/

	}
		
	return S_OK;
}

HRESULT CImgui_File::NavigationFile_Read()
{
	OPENFILENAME ofn;       // OPENFILENAME ����ü ����	
	wchar_t szFile[MAX_PATH] = {};   // ���� �̸��� ������ ���� (�ִ� 260��)	

	// ����ü �ʱ�ȭ
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // �θ� ������ �ڵ�	
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';  // �ʱ� ���� �̸� (�� ���ڿ�)	
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Binary Files\0*.bin\0All Files\0*.*\0";// ���� ���� ����	
	ofn.nFilterIndex = 1;      // �⺻ ���� ���� ����
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"..\\";  // �ʱ� ���丮
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;


	if (GetOpenFileName(&ofn))
	{
		_tchar szEXT[MAX_PATH] = {};

		_ulong dwByte = {};
		/* bin �����Ͻ� �ƴҽ� ���ǹ� �ɱ� */
		_wsplitpath_s(szFile, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);


		HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		CImgui_Navigation* pNavigationTool = CImguiManager::GetInstance()->Get_ImguiNavigationTool();	

		
		while (true)
		{
			_float3 vPoints[3] = {};	

			ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);	

			if (dwByte == 0)
				break;

			pNavigationTool->Get_Navigation()->Add_Cell(vPoints);
		}
		
		

		MSG_BOX("������ ���������� �ҷ��Խ��ϴ�.");	
		CloseHandle(hFile);	
	}


	return S_OK;

}

HRESULT CImgui_File::NavigationFile_Save()
{
	OPENFILENAME ofn;           // OPENFILENAME ����ü ����		
	wchar_t szFile[MAX_PATH] = {};   // ���� �̸��� ������ ���� (�ִ� 260��)		

	// ����ü �ʱ�ȭ
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // �θ� ������ �ڵ�		
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';  // �ʱ� ���� �̸� (�� ���ڿ�)		
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Binary Files\0*.bin\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;      // �⺻ ���� ���� ����
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"..\\";  // �ʱ� ���丮
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;


	if (GetSaveFileName(&ofn))
	{
		/* ���� �����ҷ��� �̸��� �ʿ�*/
		/* �ϴ� Ȯ���� �˻��ϱ� Height ���� bmp���Ϸ� �����ؾ� �ϴϱ�*/

		HANDLE hFile = CreateFile(ofn.lpstrFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		_ulong dwByte = {};

		/* ���⼭�� �׷��� �ش� �̸��� transformMatrix�� �����ؾ��Ѵ�.*/
		CImgui_Navigation*  pNavigationTool = CImguiManager::GetInstance()->Get_ImguiNavigationTool();

		for(auto& iter : pNavigationTool->Get_Navigation()->Get_VecCell())
		{
			_float3 vPoints[3] = {};

			vPoints[0] = iter->Get_Points()[0];
			vPoints[1] = iter->Get_Points()[1];
			vPoints[2] = iter->Get_Points()[2];
			
			WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);	

		}

		CloseHandle(hFile);
		MSG_BOX("������ ���������� ����Ǿ����ϴ�.");
	}

	return S_OK;


}


CImgui_File* CImgui_File::Create()
{
	CImgui_File* pInstance = new CImgui_File();
	
	if(FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed to Created : Imgui_File_Menu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_File::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
