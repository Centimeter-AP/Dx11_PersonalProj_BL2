#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CLight_Manager final : public CBase
{
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC* Get_Light(_uint iIndex);
	void Clear();
	_uint Get_LastLight_Index();
	void Delete_Light(_uint iIndex);
	void Delete_LastLight();

public:
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

private:
	list<class CLight*>				m_Lights;
	

public:
	static CLight_Manager* Create();
	virtual void Free() override;
};

NS_END