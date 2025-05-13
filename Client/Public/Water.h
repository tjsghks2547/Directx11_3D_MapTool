#pragma once	
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Terrain;
END


BEGIN(Client)
class CWater final : public CGameObject
{
private:
	CWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	CWater(const CWater& Prototype);
	virtual ~CWater() = default;	

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	virtual HRESULT Render() override;

	/* 10�� 31�� �߰�*/
	virtual HRESULT Ready_Component();

private:
	HRESULT Bind_ShaderResources();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };

	/* 11�� 16�� �ӽ�*/
private:
	VTXNORTEX* pVertices = { nullptr };
	_uint* pIndices = { nullptr };
	bool       m_btest = { true };
	size_t     m_iTotalTriangle = {};
	CVIBuffer_Terrain* pTerrain = { nullptr };
	bool	   m_isButtonPressed = { false };

	/* 11�� 15�� �ӽ� */
private:
	ID3D11Texture2D* m_pTexture2D = { nullptr };
	_uint							m_iWinSizeX, m_iWinSizeY;
	
	_float							m_fTimeDelta = {};
	_float							m_famplitude = {};
	_float							m_fwavelength = {};	
	_float							m_fSpeed = {};

	_float							m_faccumulatedTime = {};

public:
	static CWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
