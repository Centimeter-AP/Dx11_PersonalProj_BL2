#pragma once

#include "VIBuffer_Point_Instance.h"
#include "VIBuffer_Terrain.h"
#include "SoundController.h"
#include "Sound_Core.h"
#include "VIBuffer_Cell.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Rect.h"
#include "Navigation.h"
#include "Transform.h"
#include "Collider.h"
#include "Texture.h"
#include "Shader.h"
#include "Gravity.h"
#include "Model.h"

#include "Bounding_Sphere.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"

/* 1. 원형객체를 보관한다. */
/* 1_1. 원형객체의 타입( CGameObject, CComponent )에 크게 영향을 받지 않는다. */
/* 1_2. 보관할때 레벨별( + 모든레벨) 로 구분하여 저장할거야. */

NS_BEGIN(Engine)

class CPrototype_Manager final : public CBase
{
private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	void Clear(_uint iLevelIndex);
	HRESULT Delete_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag);
	HRESULT Replace_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);


private:
	_uint										m_iNumLevels = {};
	map<const _wstring, class CBase*>*			m_pPrototypes = { nullptr };
	typedef map<const _wstring, class CBase*>	PROTOTYPES;

private:
	class CBase* Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag);

public:
	static CPrototype_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

NS_END