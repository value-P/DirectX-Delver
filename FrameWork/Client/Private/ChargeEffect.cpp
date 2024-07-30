#include "stdafx.h"
#include "ChargeEffect.h"
#include "BossMonster2.h"

#include "Export_Utility.h"

ChargeEffect::ChargeEffect(LPDIRECT3DDEVICE9 pGraphicDev, BossMonster2* pOwner, Transform* pTargetTransform)
	:BossSkillObject(pGraphicDev), m_pOwner(pOwner), m_pTargetTransform(pTargetTransform)
{
}

ChargeEffect::ChargeEffect(const ChargeEffect& rhs)
	:BossSkillObject(rhs), m_pOwner(rhs.m_pOwner), m_pTargetTransform(rhs.m_pTargetTransform)
{
}

ChargeEffect::~ChargeEffect()
{
}

HRESULT ChargeEffect::Init_GameObject()
{
	HRESULT iResult = __super::Init_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	PSystem* pSystem = PLaserCharging::Create(m_pGraphicDev, &m_pTransformCom->Get_Info(INFO_POS), 5.f, 1.5f);
	PSMgr::GetInstance()->AddParticles(pSystem);

	return iResult;
}

_int ChargeEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pOwner->IsChargeEnd())
		m_bAlive = false;

	_vec3 vPos = m_pOwner->Get_Transform()->Get_Info(INFO_POS);
	_vec3 vTarget = m_pTargetTransform->Get_Info(INFO_POS);
	_vec3 Dir = vTarget - vPos;
	Dir.y = 0.f;
	D3DXVec3Normalize(&Dir, &Dir);

	vPos.y += 0.5f;
	vPos += Dir;

	m_pTransformCom->Set_Pos(vPos);

	_int iResult = __super::Update_GameObject(fTimeDelta);

	#pragma region ºôº¸µå
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
#pragma endregion 

	return iResult;
}

void ChargeEffect::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
}

void ChargeEffect::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 250);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT ChargeEffect::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = AnimationTexture::Create(m_pGraphicDev,
		L"../../Resource/Texture/Effect/Boss/Charge/Charge%d.png", 16, 8.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TAG_TEXTURE,pComponent });

	pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

	return S_OK;
}

void ChargeEffect::On_Collided()
{
}

ChargeEffect* ChargeEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, BossMonster2* pOwner, Transform* pTargetTransform)
{
	ChargeEffect* pInstance = new ChargeEffect(pGraphicDev, pOwner, pTargetTransform);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("ChargeEffect Create Failed");
		return nullptr;
	}

	return pInstance;
}

void ChargeEffect::Free()
{
	__super::Free();
}
