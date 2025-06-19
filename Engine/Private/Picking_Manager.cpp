#include "Picking_Manager.h"

#include "Layer.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "GameObject.h"
#include "VIBuffer_Terrain.h"
#include "GameInstance.h"

CPicking_Manager::CPicking_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{ pDevice }
    , m_pContext{ pContext }
    , m_pGameInstance {CGameInstance::Get_Instance()}
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CPicking_Manager::Initialize(HWND hWnd)
{
    m_hWnd = hWnd;

    return S_OK;
}

void CPicking_Manager::Update()
{
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(m_hWnd, &ptMouse);

    D3D11_VIEWPORT ViewPort;
	_uint   iNumViewports = 1;
    m_pContext->RSGetViewports(&iNumViewports, &ViewPort);

    _float4 vPosition = {};

    vPosition.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
    vPosition.y = ptMouse.y / (ViewPort.Height * -0.5f) + 1.f;
    vPosition.z = 0.f;
    vPosition.w = 1.f;

    /*3. 뷰스페이스로 옮기자. 로컬위치 * 월드행렬 * 뷰행렬 */
    _vector vecMousePos = XMLoadFloat4(&vPosition);
    _matrix		ProjMatrixInv;
    ProjMatrixInv = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);
    ProjMatrixInv = XMMatrixInverse(nullptr, ProjMatrixInv);
    vecMousePos = XMVector3TransformCoord(vecMousePos, ProjMatrixInv);

    /* 뷰스페이스 상의 마우스 레이와 레이포스를 구하자. */
    _vector vecRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
    _vector vecRayDir = vecMousePos - vecRayPos;


    /*3. 월드 스페이스로 옮기자. 로컬위치 * 월드행렬*/
    _matrix		ViewMatrixInv;
    ViewMatrixInv = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
    ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrixInv);
    vecRayPos = XMVector3TransformCoord(vecRayPos, ViewMatrixInv);
    vecRayDir = XMVector3TransformNormal(vecRayDir, ViewMatrixInv);
    vecRayDir = XMVector3Normalize(vecRayDir);

    XMStoreFloat3(&m_tRay.vOrigin, vecRayPos);
    XMStoreFloat3(&m_tRay.vDirection, vecRayDir);

}


_bool CPicking_Manager::Pick_Mesh(_fmatrix WorldMatrix, const _float3* pVertices, const _uint* pIndices, _uint iNumIndices, _float3& vOutPickedPos, _Out_ _float& fHitDist)
{
    _float fMinDist = FLT_MAX;
    _bool bHit = false;

    for (_uint i = 0; i < iNumIndices; i += 3)
    {
        _float3 A = pVertices[pIndices[i]];
        _float3 B = pVertices[pIndices[i + 1]];
        _float3 C = pVertices[pIndices[i + 2]];

        // 월드 변환
        _vector vA = XMVector3TransformCoord(XMLoadFloat3(&A), WorldMatrix);
        _vector vB = XMVector3TransformCoord(XMLoadFloat3(&B), WorldMatrix);
        _vector vC = XMVector3TransformCoord(XMLoadFloat3(&C), WorldMatrix);

        _float dist = 0.f;
        if (TriangleTests::Intersects(
            XMLoadFloat3(&m_tRay.vOrigin),
            XMLoadFloat3(&m_tRay.vDirection),
            vA, vB, vC, dist))
        {
            if (dist < fMinDist)
            {
                fMinDist = dist;
                _vector hitPos = XMLoadFloat3(&m_tRay.vOrigin) + XMLoadFloat3(&m_tRay.vDirection) * dist;
                XMStoreFloat3(&vOutPickedPos, hitPos);
                bHit = true;
            }
        }
    }
    fHitDist = fMinDist;
    return bHit;
}

_bool CPicking_Manager::Picking(_float3& vPickedPos, const _float3& A, const _float3& B, const _float3& C)
{
    _vector orig = XMLoadFloat3(&m_tRay.vOrigin);
    _vector dir = XMLoadFloat3(&m_tRay.vDirection);
    _vector v0 = XMLoadFloat3(&A);
    _vector v1 = XMLoadFloat3(&B);
    _vector v2 = XMLoadFloat3(&C);

    float dist = 0.f;

    if (TriangleTests::Intersects(orig, dir, v0, v1, v2, dist))
    {
        _vector hitPos = orig + dir * dist;
        XMStoreFloat3(&vPickedPos, hitPos);
        return true;
    }

    return false;
}
CGameObject* CPicking_Manager::Pick_Object_In_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _float3& fPickedPos)
{
    CLayer* pLayer = m_pGameInstance->Find_Layer(iLevelIndex, strLayerTag);
    if (nullptr == pLayer)
        return nullptr;

    list<CGameObject*> ObjectList = pLayer->Get_LayerObjectLists();
    if (!ObjectList.empty())
    {
        for (auto pObject : ObjectList)
        {
            // 버텍스와 인덱스 정보 가져오기
            auto pModel = dynamic_cast<CModel*>(pObject->Get_Component(TEXT("Com_Model")));
            if (nullptr == pModel)
                continue;
            auto pMeshes = pModel->Get_Meshes();
            if (nullptr == pMeshes)
                continue;
            for (auto pMesh : *pMeshes)
            {
                const _float3* pVertices = pMesh->Get_VertexPositions();
                const _uint* pIndices = pMesh->Get_Indices();
                _uint iNumIndices = pMesh->Get_NumIndices();
                // 월드 행렬 가져오기
                _matrix WorldMatrix = pObject->Get_Transform()->Get_WorldMatrix();
                // 피킹된 위치 저장
                _float fDist = {};
                if (m_pGameInstance->Pick_Mesh(WorldMatrix, pVertices, pIndices, iNumIndices, fPickedPos, fDist))
                {
                    return pObject;
                }
            }

        }
    }
    fPickedPos = { 0.f, 0.f, 0.f };
    return nullptr;
}

CPicking_Manager* CPicking_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
    CPicking_Manager* pInstance = new CPicking_Manager(pDevice, pContext);

    if (FAILED(pInstance->Initialize(hWnd)))
    {
        MSG_BOX("Failed To Create : CPicking_Manager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPicking_Manager::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}

