#include "stdafx.h"
#include "MeteorLine.h"
#include "Meteor.h"

#include "Export_Utility.h"

MeteorLine::MeteorLine(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, const _float& fSpeed, const _int& iDamage)
    :BossSkillObject(pGraphicDev)
{
	m_fSpeed = fSpeed;
	m_vStartPos = *pPos;
	m_iDamage = iDamage;
}

MeteorLine::MeteorLine(const MeteorLine& rhs)
    :BossSkillObject(rhs)
{
	m_fSpeed = rhs.m_fSpeed;
	m_vStartPos = rhs.m_vStartPos;
	m_iDamage = rhs.m_iDamage;
}

MeteorLine::~MeteorLine()
{
}

HRESULT MeteorLine::Init_GameObject()
{
	HRESULT iResult = __super::Init_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 pos = m_vStartPos;
	pos.y -= 20.f;
	m_pTransformCom->Set_Pos(pos);
	m_pTransformCom->Set_Scale(0.5f, 20.f, 1.f);

	return iResult;
}

_int MeteorLine::Update_GameObject(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_fTime > 5.f)
	{
		Create_Meteor();
		m_bAlive = false;
	}

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

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return iResult;
}

void MeteorLine::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void MeteorLine::Render_GameObject()
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

HRESULT MeteorLine::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Effect/Boss/Effect7.png");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

	pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

	return S_OK;
}

void MeteorLine::Create_Meteor()
{
	Meteor* Obj = Meteor::Create(m_pGraphicDev, &m_vStartPos, m_fSpeed, m_iDamage);
	Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
		Add_GameObject(L"Meteor", Obj);
}

MeteorLine* MeteorLine::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, const _float& fSpeed, const _int& iDamage)
{
	MeteorLine* pInstance = new MeteorLine(pGraphicDev, pPos,fSpeed, iDamage);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MeteorLine Create Failed");
		return nullptr;
	}

	return pInstance;

}

void MeteorLine::Free()
{
	__super::Free();
}
