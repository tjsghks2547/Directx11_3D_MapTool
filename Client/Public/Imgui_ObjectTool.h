#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CGameInstance;
class CTexture;	
class CLayer;
class CTransform;
class CGameObject;
class CMesh;
END

class CImgui_ObjectTool : public CBase
{
private:
	CImgui_ObjectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImgui_ObjectTool() = default;

	
public: 
	HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	HRESULT Update(); 

private:
	struct Picked_Object_Info	
	{
		CGameObject* pGamObject;	
		CMesh*		 _pMesh;	
		VTXMESH*	 pVertices;	
		_uint*		 Indices;
		CTransform*  pTransform; 
		CLayer*		 pLayer;
	};

private:
	ID3D11Device*		  m_pDevice	           = { nullptr };
	ID3D11DeviceContext*  m_pContext	       = { nullptr };
	CGameInstance*		  m_pGameInstance      = { nullptr };
	CTransform*			  m_pTransform         = { nullptr };
	CTransform*           m_pRealTransform     = { nullptr };
	CGameObject*          m_pCurrentGameObject = { nullptr };
	CLayer*				  m_pCurrentLayer      = { nullptr };


	bool    m_bObjectWindowOnOff = { false };
	bool    m_bInstallObject = { false };
	bool    m_bPickingObject = { false };
	bool    m_bObjectTransformViewOnOff = { false };
	bool    m_bPickingObjectFromMesh = { false };
	bool    m_bNavigationPickingFromMesh = { false };
	

	bool    isMouseOnObjectInformationWindow = false; // 클릭 상태 추적
	bool    m_bPickingSuccess = false;


	bool    m_bRealPickingSuccess = false;
	
	_uint   m_iCurrentView = {};
	_int    m_iCurrentClickObject = {-1};

	_tchar	m_szTextureFilePath[MAX_PATH]  = TEXT("");
	_tchar  m_szCurrentLayerName[MAX_PATH] = TEXT("");

	map<const _wstring, class CLayer*>* m_pLayer = { nullptr };

	_float3   m_fMeshPickingPoint = {};
	_float3   m_fMeshPickingScale = {1.f,1.f,1.f};	
	_float4x4 m_PickingMeshRotationMatrix = {};

	map<_float ,_float3>  finalPicking;
	map<_float, _float3>  finalNoramlVector; 

	/* 테스트 */
	int countA = 0;	
	int countB = 0;	
	int countC = 0;

	/* 물체가 겹쳐있을 때의 피킹*/
	map<_float, Picked_Object_Info> m_mapPickedMesh;	


	/* 각도 관련*/
	_float m_Angle_X = 0.f;		
	_float m_Angle_Y = 0.f;		
	_float m_Angle_Z = 0.f;		

	_float m_Pre_Angle_X = 0.f;	
	_float m_Pre_Angle_Y = 0.f;	
	_float m_Pre_Angle_Z = 0.f;

public:
	bool    Get_Picking_State() { return m_bRealPickingSuccess; }	// 네비게이션에 넘겨주기 
	_float3 Get_MeshPicking_Point() { return m_fMeshPickingPoint; }	
	bool    Get_Picking_Object_bool() { return m_bInstallObject; }
	bool    Compute_Final_MeshPicking(map<_float, Picked_Object_Info> _mapPickedMesh, FXMVECTOR mousePos, FXMVECTOR mouseRay);
	_float3 Get_Mesh_Scale() { return m_fMeshPickingScale; }
	_matrix Get_Picking_Mesh_Rotation_Matrix() { return XMLoadFloat4x4(&m_PickingMeshRotationMatrix); }	
private:
	void  ObjectWindow_View_1();
	void  ObjectWindow_View_2();
	void  ObjectWindow_View_Transform();

	bool  Picking_Object();

	HRESULT  InstallObject(_int _CurrentClickObject);
	HRESULT  InstallFromObject(_int _CurrentClickObject);
	HRESULT  LoadTexture(const _tchar* strTextureFilePath, _uint iNumTextures);

private:
	vector<ID3D11ShaderResourceView*> m_Textures;

public:
	static CImgui_ObjectTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override; 

};

