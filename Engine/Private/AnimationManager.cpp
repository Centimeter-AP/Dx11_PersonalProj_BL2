#include "AnimationManager.h"

HRESULT CAnimationManager::Initialize(vector<CBone*>* Bones, vector<CAnimation*>* Anims)
{
    if (nullptr == Bones || nullptr == Anims)
        return E_FAIL;
    m_pBones = Bones;
    m_pAnims = Anims;
    m_PreviousPoseMatrices.resize((*m_pBones).size());
    return S_OK;
}

void CAnimationManager::SetAnimation(_uint iAnimIndex, _bool isLoop, _float fBlendDuration)
{
    if (iAnimIndex >= m_pAnims->size())
        return;


    if (fBlendDuration > 0.f && m_pCurrentAnim)
    {
        m_pNextAnim = (*m_pAnims)[iAnimIndex];
        m_fBlendTime = 0.f;
        m_fBlendDuration = fBlendDuration;
        m_isLoop = isLoop;

        // ���� �� ���¸� ĸó
        for (_uint i = 0; i < (*m_pBones).size(); ++i)
            m_PreviousPoseMatrices[i] = XMLoadFloat4x4((*m_pBones)[i]->Get_TransformationMatrix());
    }
    else
    {
        m_pCurrentAnim = (*m_pAnims)[iAnimIndex];
        m_pCurrentAnim->Reset();
        m_isLoop = isLoop;
    }
}
//ȫ���� �Դٰ�
//�ʴ��߿���
_bool CAnimationManager::Update(_float fTimeDelta, const _fmatrix& PreTransformMatrix)
{
    auto& Bones = *m_pBones;
    _bool isFinished = { false };
    if (m_pNextAnim)
    {
        // ���� ����
        m_fBlendTime += fTimeDelta;
        _float ratio = min(m_fBlendTime / m_fBlendDuration, 1.f);

        // Next �ִϸ��̼� ������Ʈ
        m_pNextAnim->Update_Bones(fTimeDelta, Bones, m_isLoop);

        // ���� ��� ��� �� ���� �ٷ� �ݿ�
        for (_uint i = 0; i < Bones.size(); ++i)
        {
            _matrix matCur = m_PreviousPoseMatrices[i];
            _matrix matNext = m_pNextAnim->Get_TransformMatrix(i);
            _matrix blended = BlendMatrices(matCur, matNext, ratio);
            Bones[i]->Set_TransformationMatrix(blended);
        }

        if (ratio >= 1.f)
        {
            m_pCurrentAnim = m_pNextAnim;
            //m_pCurrentAnim->Reset();
            m_pNextAnim = nullptr;
            m_fBlendTime = m_fBlendDuration = 0.f;
        }
    }
    else if (m_pCurrentAnim)
    {
        // �Ϲ� �ִϸ��̼� ���
        isFinished = m_pCurrentAnim->Update_Bones(fTimeDelta, Bones, m_isLoop);
    }
    for (CBone* pBone : Bones)
    {
        pBone->Update_CombinedTransformationMatrix(Bones, PreTransformMatrix);
    }
    return isFinished;
}

const _float CAnimationManager::Get_CurrentTrackPosition()
{
	if (m_pNextAnim)
	{
		return m_pNextAnim->Get_CurrentTrackPosition();
	}
    else
        return m_pCurrentAnim->Get_CurrentTrackPosition();
}

//void CAnimationManager::ApplyToBones(const _fmatrix& PreTransformMatrix)
//{
//    auto& Bones = *m_pBones;
//    for (CBone* pBone : Bones)
//    {
//        pBone->Update_CombinedTransformationMatrix(Bones, PreTransformMatrix);
//    }
//}

XMMATRIX CAnimationManager::BlendMatrices(FXMMATRIX M1, CXMMATRIX M2, float t)
{
    // 1) ����
    XMVECTOR scale1, rotQ1, trans1;
    XMVECTOR scale2, rotQ2, trans2;
    XMMatrixDecompose(&scale1, &rotQ1, &trans1, M1);
    XMMatrixDecompose(&scale2, &rotQ2, &trans2, M2);

    // 2) �� ���� ����
    XMVECTOR scale = XMVectorLerp(scale1, scale2, t);
    XMVECTOR rotQ = XMQuaternionSlerp(rotQ1, rotQ2, t);
    XMVECTOR trans = XMVectorLerp(trans1, trans2, t);

    // 3) ���ռ�
    return XMMatrixAffineTransformation(
        scale,                  // ������ ����
        XMVectorZero(),         // �����ϡ�ȸ�� ������ (0,0,0)
        rotQ,                   // ȸ�� quaternion
        trans                   // �̵� ����
    );
}

CAnimationManager* CAnimationManager::Create(vector<CBone*>* Bones, vector<CAnimation*>* Anims)
{
    CAnimationManager* pInstance = new CAnimationManager();

    if (FAILED(pInstance->Initialize(Bones, Anims)))
    {
        MSG_BOX("Failed to Created : CAnimationManager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAnimationManager::Free()
{
    __super::Free();
}
