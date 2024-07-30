#include "stdafx.h"
#include "Explosion.h"
#include "Export_System.h"
#include "Export_Utility.h"

Explosion::Explosion(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: Effect(_pGraphicDevice)
{
}

Explosion::~Explosion()
{
}

Explosion* Explosion::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3& _vPos)
{
	Explosion* pInstance = new Explosion(_pGraphicDevice);

	if (FAILED(pInstance->Init_Explosion(_vPos)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_Explosion Failed", L"Explosion::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT Explosion::Init_Explosion(const D3DXVECTOR3& _vPos)
{
	if (FAILED(Effect::Init_Effect()))
	{
		return E_FAIL;
	}

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	m_pColliderCom->SetSize(2.0f, 2.0f, 2.0f);
	m_pColliderCom->SetColLayer(COL_EXPLOSION);

	m_pTransformCom->Set_Scale(2.0f, 2.0f, 2.0f);
	m_pTransformCom->Set_Pos(_vPos);

	m_pAniTexCom->SetFrameSpeed(8.0f);

	return S_OK;
}

int Explosion::Update_GameObject(const _float& _fTimeDelta)
{
	GameObject::Update_GameObject(_fTimeDelta);
	if (1 & m_pAniTexCom->GetRepeatCnt()) { return -1; }

	if (!m_bBoom)
	{
		m_pColliderCom->ReBuild();
		Engine::AddCollider(m_pColliderCom);
		m_bBoom = true;
	}

	return 0;
}

void Explosion::LateUpdate_GameObject()
{
	Effect::LateUpdate_GameObject();
}

void Explosion::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	ComputeBillBoard();
	m_pAniTexCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT Explosion::Add_Component()
{
	m_pAniTexCom = dynamic_cast<AnimationTexture*>(Engine::Clone_Proto(L"Proto_Explosion"));
	if (nullptr == m_pAniTexCom) { return E_FAIL; }
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Explosion", m_pAniTexCom });

	m_pColliderCom = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
	if (nullptr == m_pColliderCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_Collider",m_pColliderCom });
	m_pColliderCom->SetTransform(m_pTransformCom);

	return S_OK;
}

void Explosion::Free()
{
	GameObject::Free();
}
