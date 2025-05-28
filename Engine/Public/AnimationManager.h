#pragma once

#include "Base.h"
#include "Bone.h"
#include "Animation.h"

NS_BEGIN(Engine)

class ENGINE_DLL CAnimationManager final : public CBase
{
protected:
	CAnimationManager() {}
	virtual ~CAnimationManager() = default;

public:
	HRESULT Initialize(vector<CBone*>* Bones, vector<CAnimation*>* Anims);

	void SetAnimation(_uint iAnimIndex, _bool isLoop, _float fBlendDuration = 0.2f);
	_bool Update(_float fTimeDelta, const _fmatrix& PreTransformMatrix);
	//void ApplyToBones(const _fmatrix& PreTransformMatrix);

	void    Reset_CurAnimationFrame() { m_pCurrentAnim->Reset(); }

protected:
	vector<CBone*>*			m_pBones = nullptr;
	vector<CAnimation*>*	m_pAnims = nullptr;

	CAnimation*				m_pCurrentAnim = nullptr;
	CAnimation*				m_pNextAnim = nullptr;

	vector<_matrix>			m_PreviousPoseMatrices;
	_float					m_fBlendTime = 0.f;
	_float					m_fBlendDuration = 0.f;
	_bool					m_isLoop = false;

public:
	static XMMATRIX BlendMatrices(FXMMATRIX M1, CXMMATRIX M2, float t);
	static CAnimationManager* Create(vector<CBone*>* Bones, vector<CAnimation*>* Anims);
	virtual void Free() override;
};

NS_END