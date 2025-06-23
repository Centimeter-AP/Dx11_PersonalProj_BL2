#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGravity : public CComponent
{
public:
	typedef struct tagGravityDesc
	{
		class CNavigation* pOwnerNavigationCom = { nullptr };
		class CTransform* pOwnerTransformCom = { nullptr };
		_float fGravity = -50.f;
		_float fJumpPower = 10.f;
	}DESC;

/* 
	Ʃ�� ��
	���� ���̰� �ʹ� ������ JumpPower�� �ø�����

	������ �� �Ǵµ� ������ �ʹ� ������ Gravity�� ���߼���

	�����ϸ鼭 ���� �����ϸ� fTimeDelta ���� �Ǵ� Y ��ġ Ŭ���� ó���� ��������

	����ϳ׿�
*/

private:
	CGravity(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGravity(const CGravity& Prototype);
	virtual ~CGravity() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	void Update(_float fTimeDelta);


public:
	_bool Is_Grounded() const { return m_isGrounded; }
	//����ఢ
	const _bool* Is_Grounded_Ptr() { return &m_isGrounded; }

	void Set_IsGrounded(_bool isGrounded) { m_isGrounded = isGrounded; }
	void Jump();

private:
	class CNavigation*		m_pNavigationCom = { nullptr };
	class CTransform*		m_pTransformCom = { nullptr };

	_bool					m_isGrounded = { true };
	_float					m_fGravity = {};
	_float					m_fVerticalVelocity = {};
	_float					m_fJumpPower = {};


public:
	static CGravity* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

NS_END