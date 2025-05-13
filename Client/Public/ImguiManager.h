#pragma once


#include "Base.h"
#include "GameInstance.h"
#include "Imgui_TerrainTool.h"
#include "Imgui_ObjectTool.h"

class CImgui_File;
class CImgui_ObjectTool;
class CImgui_Navigation;	


class CImguiManager : public CBase
{
	DECLARE_SINGLETON(CImguiManager)

private:
	CImguiManager();
	virtual ~CImguiManager(); 

public:

	void Init(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pGraphicDeviceContext);	
	void Update();
	void Render(); 
	void Free(); 

public:
	ID3D11Device*		   Get_Device()        { return m_pDevice; }
	ID3D11DeviceContext*   Get_DeviceContext() { return m_pContext; }
	CGameInstance*		   Get_GameInstance()  { return m_pGameInstance; }

private:

	ID3D11Device*			m_pDevice		   = { nullptr };		
	ID3D11DeviceContext*	m_pContext		   = { nullptr };	
	CGameInstance*			m_pGameInstance    = { nullptr };

	
	CImgui_File*			 m_pImguiFileMenu    = { nullptr };
	CImgui_ObjectTool*       m_pImguiObjectTool  = { nullptr };
	CImgui_TerrainTool*		 m_pImguiTerrainTool = { nullptr };
	CImgui_Navigation*		 m_pImguiNavigation  = { nullptr };

public:
	CImgui_Navigation* Get_ImguiNavigationTool()
	{
		return m_pImguiNavigation;	
	}

	CImgui_ObjectTool*  Get_ImguiObjectTool()
	{
		return m_pImguiObjectTool;		
	}

	_float3 Get_Object_Picking_Point()
	{
		return  m_fPicking_Point;
	}

	_bool Get_Object_Picking_bool()
	{
		return  m_pImguiObjectTool->Get_Picking_Object_bool();
	}

	void Set_Object_Picking_Point(_float _x, _float _y, _float _z)
	{
		m_fPicking_Point.x = _x;
		m_fPicking_Point.y = _y;
		m_fPicking_Point.z = _z;
	}

	bool  Get_TerrainPicking_bool() 
	{ 
		return m_pImguiTerrainTool->Get_PickingTileBool();	
	}

	void Set_PickingPoint(_float3 _point1, _float3 _point2, _float3 _point3) 
	{
		m_fPicking_Trianle_Point[0] = &_point1;
		m_fPicking_Trianle_Point[1] = &_point2;
		m_fPicking_Trianle_Point[2] = &_point3;
	}
	
	_float3** Get_Triangle_Pont() { return m_fPicking_Trianle_Point; }


	void Set_MousePicking(bool _bool)
	{
		m_bMousePicking = _bool;
	}

	_bool Get_MousePicking()
	{
		return m_bMousePicking;
	}

	_float3 Get_Picking_Scale()
	{
		return m_pImguiObjectTool->Get_Mesh_Scale();
	}

	

private:
	D3D11_RASTERIZER_DESC  m_RasterDesc = {};
	ID3D11RasterizerState* m_pRasterRiseState = { nullptr };
	
	
	bool    WireFrameOnOff = { false };
	/* 마우스 피킹이 눌렸는지 확인하기 위한 작업*/
	bool    m_bMousePicking = { false };


	_float3* m_fPicking_Trianle_Point[3];
	_float3  m_fPicking_Point = {};
	

};

