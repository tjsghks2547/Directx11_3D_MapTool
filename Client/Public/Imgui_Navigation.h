#pragma once

#include "Base.h"

class CImgui_ObjectTool; 

BEGIN(Engine)	
class CNavigation;	
class CGameInstance;
class CCube; 
class CCell;
END	

class CImgui_Navigation : public CBase  
{
private:
	CImgui_Navigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CImgui_ObjectTool* pObjectTool);	
	virtual ~CImgui_Navigation() = default; 


public:
	virtual HRESULT Initialize();	
	virtual void Update();	
	virtual HRESULT Render();	

public:
	CNavigation* Get_Navigation() { return m_pNavigation; }


private:
	ID3D11Device*		 m_pDevice  = { nullptr };		
	ID3D11DeviceContext* m_pContext = { nullptr };	

	CImgui_ObjectTool*	 m_pObjectTool = { nullptr };
	vector<_float3>  	 m_vecPickingPoint;

private:
	_bool				 m_bshowNavigationToolWindow = { false };	
	_bool				 m_bNavigationTool = { false };
	_bool				 m_bDeleteCellTool = { false };
	_bool				 m_bDeleteCube     = { false };
	_bool			     m_bTransformMesh  = { false };
	_bool				 m_bWireFrameCellOnOff = { false };
	_bool				 m_bWireFrameCubeOnOff = { false };
	
	VTXPOS*			     m_pCurrentSelectedPoint      = { nullptr };
	ID3D11Buffer*	     m_pCurrentSelectedCellBuffer = { nullptr };
	CCube*				 m_pCurrentSelectedCube       = { nullptr };


private:
	CNavigation*				m_pNavigation    =  { false };	
	CGameInstance*				m_pGameInstance  =  { nullptr };		

private:
	void		         IntersectRay(class CCube* _pCube);
	_bool				 IntersectCell(class CCell* _pCell);
	_bool				 ChecKIntersect();
	

	VTXPOS*				 FindPoint(_float3 point);	/* 매개변수로 마우스 매쉬 피킹 클릭지점*/

	map<float, CCube*>   m_mapCube;
	map<float, CCell*>   m_mapCell; 



public:
	static CImgui_Navigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CImgui_ObjectTool* pObjectTool);	
	virtual void Free() override ;	 

};

