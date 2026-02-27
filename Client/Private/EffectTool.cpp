#include "EffectTool.h"
#include "Layer.h"
#include "Shader.h"
#include "GameInstance.h"
#include "ToolParticle.h"
#include "EffectSequence.h"

CEffectTool::CEffectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImGuiTool{ pDevice, pContext }
{

}

CEffectTool::CEffectTool(const CEffectTool& Prototype)
	: CImGuiTool(Prototype)
{
}

HRESULT CEffectTool::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffectTool::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	return S_OK;
}

void CEffectTool::Priority_Update(_float fTimeDelta)
{

}

EVENT CEffectTool::Update(_float fTimeDelta)
{
	Key_Input();
	return EVN_NONE;
}

void CEffectTool::Late_Update(_float fTimeDelta)
{
}

HRESULT CEffectTool::Render()
{
	if (m_pWindowData->ShowTerrainMenu)
	{
		if (FAILED(Render_EffectTool()))
			return E_FAIL;

		
	}

	return S_OK;
}


HRESULT CEffectTool::Render_EffectTool()
{
	//if (FAILED(Make_Particles()))
	//	return E_FAIL;

	if (FAILED(Test_Sequence()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffectTool::Make_Particles()
{
	ImGui::Begin("Particle Maker");
	ImGui::DragInt("Num Instance", &m_iNumInstance, 1, 1, 1000, "%d");
	ImGui::DragFloat3("Pivot", reinterpret_cast<_float*>(&m_vPivot), 0.1f, -1000.f, 1000.f, "%.1f");
	ImGui::DragFloat2("LifeTime", reinterpret_cast<_float*>(&m_vLifeTime), 0.1f, 0.f, 100.f, "%.1f");
	ImGui::DragFloat2("Speed", reinterpret_cast<_float*>(&m_vSpeed), 0.1f, 1.f, 1000.f, "%.1f");
	ImGui::DragFloat3("Range", reinterpret_cast<_float*>(&m_vRange), 0.1f, 1.f, 1000.f, "%.1f");
	ImGui::DragFloat2("Size", reinterpret_cast<_float*>(&m_vSize), 0.1f, 1.f, 1000.f, "%.1f");
	ImGui::DragFloat3("Center", reinterpret_cast<_float*>(&m_vCenter), 0.1f, -1000.f, 1000.f, "%.1f");
	ImGui::Checkbox("Loop", &m_isLoop);


	if (ImGui::RadioButton("Explosion", m_eParticleType == PTYPE_SPREAD)) {
		m_eParticleType = PTYPE_SPREAD;
	}
	if (ImGui::RadioButton("Drop", m_eParticleType == PTYPE_DROP)) {
		m_eParticleType = PTYPE_DROP;
	}
	if (ImGui::RadioButton("Spread + Drop", m_eParticleType == PTYPE_SPDROP)) {
		m_eParticleType = PTYPE_SPDROP;
	}
	if (ImGui::RadioButton("Drop_Gravity", m_eParticleType == PTYPE_DROPGRAV)) {
		m_eParticleType = PTYPE_DROPGRAV;
	}
	if (m_pToolParticle != nullptr)
	{
		m_pToolParticle->Set_PType(m_eParticleType);
	}
	if (Button("Change Loop"))
	{
		m_isLoop = !m_isLoop;
		m_pToolParticle->Set_Loop(m_isLoop);
	}

	if (Button("Make Particle"))
	{
		if (m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Particle")) != nullptr)
		{
			m_pGameInstance->Clear_Layer(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Particle"));
			m_pToolParticle = nullptr;
		}
		/* For.Prototype_Component_VIBuffer_Explosion_Test */
		CVIBuffer_Point_Instance::POINT_INSTANCE_DESC		Desc{};
		Desc.iNumInstance = m_iNumInstance;
		Desc.vCenter = m_vCenter;
		Desc.vRange = m_vRange;
		Desc.vSize = m_vSize;
		Desc.vLifeTime = m_vLifeTime;
		Desc.vSpeed = m_vSpeed;
		Desc.isLoop = m_isLoop;
		Desc.vPivot = m_vPivot;

		if (FAILED(m_pGameInstance->Replace_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_Component_VIBuffer_ToolParticle"),
			CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &Desc))))
			return E_FAIL;

		/* For.Prototype_GameObject_Explosion_Test */
		if (FAILED(m_pGameInstance->Replace_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_ToolParticle"),
			CToolParticle::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		CGameObject::DESC Particledesc = {};
		Particledesc.iLevelID = ENUM_CLASS(LEVEL::MAPTOOL);
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_ToolParticle"),
			ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Particle"), &Particledesc)))
			return E_FAIL;

		m_pToolParticle = static_cast<CToolParticle*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Particle"), 0));
	}

	ImGui::End();
	return S_OK;
}

HRESULT CEffectTool::Test_Sequence()
{
	ImGui::Begin("Effect Sequence");

	static _int iCurFrame = {};
	static _bool bExpanded = { true };
	static _int iSelected = {-1};


	if (Button("ADD"))
	{

	}

	//ImSequencer::Sequencer(m_pSequence, &iCurFrame, &bExpanded, &iSelected, nullptr, ImSequencer::SEQUENCER_EDIT_STARTEND);


	ImGui::End();
	return S_OK;
}

void CEffectTool::Key_Input()
{

}

CEffectTool* CEffectTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CEffectTool* pInstance = new CEffectTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CEffectTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffectTool::Clone(void* pArg)
{
	CEffectTool* pInstance = new CEffectTool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffectTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CEffectTool::Free()
{
	__super::Free();

	Safe_Release(m_pShader);
}
