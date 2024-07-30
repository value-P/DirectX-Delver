#include "stdafx.h"
#include "Fire.h"
#include "Export_System.h"
#include "Export_Utility.h"

Fire::Fire(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: EnvironmentBase(_pGraphicDevice)
{
}

Fire::~Fire()
{
}

Fire* Fire::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3& _vPos)
{
	Fire* pInstance = new Fire(_pGraphicDevice);

	if (FAILED(pInstance->Init_Fire(_vPos)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_Fire Failed", L"Fire::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT Fire::Init_Fire(const D3DXVECTOR3& _vPos)
{
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	m_pAniTexCom->SetFrameSpeed(10.f);

	m_pTransformCom->Set_Pos(_vPos);
	m_pTransformCom->Set_Scale(2.3f, 2.3f, 2.3f);

	return S_OK;
}

_int Fire::Update_GameObject(const float& _fTimeDelta)
{
	GameObject::Update_GameObject(_fTimeDelta);
	
	return 0;
}

void Fire::LateUpdate_GameObject()
{
	GameObject::Compute_ViewZ(&(m_pTransformCom->Get_Info(INFO_POS)));
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
}

void Fire::Render_GameObject()
{
	ComputeBillBoard();
	/*m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pAniTexCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	/*m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);*/
}

HRESULT Fire::Add_Component()
{
	m_pBufferCom = dynamic_cast<RcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", m_pBufferCom });

	m_pTransformCom = dynamic_cast<Transform*>(Engine::Clone_Proto(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) { return E_FAIL; }
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", m_pTransformCom });

	m_pAniTexCom = dynamic_cast<AnimationTexture*>(Clone_Proto(L"Proto_Fire"));
	if (nullptr == m_pAniTexCom) { return E_FAIL; }
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Fire", m_pAniTexCom });

	return S_OK;
}

void Fire::Free()
{
	GameObject::Free();
}
