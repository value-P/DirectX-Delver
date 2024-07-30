#include "Collider.h"
#include "Transform.h"

Engine::Collider::Collider(LPDIRECT3DDEVICE9 _pGraphicDevice)
    : Component(_pGraphicDevice)
{
}

Engine::Collider::Collider(const Collider& _rhs)
    : Component(dynamic_cast<const Component&>(_rhs))
{
}

Engine::Collider::~Collider()
{
}

Collider* Engine::Collider::Create(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
    Collider* pInstance = new Collider(_pGraphicDevice);

    if (FAILED(pInstance->Init_Collider()))
    {
        Safe_Release(pInstance);
        MessageBox(nullptr, L"Init_Collider Failed", L"Collider::Create", MB_OK);
        return nullptr;
    }

    return pInstance;
}

Component* Engine::Collider::Clone()
{
    return new Collider(*this);
}

HRESULT Engine::Collider::Init_Collider()
{
    m_tInfo.m_fRadius = sqrt(3.0f);
    return S_OK;
}

int Engine::Collider::Update_Component(const _float& fTimeDelta)
{
    if (nullptr != m_pTransform)
    {
        memset(&m_iColSphere, 0, 4);
        memset(&m_iColOBB, 0, 4);
        memset(&m_iColType, 0, 4);
        for (int i = 0; i < COL_END; i++)
        {
            m_lstColSphere_Info[i].clear();
            m_lstColOBB_Info[i].clear();
        }
        SettingCInfo();
    }
    return 0;
}

void Engine::Collider::LateUpdate_Component()
{
}

const list<pair<CINFO, D3DXVECTOR3>>& Engine::Collider::GetColSphere_Info(COLLAYER _eType)
{
    for (int i = 0; i < COL_END; i++)
    {
		if (_eType & 1 << i) 
        {
            return m_lstColSphere_Info[i];
        }
    }
    return list<pair<CINFO, D3DXVECTOR3>>{};
}

const list<pair<CINFO, D3DXVECTOR3>>& Engine::Collider::GetColOBB_Info(COLLAYER _eType)
{
    for (int i = 0; i < COL_END; i++)
    {
        if (_eType & 1 << i)
        {
            return m_lstColOBB_Info[i];
        }
    }
	return list<pair<CINFO, D3DXVECTOR3>>{};
}

void Engine::Collider::SetTransform(Transform* _pTransform)
{
    if (nullptr == m_pTransform || nullptr != _pTransform)
    {
        m_pTransform = _pTransform;
    }
}

void Engine::Collider::SetInfo(INFO _eType, const D3DXVECTOR3& _vInfo)
{
    m_tInfo.m_vInfo[_eType] = _vInfo;
}

void Engine::Collider::SetSize(const float& _fX, const float& _fY, const float& _fZ)
{
    m_tInfo.m_vSize.x = _fX;
    m_tInfo.m_vSize.y = _fY;
    m_tInfo.m_vSize.z = _fZ;
    SettingRadius();
}

void Engine::Collider::SetSize(const D3DXVECTOR3& _vSize)
{
    m_tInfo.m_vSize = _vSize;
    SettingRadius();
}

void Engine::Collider::SetColLayer(COLLAYER _eColLayer)
{
    m_tInfo.m_iColLayer = _eColLayer;
}

void Engine::Collider::SetThisPointer(void* _pThis)
{
    m_tInfo.m_pThis = _pThis;
}

void Engine::Collider::SetDamage(const int& _iDamgae)
{
    m_tInfo.m_iDamage = _iDamgae;
}

void Engine::Collider::AddColSphere(const size_t& _iColType, const CINFO& _ColInfo, const D3DXVECTOR3& _vNear)
{
	m_iColSphere |= _iColType;
    for (int i = 0; i < COL_END; i++)
    {
        if (_iColType & 1 << i)
        {
			m_lstColSphere_Info[i].push_back({ _ColInfo ,_vNear});
            return;
        }
    }
}

void Engine::Collider::AddColOBB(const size_t& _iColType, const CINFO& _ColInfo, const D3DXVECTOR3& _vNear)
{
    m_iColOBB |= _iColType;
    for (int i = 0; i < COL_END; i++)
    {
        if (_iColType & 1 << i)
        {
			m_lstColOBB_Info[i].push_back({ _ColInfo ,_vNear });
            return;
        }
    }
}

void Engine::Collider::AddColType(const size_t& _iColType)
{
    m_iColType |= _iColType;
}

void Engine::Collider::ReBuild()
{
    if (nullptr != m_pTransform)
    {
        SettingCInfo();
    }
}

void Engine::Collider::SettingCInfo()
{
	m_tInfo.m_vInfo[INFO_POS] = m_pTransform->Get_Info(INFO_POS);
	D3DXMatrixIdentity(&m_tInfo.m_matWorld);
	for (size_t i = 0; i < INFO_POS; i++)
	{
		memcpy_s(&m_tInfo.m_vInfo[i], sizeof(D3DXVECTOR3), &m_tInfo.m_matWorld.m[i][0], sizeof(D3DXVECTOR3));
	}
	for (size_t i = 0; i < INFO_POS; ++i)
	{
		m_tInfo.m_vInfo[i] *= *(((_float*)&m_tInfo.m_vSize) + i);
	}
	_matrix			matRot[ROT_END];
	D3DXVECTOR3 vAngle = m_pTransform->GetAngle();
	D3DXMatrixRotationX(&matRot[ROT_X], vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], vAngle.z);
	for (size_t i = 0; i < INFO_POS; ++i)
	{
		for (size_t j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_tInfo.m_vInfo[i], &m_tInfo.m_vInfo[i], &matRot[j]);
		}
	}
	for (size_t i = 0; i < INFO_END; ++i)
	{
		memcpy_s(&m_tInfo.m_matWorld.m[i][0], sizeof(D3DXVECTOR3), &m_tInfo.m_vInfo[i], sizeof(D3DXVECTOR3));
	}
}

void Engine::Collider::ClearInfo()
{
	memset(&m_iColSphere, 0, 4);
	memset(&m_iColOBB, 0, 4);
	memset(&m_iColType, 0, 4);
	for (int i = 0; i < COL_END; i++)
	{
		m_lstColSphere_Info[i].clear();
		m_lstColOBB_Info[i].clear();
	}
}

void Engine::Collider::SelfBuilding()
{
    if (m_pTransform)
	{
		D3DXMatrixIdentity(&m_tInfo.m_matWorld);
		memcpy_s(&m_tInfo.m_vInfo[INFO_RIGHT], 12, &D3DXVECTOR3{ m_pTransform->Get_Info(INFO_RIGHT) * m_tInfo.m_vSize.x }, 12);
		memcpy_s(&m_tInfo.m_vInfo[INFO_UP], 12, &D3DXVECTOR3{ m_pTransform->Get_Info(INFO_UP) * m_tInfo.m_vSize.y }, 12);
		memcpy_s(&m_tInfo.m_vInfo[INFO_LOOK], 12, &D3DXVECTOR3{ m_pTransform->Get_Info(INFO_LOOK) * m_tInfo.m_vSize.z }, 12);
		memcpy_s(&m_tInfo.m_vInfo[INFO_POS], 12, &m_pTransform->Get_Info(INFO_POS), 12);
		memcpy_s(&m_tInfo.m_matWorld.m[0][0], 12, &m_tInfo.m_vInfo[INFO_RIGHT], 12);
		memcpy_s(&m_tInfo.m_matWorld.m[1][0], 12, &m_tInfo.m_vInfo[INFO_UP], 12);
		memcpy_s(&m_tInfo.m_matWorld.m[2][0], 12, &m_tInfo.m_vInfo[INFO_LOOK], 12);
        memcpy_s(&m_tInfo.m_matWorld.m[3][0], 12, &m_tInfo.m_vInfo[INFO_POS], 12);
	}
}

void Engine::Collider::SelfBuilding(const D3DXVECTOR3& _vRight, const D3DXVECTOR3& _vUp, const D3DXVECTOR3& _vLook, const D3DXVECTOR3& _vPos)
{
	D3DXMatrixIdentity(&m_tInfo.m_matWorld);
	memcpy_s(&m_tInfo.m_vInfo[INFO_RIGHT], 12, &D3DXVECTOR3{ _vRight * m_tInfo.m_vSize.x }, 12);
	memcpy_s(&m_tInfo.m_vInfo[INFO_UP], 12, &D3DXVECTOR3{ _vUp * m_tInfo.m_vSize.y }, 12);
	memcpy_s(&m_tInfo.m_vInfo[INFO_LOOK], 12, &D3DXVECTOR3{ _vLook * m_tInfo.m_vSize.z }, 12);
	memcpy_s(&m_tInfo.m_vInfo[INFO_POS], 12, &_vPos, 12);
	memcpy_s(&m_tInfo.m_matWorld.m[0][0], 12, &m_tInfo.m_vInfo[INFO_RIGHT], 12);
	memcpy_s(&m_tInfo.m_matWorld.m[1][0], 12, &m_tInfo.m_vInfo[INFO_UP], 12);
	memcpy_s(&m_tInfo.m_matWorld.m[2][0], 12, &m_tInfo.m_vInfo[INFO_LOOK], 12);
    memcpy_s(&m_tInfo.m_matWorld.m[3][0], 12, &m_tInfo.m_vInfo[INFO_POS], 12);
}

void Engine::Collider::SettingRadius()
{
    m_tInfo.m_fRadius = sqrt(m_tInfo.m_vSize.x * m_tInfo.m_vSize.x + m_tInfo.m_vSize.y * m_tInfo.m_vSize.y + m_tInfo.m_vSize.z * m_tInfo.m_vSize.z);
}

void Engine::Collider::Free()
{
    for (int i = 0; i < COL_END; i++)
    {
        m_lstColSphere_Info[i].clear();
        m_lstColOBB_Info[i].clear();
    }
    m_pTransform = nullptr;
    Component::Free();
}
