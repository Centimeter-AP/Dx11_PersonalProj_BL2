#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones);

	void Update_Bones(_float fTimeDelta, const vector<CBone*>& Bones, _bool isLoop);
private:
	/* ��ü ��� �Ÿ�. */
	_float					m_fDuration = {};
	_float					m_fTickPerSecond = {};
	_float					m_fCurrentTrackPosition = {};

	/* �� �ִϸ��̼��� ǥ���ϱ����ؼ� ����ϴ� ���� ���� */
	_uint					m_iNumChannels;	
	vector<class CChannel*>	m_Channels;

public:
	static CAnimation* Create(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones);
	virtual void Free() override;
};

NS_END