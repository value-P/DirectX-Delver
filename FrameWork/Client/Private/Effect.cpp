#include "stdafx.h"
#include "Effect.h"
#include "Export_System.h"
#include "Export_Utility.h"

Effect::Effect(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: GameObject(_pGraphicDevice)
{
}

Effect::~Effect()
{
}

HRESULT Effect::Init_Effect()
{
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	return S_OK;
}

int Effect::Update_GameObject(const _float& _fTimeDelta)
{
	if (!m_bAlive)
	{
		return -1;
	}

	m_fAliveTime -= _fTimeDelta;

	if (m_fAliveTime < 0.0f)
	{
		m_bAlive = false;
	}

	return 0;
}

void Effect::LateUpdate_GameObject()
{
	GameObject::Compute_ViewZ(&(m_pTransformCom->Get_Info(INFO_POS)));
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
}

void Effect::SetPos(const D3DXVECTOR3& _vPos)
{
	m_pTransformCom->Set_Pos(_vPos.x, _vPos.y, _vPos.z);
}

HRESULT Effect::Add_Component()
{
	m_pBufferCom = dynamic_cast<RcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", m_pBufferCom });

	m_pTransformCom = dynamic_cast<Transform*>(Engine::Clone_Proto(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) { return E_FAIL; }
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", m_pTransformCom });

	return S_OK;
}

void Effect::ComputeBillBoard()
{
	D3DXMATRIX matView, matBill;

	D3DXMatrixIdentity(&matBill);	// ºôº¸µå Çà·Ä ÃÊ±âÈ­
	
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);	// ºä Çà·ÄÀ» ¾ò¾î¿È

	memcpy_s(&matBill._11, 4, &matView._11, 4);
	memcpy_s(&matBill._13, 4, &matView._13, 4);
	memcpy_s(&matBill._31, 4, &matView._31, 4);
	memcpy_s(&matBill._33, 4, &matView._33, 4);

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &(matBill * *(m_pTransformCom->Get_WorldMatrix())));
}

void Effect::Free()
{
	GameObject::Free();
}
