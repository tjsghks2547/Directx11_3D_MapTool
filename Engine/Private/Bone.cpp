#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
{
	/* 여기서 Create 하고 난 뒤의 호출되는 함수니깐 bone의 transformation과 combinedTrasnformation 정보를 채워줘야할듯*/
	strcpy_s(m_szName, pAINode->mName.data);	

	/*pAINode->mTransformation**은 해당 노드의 로컬 변환 행렬*/
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));	

	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));	

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	m_iParentBoneIndex = iParentBoneIndex;
	
	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	if (-1 == m_iParentBoneIndex)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, PreTransformMatrix * XMLoadFloat4x4(&m_TransformationMatrix));

	else
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
	}
}

HRESULT CBone::Save_Bone(ostream& os)
{

	os.write(m_szName, sizeof(_char) * MAX_PATH);
	os.write((char*)&m_iParentBoneIndex, sizeof(_int));
	os.write((char*)&m_TransformationMatrix, sizeof(_float4x4));

	return S_OK;	
}

HRESULT CBone::Load_Bone(istream& os)
{
	
	os.read(m_szName, sizeof(_char) * MAX_PATH);
	os.read((char*)&m_iParentBoneIndex, sizeof(_int));
	os.read((char*)&m_TransformationMatrix, sizeof(_float4x4));
	return S_OK;	
}

CBone* CBone::Create(const aiNode* pAINode, _int iParentBoneIndex)
{
	CBone* pInstance = new CBone(); 

	if(FAILED(pInstance->Initialize(pAINode,iParentBoneIndex)))
	{
		MSG_BOX("Failed to Created : Bone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::LoadCreate(std::istream& os)
{
	CBone* pInstance = new CBone();

	if(FAILED(pInstance->Load_Bone(os)))
	{
		MSG_BOX("Failed to Created : Bone");
		Safe_Release(pInstance);
	}

	return pInstance;	
}

CBone* CBone::Clone()
{
	return new CBone(*this);
}

void CBone::Free()
{
	__super::Free();
}
