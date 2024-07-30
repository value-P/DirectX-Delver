#include "stdafx.h"
#include "MWLever.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "MoveWall.h"

MWLever::MWLever(LPDIRECT3DDEVICE9 _pGraphicDevice)
    : GameObject(_pGraphicDevice)
{
}

MWLever::~MWLever()
{
}

MWLever* MWLever::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vPullDir, const D3DXVECTOR3& _vUp, MoveWall* _pMoveWall, const D3DXVECTOR3& _vSize)
{
    MWLever* pInstance = new MWLever(_pGraphicDevice);

    if (FAILED(pInstance->Init_MWLever(_vPos, _vPullDir, _vUp, _pMoveWall, _vSize)))
    {
        Safe_Release(pInstance);
        MessageBox(nullptr, L"Init_MWLever Failed", L"MWLever::Create", MB_OK);
        return nullptr;
    }

    return pInstance;
}

HRESULT MWLever::Init_MWLever(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vPullDir, const D3DXVECTOR3& _vUp, MoveWall* _pMoveWall, const D3DXVECTOR3& _vSize)
{
	if (FAILED(Add_Component()))
    {
        return E_FAIL;
    }

	if (nullptr != _pMoveWall)
    {
        m_pMoveWall = _pMoveWall;
    }

    // 레버 돌 부분
    D3DXVECTOR3 vInfo[INFO_END];
    D3DXVec3Cross(&vInfo[INFO_RIGHT], &_vUp, &_vPullDir);
    D3DXVec3Cross(&vInfo[INFO_UP], &_vPullDir, &vInfo[INFO_RIGHT]);
	memcpy_s(&vInfo[INFO_LOOK], sizeof(D3DXVECTOR3), &_vPullDir, sizeof(D3DXVECTOR3));
    memcpy_s(&vInfo[INFO_POS], sizeof(D3DXVECTOR3), &_vPos, sizeof(D3DXVECTOR3));

    m_pTransformCom->Set_Scale(_vSize);
    m_pTransformCom->Set_Info(INFO_RIGHT, vInfo[INFO_RIGHT]);
    m_pTransformCom->Set_Info(INFO_UP, vInfo[INFO_UP]);
    m_pTransformCom->Set_Info(INFO_LOOK, vInfo[INFO_LOOK]);
    m_pTransformCom->Set_Info(INFO_POS, vInfo[INFO_POS]);

    D3DXMATRIX matWorld;
    D3DXMatrixIdentity(&matWorld);

    memcpy_s(&matWorld.m[0][0], sizeof(D3DXVECTOR3), &D3DXVECTOR3{ _vSize.x * vInfo[INFO_RIGHT] }, sizeof(D3DXVECTOR3));
    memcpy_s(&matWorld.m[1][0], sizeof(D3DXVECTOR3), &D3DXVECTOR3{ _vSize.y * vInfo[INFO_UP] }, sizeof(D3DXVECTOR3));
    memcpy_s(&matWorld.m[2][0], sizeof(D3DXVECTOR3), &D3DXVECTOR3{ _vSize.z * vInfo[INFO_LOOK] }, sizeof(D3DXVECTOR3));
    memcpy_s(&matWorld.m[3][0], sizeof(D3DXVECTOR3), &vInfo[INFO_POS], sizeof(D3DXVECTOR3));
    m_pTransformCom->Set_WorldMatrix(&matWorld);                                                    // m_pTransformCom matworld
    m_pCollider->SetSize(_vSize);                                                                   // Collider size
    m_pCollider->SelfBuilding();
    m_pCollider->SetColLayer(COL_INTERACTION);
    m_pCollider->SetThisPointer(this);

    // 레버 나무 부분
	D3DXVECTOR3 vLaver[INFO_END], vScale{ _vSize.x * 0.3f, _vSize.y * 2.0f, _vSize.z * 0.3f };
    D3DXMATRIX matRot;
    m_fRadian = -0.523598775f;    // 30도;
    m_fOmega = 0.0523f;
    memcpy_s(&vLaver[INFO_RIGHT], sizeof(D3DXVECTOR3), &vInfo[INFO_RIGHT], sizeof(D3DXVECTOR3));    // vLaver[INFO_RIGHT]
    memcpy_s(&m_vAxis, sizeof(D3DXVECTOR3), &vInfo[INFO_RIGHT], sizeof(D3DXVECTOR3));               // m_vAxis
	D3DXMatrixRotationAxis(&matRot, &m_vAxis, m_fRadian);
    D3DXVec3TransformNormal(&vLaver[INFO_UP], &_vUp, &matRot);                                      // vLaver[INFO_UP]
    D3DXVec3Cross(&vLaver[INFO_LOOK], &vLaver[INFO_RIGHT], &vLaver[INFO_UP]);                       // vLaver[INFO_LOOK]
	memcpy_s(&vLaver[INFO_POS], sizeof(D3DXVECTOR3), &D3DXVECTOR3{ vInfo[INFO_POS] + m_pLeverTrans->Get_Scale().y * vLaver[INFO_UP] }, sizeof(D3DXVECTOR3));

    m_pLeverTrans->Set_Scale(vScale);
    m_pLeverTrans->Set_Info(INFO_RIGHT, vLaver[INFO_RIGHT]);
    m_pLeverTrans->Set_Info(INFO_UP, vLaver[INFO_UP]);
    m_pLeverTrans->Set_Info(INFO_LOOK, &vLaver[INFO_LOOK]);
    m_pLeverTrans->Set_Info(INFO_POS, &vLaver[INFO_POS]);

    D3DXMatrixIdentity(&matWorld);
    memcpy_s(&matWorld.m[0][0], sizeof(D3DXVECTOR3), &D3DXVECTOR3{ vScale.x * vLaver[INFO_RIGHT] }, sizeof(D3DXVECTOR3));
    memcpy_s(&matWorld.m[1][0], sizeof(D3DXVECTOR3), &D3DXVECTOR3{ vScale.y * vLaver[INFO_UP] }, sizeof(D3DXVECTOR3));
    memcpy_s(&matWorld.m[2][0], sizeof(D3DXVECTOR3), &D3DXVECTOR3{ vScale.z * vLaver[INFO_LOOK] }, sizeof(D3DXVECTOR3));
    memcpy_s(&matWorld.m[3][0], sizeof(D3DXVECTOR3), &vLaver[INFO_POS], sizeof(D3DXVECTOR3));
    m_pLeverTrans->Set_WorldMatrix(&matWorld);

    

    return S_OK;
}

int MWLever::Update_GameObject(const float& _fTimeDelta)
{
    if (m_bActivating)
    {
        if (0.523598775f > m_fRadian)
        {
			m_fRadian += m_fOmega;
            WorkingLever();
        }
    }
    else
    {
        if (-0.523598775f < m_fRadian)
        {
            m_fRadian -= m_fOmega;
            WorkingLever();
            if (-0.523598775f >= m_fRadian)
            {
                m_bUsable = true;
            }
        }
    }

    m_pCollider->ClearInfo();
    Engine::AddCollider(m_pCollider);

    return 0;
}

void MWLever::LateUpdate_GameObject()
{
    if (m_bUsable)
    {
        if (m_pCollider->GetColType() & COL_PLAYER)
        {
            if (Engine::Get_DIKeyState(DIK_E) & 0x80)
            {
                if (nullptr != m_pMoveWall)
                {
                    if (!m_pMoveWall->GetMove())
                    {
                        m_pMoveWall->SetMove();
                        m_bActivating = true;
                        m_bUsable = false;
                    }
                }
                else
                {
                    m_bActivating = true;
                    m_bUsable = false;
                }
            }
        }
    }
    else
    {
        if (m_bActivating)
        {
            if (nullptr != m_pMoveWall)
            {
                if (!m_pMoveWall->GetMove())
                {
                    m_bActivating = false;
                }
            }
            else
            {
                if (0.523598775f <= m_fRadian)
                {
                    m_bActivating = false;
                }
            }
        }
    }
    Engine::Add_RenderGroup(RENDER_NONALPHA, this);
}

void MWLever::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pLeverTrans->Get_WorldMatrix());
    m_pLeverTex->Set_Texture();
    m_pBufferCom->Render_Buffer();
}

HRESULT MWLever::Add_Component()
{
    m_pTransformCom = dynamic_cast<Transform*>(Engine::Clone_Proto(L"Proto_Transform"));
    if (nullptr == m_pTransformCom) { return E_FAIL; }
    m_mapComponent[ID_STATIC].insert({ L"Proto_Transform", m_pTransformCom });

    m_pLeverTrans = dynamic_cast<Transform*>(Engine::Clone_Proto(L"Proto_Transform"));
    if (nullptr == m_pLeverTrans) { return E_FAIL; }
    m_mapComponent[ID_STATIC].insert({ L"Proto_Transform2", m_pLeverTrans });

    m_pBufferCom = dynamic_cast<CubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
    if (nullptr == m_pBufferCom) { return E_FAIL; }
    m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTex",m_pBufferCom });

    m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_LeverStone"));
    if (nullptr == m_pTextureCom) { return E_FAIL; }
    m_mapComponent[ID_STATIC].insert({ L"Proto_LeverStone",m_pTextureCom });

    m_pLeverTex = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_LeverWood"));
    if (nullptr == m_pLeverTex) { return E_FAIL; }
    m_mapComponent[ID_STATIC].insert({ L"Proto_LeverWood",m_pLeverTex });

    m_pCollider = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
    if (nullptr == m_pCollider) { return E_FAIL; }
    m_mapComponent[ID_STATIC].insert({ L"Proto_Collider",m_pCollider });
    m_pCollider->SetTransform(m_pTransformCom);


    return S_OK;
}

void MWLever::WorkingLever()
{
	D3DXVECTOR3 vLaver[INFO_END], vScale{ m_pLeverTrans->Get_Scale() };
	D3DXMATRIX matRot, matWorld;
    D3DXMatrixRotationAxis(&matRot, &m_vAxis, m_fRadian);
	D3DXVec3TransformNormal(&vLaver[INFO_UP], &m_pTransformCom->Get_Info(INFO_UP), &matRot);
    D3DXVec3Cross(&vLaver[INFO_LOOK], &m_vAxis, &vLaver[INFO_UP]);
    memcpy_s(&vLaver[INFO_POS], sizeof(D3DXVECTOR3), &D3DXVECTOR3{m_pTransformCom->Get_Info(INFO_POS) + m_pLeverTrans->Get_Scale().y * vLaver[INFO_UP]}, sizeof(D3DXVECTOR3));

    m_pLeverTrans->Set_Info(INFO_UP, vLaver[INFO_UP]);
    m_pLeverTrans->Set_Info(INFO_LOOK, &vLaver[INFO_LOOK]);
    m_pLeverTrans->Set_Info(INFO_POS, &vLaver[INFO_POS]);

    D3DXMatrixIdentity(&matWorld);
	memcpy_s(&matWorld.m[0][0], sizeof(D3DXVECTOR3), &D3DXVECTOR3{ vScale.x * m_vAxis }, sizeof(D3DXVECTOR3));
    memcpy_s(&matWorld.m[1][0], sizeof(D3DXVECTOR3), &D3DXVECTOR3{ vScale.y * vLaver[INFO_UP] }, sizeof(D3DXVECTOR3));
    memcpy_s(&matWorld.m[2][0], sizeof(D3DXVECTOR3), &D3DXVECTOR3{ vScale.z * vLaver[INFO_LOOK] }, sizeof(D3DXVECTOR3));
    memcpy_s(&matWorld.m[3][0], sizeof(D3DXVECTOR3), &vLaver[INFO_POS], sizeof(D3DXVECTOR3));
    m_pLeverTrans->Set_WorldMatrix(&matWorld);
}

void MWLever::Free()
{
    GameObject::Free();
}
