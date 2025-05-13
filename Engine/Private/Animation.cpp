#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initiailize(const aiAnimation* pAIAnimation, const CModel* pModel, vector<_uint>& CurrentKeyFrameIndices)
{
	/* �� �ִϸ��̼��� ��ü ���� */
	m_fDuration = static_cast<_float>(pAIAnimation->mDuration);

	/* �� �ִϸ��̼��� �ʴ� ��� �Ÿ�*/
	m_fTickPerSecond = static_cast<_float>(pAIAnimation->mTicksPerSecond);

	/* �� �ִϸ��̼��� �����ϴµ� �־� �ʿ��� ���� ���� */
	m_iNumChannels = pAIAnimation->mNumChannels; 

	CurrentKeyFrameIndices.resize(m_iNumChannels);	

	m_CurrentKeyFameIndices = &CurrentKeyFrameIndices;	

	for (size_t i =0; i< m_iNumChannels; i++)
	{
		CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i], pModel);
		if (pChannel == nullptr)
			return E_FAIL;

		m_Channels.push_back(pChannel); 
	}

	return S_OK;
}

_bool CAnimation::Update_TransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _float* pCurrentTrackPoisiton, vector<_uint>& CurrentKeyFrameIndices, _bool isLoop)
{
	*pCurrentTrackPoisiton += m_fTickPerSecond * fTimeDelta;

	if (true == isLoop && *pCurrentTrackPoisiton >= m_fDuration)
	{
		*pCurrentTrackPoisiton = 0.f;
	}


	/* ���� �ִϸ��̼����� �̵������� ���� �ɷ��� true�� ��ȯ�ɰ���.*/
	if (*pCurrentTrackPoisiton >= m_fDuration)
		return true;



	_uint			iNumChannels = {};

	for (auto& pChannel : m_Channels)
	{
		pChannel->Update_TransformationMatrix(*pCurrentTrackPoisiton, &CurrentKeyFrameIndices[iNumChannels++], Bones);
	}


	return false;
}

void CAnimation::Reset(const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices, _float* pCurrentTrackPoisiton)
{
	*pCurrentTrackPoisiton = 0.0f;
	m_isFinished = false;

	_uint  iChannelIndex = { 0 };
	for (auto& pChannel : m_Channels)
	{
		pChannel->Reset_TransformationMatrix(Bones, &CurrentKeyFrameIndices[iChannelIndex++]);
	}
}

_uint CAnimation::Get_ChannelIndex(const _char* pChannelName)	
{
	_uint	iChannelIndex = {};

	auto iter = find_if(m_Channels.begin(), m_Channels.end(), [&](CChannel* pChannel)->_bool {
		if (false == strcmp(pChannel->Get_Name(), pChannelName))
			return true;

		++iChannelIndex;
		return false;
		});

	return iChannelIndex;
}

_bool CAnimation::Lerp_NextAnimation(_float fTimeDelta, CAnimation* pNextAnimation, const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices)
{
	m_LerpTimeAcc += fTimeDelta;

	if (pNextAnimation && m_LerpTimeAcc <= m_LerpTime)
	{
		_uint iChannelIndex = 0;
		for (auto& pChannel : m_Channels)
		{
			pChannel->Lerp_TransformationMatrix(Bones, pNextAnimation->m_Channels[iChannelIndex], m_LerpTime, m_LerpTimeAcc, &CurrentKeyFrameIndices[iChannelIndex]);
			iChannelIndex++;
		}
		return false;
	}

	else
	{
		m_LerpTimeAcc = 0.0f;
		return true;
	}

}

HRESULT CAnimation::Save_Anim(ostream& os)
{
	os.write((char*)&m_fDuration, sizeof(_float));
	os.write((char*)&m_fTickPerSecond, sizeof(_float));	
	os.write((char*)&m_fCurrentTrackPosition, sizeof(_float));
	os.write((char*)&m_isFinished, sizeof(_bool));
	os.write((char*)&m_iNumChannels, sizeof(_uint));	

	for (auto& channel : m_Channels)
		channel->Save_Channel(os);

	return S_OK;	
}

HRESULT CAnimation::Load_Anim(istream& is, vector<_uint>& CurrentKeyFrameIndices)
{
	is.read((char*)&m_fDuration, sizeof(_float));
	is.read((char*)&m_fTickPerSecond, sizeof(_float));
	is.read((char*)&m_fCurrentTrackPosition, sizeof(_float));
	is.read((char*)&m_isFinished, sizeof(_bool));
	is.read((char*)&m_iNumChannels, sizeof(_uint));
	/*readVectorFromBinary(is, m_CurrentKeyFrameIndices);*/
	CurrentKeyFrameIndices.resize(m_iNumChannels);			
	

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::LoadCreate(is);
		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

CAnimation* CAnimation::Create(const aiAnimation* pAIAnimation, const CModel* pModel, vector<_uint>& CurrentKeyFrameIndices)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initiailize(pAIAnimation, pModel,CurrentKeyFrameIndices)))	
	{
		MSG_BOX("Failed to Created : Animation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::LoadCreate(istream& is, vector<_uint>* _CurrentKeyFrameIndices)
{
	CAnimation* pInstance = new CAnimation();	
	if (FAILED(pInstance->Load_Anim(is, *_CurrentKeyFrameIndices)))	
	{
		MSG_BOX("Failed To LoadCreated : CAnimation");	
		Safe_Release(pInstance);
	}

	return pInstance;	
}

void CAnimation::Free()
{
	__super::Free();
	
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

}
