#include "stdafx.h"
#include "MeleeAttack.h"
#include "Export_System.h"
#include "Export_Utility.h"

MeleeAttack::MeleeAttack(LPDIRECT3DDEVICE9 _pGraphicDevice)
    : Effect(_pGraphicDevice)
{
}

MeleeAttack::~MeleeAttack()
{
}

MeleeAttack* MeleeAttack::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3 & _vPos, const D3DXVECTOR3 & _vLook, const float& _fColSize, const int& _iDamage)
{
	MeleeAttack* pInstance = new MeleeAttack(_pGraphicDevice);

	if (FAILED(pInstance->Init_MeleeAttack(_vPos, _vLook, _fColSize, _iDamage)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_MeleeAttack Failed", L"MeleeAttack::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT MeleeAttack::Init_MeleeAttack(const D3DXVECTOR3 & _vPos, const D3DXVECTOR3 & _vLook, const float& _fColSize, const int& _iDamage)
{
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
    }

    m_iDamage = _iDamage;
    m_pColliderCom->SetDamage(m_iDamage);
	m_pColliderCom->SetSize(_fColSize, _fColSize, _fColSize);
	D3DXVECTOR3 vRight, vUp, vLook;
    D3DXVec3Normalize(&vLook, &_vLook);
	D3DXVec3Cross(&vRight, &D3DXVECTOR3{ 0.0f,1.0f,0.0f }, &vLook);
    D3DXVec3Cross(&vUp, &vLook, &vRight);
    m_pColliderCom->SelfBuilding(vRight, vUp, vLook, _vPos);

    return S_OK;
}

int MeleeAttack::Update_GameObject(const _float& _fTimeDelta)
{
    if (m_bAttack)
    {
        return -1;
    }

    Engine::AddCollider(m_pColliderCom);
    m_bAttack = true;

    return 0;
}

void MeleeAttack::LateUpdate_GameObject()
{
}

void MeleeAttack::Render_GameObject()
{
}

void MeleeAttack::SetColType(COLLAYER _eType)
{
    if (nullptr != m_pColliderCom)
    {
        m_pColliderCom->SetColLayer(_eType);
    }
}

HRESULT MeleeAttack::Add_Component()
{
    m_pColliderCom = dynamic_cast<Collider*>(ProtoMgr::GetInstance()->Clone_Proto(L"Proto_Collider"));
    if (nullptr == m_pColliderCom) { return E_FAIL; }
    m_mapComponent[ID_STATIC].insert({ L"Proto_Collider",m_pColliderCom });

    return S_OK;
}

void MeleeAttack::Free()
{
    GameObject::Free();
}
