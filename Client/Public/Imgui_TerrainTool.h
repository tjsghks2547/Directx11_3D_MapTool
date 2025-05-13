#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)


END

class CImgui_TerrainTool : public CBase
{
private:
	CImgui_TerrainTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImgui_TerrainTool() = default;


public:
	HRESULT Init();
	HRESULT Update();

public:

	bool  Get_PickingTileBool() { return m_bPickingTile; }


private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	


	bool   m_bTerrainWindowOnOff = { false };
	bool   m_bPickingTile = { false };



public:
	static CImgui_TerrainTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	virtual void Free() override;

};

