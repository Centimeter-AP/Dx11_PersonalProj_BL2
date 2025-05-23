#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{

}

CAnimation::CAnimation(const CAnimation& Prototype)
	: m_fDuration { Prototype.m_fDuration }
	, m_fTickPerSecond { Prototype.m_fTickPerSecond }
	, m_fCurrentTrackPosition { Prototype.m_fCurrentTrackPosition }
	, m_CurrentKeyFrameIndices { Prototype.m_CurrentKeyFrameIndices }
	, m_iNumChannels { Prototype.m_iNumChannels }
	, m_Channels { Prototype.m_Channels }
{
	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);
}


HRESULT CAnimation::Initialize(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones)
{
	/* �� �ִϸ��̼��� ��Ʈ���ؾ��ϴ� ���� ���� */
	m_iNumChannels = pAIAnimation->mNumChannels;

	m_fTickPerSecond = (_float)pAIAnimation->mTicksPerSecond;
	m_fDuration = (_float)pAIAnimation->mDuration;

	m_CurrentKeyFrameIndices.resize(m_iNumChannels);

	/* �� ���� ������ �������Ѵ�. */
	for (size_t i = 0; i < m_iNumChannels; i++)
	{		
		CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i], Bones);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

    return S_OK;
}

HRESULT CAnimation::Initialize(ifstream& ifs, const vector<class CBone*>& Bones)
{
	ifs.read(reinterpret_cast<_char*>(&m_fTickPerSecond), sizeof(_float));  // �ִϸ��̼� �ȿ� ä�� ��� 
	ifs.read(reinterpret_cast<_char*>(&m_fDuration), sizeof(_float));  // �ִϸ��̼� �ȿ� ä�� ��� 
	/* �� �ִϸ��̼��� ��Ʈ���ؾ��ϴ� ���� ���� */
	ifs.read(reinterpret_cast<_char*>(&m_iNumChannels), sizeof(_uint));  // �ִϸ��̼� �ȿ� ä�� ��� 


	m_CurrentKeyFrameIndices.resize(m_iNumChannels);

	/* �� ���� ������ �����Ѵ�. */
	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		CChannel* pChannel = CChannel::Create(ifs, Bones);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

_bool CAnimation::Update_Bones(_float fTimeDelta, const vector<CBone*>& Bones, _bool isLoop)
{
	_bool			isFinished = { false };

	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

	if (m_fCurrentTrackPosition >= m_fDuration)
	{
		if (false == isLoop)
		{
			m_fCurrentTrackPosition = m_fDuration;
			isFinished = true;
			goto exit;
		}		
		
		m_fCurrentTrackPosition = 0.f;			
	}

	

	/*for (auto& pChannel : m_Channels)*/
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i]->Update_TransformationMatrix(&m_CurrentKeyFrameIndices[i], m_fCurrentTrackPosition, Bones);
	}

exit:
	return isFinished;
}

CAnimation* CAnimation::Create(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, Bones)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Create(ifstream& ifs, const vector<class CBone*>& Bones)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(ifs, Bones)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	__super::Free();

		for (auto& pChannel : m_Channels)
			Safe_Release(pChannel);

	m_Channels.clear();


}
