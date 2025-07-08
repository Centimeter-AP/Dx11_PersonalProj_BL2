#include "Light_Manager.h"
#include "Light.h"

CLight_Manager::CLight_Manager()
{

}

const LIGHT_DESC* CLight_Manager::Get_Light(_uint iIndex)
{
	auto	iter = m_Lights.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;
	
	return (*iter)->Get_LightDesc();
}

void CLight_Manager::Clear()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);
	m_Lights.clear();
}

_uint CLight_Manager::Get_LastLight_Index()
{
	_uint	iIndex = {};
	for (auto iter = m_Lights.begin(); iter != m_Lights.end(); iter++)
		++iIndex;

	return iIndex;
}

void CLight_Manager::Delete_Light(_uint iIndex)
{
	auto	iter = m_Lights.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;
	if (iter != m_Lights.end())
	{
		Safe_Release(*iter);
		m_Lights.erase(iter);
	}
}

void CLight_Manager::Delete_LastLight()
{
	Safe_Release(m_Lights.back());
	m_Lights.pop_back();
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC& LightDesc)
{
	CLight* pLight = CLight::Create(LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	for (auto& pLight : m_Lights)
		pLight->Render(pShader, pVIBuffer);

	return S_OK;
}


CLight_Manager* CLight_Manager::Create()
{
	return new CLight_Manager;
}

void CLight_Manager::Free()
{
	__super::Free();

	for (auto& pLight : m_Lights)	
		Safe_Release(pLight);

	m_Lights.clear();
}
