#pragma once

#include "Component.h"

BEGIN(Engine)
class CAnimation;	

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum MODEL { MODEL_NONANIM, MODEL_ANIM, MODEL_END};

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	CModel(const CModel& Prototype);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	_uint Get_BoneIndex(const _char* pBoneName) const;
	const _float4x4* Get_BoneMatrix(const _char* pBoneName) const;	

public:
	virtual HRESULT Initialize_Prototype(MODEL eModelType,const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);


public:
	void SetUp_Animation(_uint iAnimIndex, _bool isLoop);	
	_bool Play_Animation(_float fTimeDelta);
	HRESULT Bind_Material(class CShader* pShader, _uint iMeshIndex, aiTextureType eType, const _char* pConstantName, _uint iTextureIndex = 0);
	HRESULT Bind_BoneMatrices(class CShader* pShader, _uint iMeshIndex, const _char* pConstantName);
	vector<class CMesh*>& Get_Meshes() { return m_Meshes; }	

private:
	/* ������ ������ �����Ѵ�. */
	const aiScene* m_pAIScene = { nullptr };

	/* ������ �� �а� �������ش�. */
	Assimp::Importer				m_Importer = {};	

private:
	_uint							m_iNumMeshes = {};
	vector<class CMesh*>			m_Meshes;

	MODEL							m_eModelType = MODEL_END;	

private:
	_uint                           m_iNumMaterials = {};	
	vector<class CMaterial*>	    m_Materials;


private:
	vector<class CBone*>			m_Bones; 

private:
	_uint							m_iNumAnimations = {};
	_uint							m_iCurrentAnimIndex = {};
	/* 11�� 27�� �߰�*/
	_uint							m_iPreviousAnimIndex = {};
	//
	vector<class CAnimation*>       m_Animations;
	_bool							m_isAnimLoop = {};

	/* �ܲ� �߰� */
	_float							m_fCurrentTrackPosition = {};
	vector<vector<_uint>>			m_CurrentKeyFrameIndices; /* �ִϸ��̼��� ���� �ϴ� �ε��� */
	_float4x4						m_PreTransformMatrix;


private:	
	/* �������� �߰��۾�*/
	CAnimation* m_pNextAnimation = nullptr;	
	CAnimation* m_pCurrentAnimation = nullptr;	
	_uint       m_iNextAnimationIndex = {};	
	_bool       m_bFinished = { false };	
	_uint       m_iCurrentAnimationIndex = {};	
	_bool       m_bLerpFinished = { false };	


/* ���̳ʸ�ȭ ����*/
private:
	string						    m_fullpath;		
	//_float4x4					    m_PreTransformMatrix;	
	_bool						    isFile = false;
private:
	HRESULT		Save_Model(const _char* pModelFilePath);	
	HRESULT     Load_Model(_fmatrix PreTransformMatrix);	


private:
	HRESULT Ready_Meshes(_fmatrix PreTransformMatrix);
	HRESULT Ready_Materials(const _char* pModelFilePath);
	HRESULT Ready_Bones(aiNode* pAINode, _int iParentBoneIndex = -1);
	HRESULT Ready_Animations();

public:
	
	MODEL Get_ModelType() { return m_eModelType; }		

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, MODEL eModelType,_fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END