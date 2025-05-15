#pragma once

#include "Base.h"

/* ��. (aiNode, aiBone, aiNodeAnim) */
/* aiNode�� �̿��ؼ� ������ �� ����. */

NS_BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	HRESULT Initialize(const aiNode* pAINode, _int iParentBoneIndex);
	void Update_TransformationMatrix(_fmatrix TransformationMatrix);
	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones);

private:
	_char					m_szName[MAX_PATH] = {};

	/* (A : �� �� ��ü�� �������� ��ȯ������ ǥ���� ���) */
	_float4x4				m_TransformationMatrix = {};

	/* (A : �� �� ��ü�� �������� ��ȯ������ ǥ���� ���) * �θ����. = A: �θ� �������� ȸ���Ѵ�. */
	_float4x4				m_CombinedTransformationMatrix = {};	

	_int					m_iParentBoneIndex = { -1 };

public:
	static CBone* Create(const aiNode* pAINode, _int iParentBoneIndex);
	virtual void Free() override;

};

NS_END