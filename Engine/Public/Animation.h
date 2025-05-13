#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default; 


public:
	HRESULT Initiailize(const aiAnimation* pAIAnimation, const class CModel* pModel, vector<_uint>& CurrentKeyFrameIndices);
	_bool Update_TransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _float* pCurrentTrackPoisiton, vector<_uint>& CurrentKeyFrameIndices, _bool isLoop);	

	/* 11�� 27�� �߰��� �ڵ� */
	void  Reset(const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices, _float* pCurrentTrackPoisiton);
	_uint Get_ChannelIndex(const _char* pChannelName);	
	_bool Lerp_NextAnimation(_float fTimeDelta, class CAnimation* pNextAnimation, const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices);


private:
	_float					   m_fDuration = {};
	_float					   m_fTickPerSecond = {};
	_float					   m_fCurrentTrackPosition = {};
	_float					   m_iNumChannels = {};

	vector<class CChannel*>    m_Channels; 

	/*11�� 27�� �߰� �ڵ�*/
/* �׷��� ���⼭ �ִϸ��̼��� �ʱ�ȭ ��������ϴµ�.*/

	bool                       m_bReset = { false };	

	_float					   m_LerpTime = 1.2f;	
	_float					   m_LerpTimeAcc = 0.f;	

	_bool					   m_isFinished = { false };	

	vector<_uint>*             m_CurrentKeyFameIndices;		

public:
	HRESULT Save_Anim(ostream& os);
	HRESULT Load_Anim(istream& is, vector<_uint>& CurrentKeyFrameIndices);	

	static CAnimation* Create(const aiAnimation* pAIAnimation, const class CModel* pModel, vector<_uint>& CurrentKeyFrameIndices);
	static CAnimation* LoadCreate(istream& is, vector<_uint>* _CurrentKeyFrameIndices);
	virtual void Free() override; 
	
};

END