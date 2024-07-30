#include "stdafx.h"
#include "DragonClow.h"

#include "Export_Utility.h"

DragonClow::DragonClow(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pDir, const _float& fSpeed, const _int& iDamage)
	:BossSkillObject(pGraphicDev)
{
	m_vStartDirection = *pDir;
	m_fSpeed = fSpeed;
	m_iDamage = iDamage;
}

DragonClow::~DragonClow()
{
}

HRESULT DragonClow::Init_GameObject()
{
	HRESULT iResult = __super::Init_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pColliderCom->SetColLayer(COL_MONSTER_PROJ);
	m_pColliderCom->SetDamage(m_iDamage);
	m_pColliderCom->SetThisPointer(this);
	m_pColliderCom->SetSize(1.f, 1.f, 1.f);
	m_pColliderCom->SetTransform(m_pTransformCom);

	m_pParticles = PTrace::Create(m_pGraphicDev);
	if (nullptr == m_pParticles) { return E_FAIL; }
	PSMgr::GetInstance()->AddParticles(m_pParticles);
	m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 139, 0, 190));

	return iResult;
}

_int DragonClow::Update_GameObject(const _float& fTimeDelta)
{
	m_fAliveTime += fTimeDelta;
	if (m_fAliveTime > 10.f)
		m_bAlive = false;

	if (!m_bAlive) { return -1; }
	m_pTransformCom->Move_Forward(&m_vStartDirection, fTimeDelta, m_fSpeed);

	m_fParticleGenerateTime += fTimeDelta;
	if (m_fParticleGenerateTime > 0.1f)
	{
		m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 139, 0, 190));
		m_fParticleGenerateTime = 0.0f;
	}

	_int iResult = __super::Update_GameObject(fTimeDelta);

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

	m_pColliderCom->ReBuild();
	Engine::AddCollider(m_pColliderCom);

	return iResult;
}

void DragonClow::LateUpdate_GameObject()
{
	On_Collided();
	__super::LateUpdate_GameObject();
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
}

void DragonClow::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT DragonClow::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../../Resource/Texture/Effect/Boss/DragonClow.png");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

	pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

	return S_OK;
}

void DragonClow::On_Collided()
{

}

DragonClow* DragonClow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pDir, const _float& fSpeed, const _int& iDamage)
{
	DragonClow* pInstance = new DragonClow(pGraphicDev, pDir, fSpeed, iDamage);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("DragonClow Create Failed");
		return nullptr;
	}

	return pInstance;

}

void DragonClow::Free()
{
	__super::Free();
}
