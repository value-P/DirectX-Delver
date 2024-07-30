#include "stdafx.h"
#include "EnvironmentBase.h"
#include "Export_Utility.h"
//#include <time.h> 

EnvironmentBase::EnvironmentBase(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
	
}

EnvironmentBase::EnvironmentBase(const EnvironmentBase& rhs)
	: Super(rhs)
{
}

EnvironmentBase::~EnvironmentBase()
{
}

HRESULT EnvironmentBase::Init_GameObject()
{
	Super::Init_GameObject();

	_vec3 randomPos = { static_cast<float>(rand() % 120), 0.f, static_cast<float>(rand() % 120) };
	m_pTransformCom->Set_Info(INFO_POS, &randomPos);

	return S_OK;
}

_int EnvironmentBase::Update_GameObject(const _float& fTimeDelta)
{
	_int	iExit = Super::Update_GameObject(fTimeDelta);

	_matrix matWorld, matView, matBill;
	matWorld = *m_pTransformCom->Get_WorldMatrix();

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// ºô * (½º * ÀÚ * ÀÌ)

	_matrix newMatWorld = matBill * matWorld;

	m_pTransformCom->Set_WorldMatrix(&newMatWorld);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void EnvironmentBase::LateUpdate_GameObject()
{

	Super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void EnvironmentBase::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT EnvironmentBase::Add_Component(wstring textureKey)
{
	Component* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<RcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(textureKey));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ textureKey, pComponent });

	pComponent = m_pColliderCom = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Collider",pComponent });
	m_pColliderCom->SetTransform(m_pTransformCom);

	return S_OK;
}

void EnvironmentBase::OnCollided()
{
}

void EnvironmentBase::ComputeBillBoard()
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

EnvironmentBase* EnvironmentBase::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer)
{
	EnvironmentBase* pInstance = new EnvironmentBase(pGraphicDev);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("EnvironmentBase Create Failed");
		return nullptr;
	}

	if (FAILED(pInstance->Add_Component(textureKey)))
	{
		Safe_Release(pInstance);
		MSG_BOX("EnvironmentBase Create Failed");
		return nullptr;
	}

	pInstance->Set_TextureKey(textureKey);

	pInstance->Get_Collider()->SetSize(colSize);
	pInstance->Get_Collider()->SetColLayer(colLayer);
	pInstance->Get_Collider()->SetThisPointer(pInstance);

	return pInstance;
}

void EnvironmentBase::Free()
{
	Super::Free();
}
