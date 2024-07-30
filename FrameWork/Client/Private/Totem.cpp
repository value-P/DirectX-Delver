#include "stdafx.h"
#include "Totem.h"

#include "Export_System.h"
#include "Export_Utility.h"

Totem::Totem(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, Transform* pTarget)
	:BossSkillObject(pGraphicDev), m_pTarget(pTarget)
{
	m_vStartPos = *pPos;
}

Totem::~Totem()
{
}

HRESULT Totem::Init_GameObject()
{
	HRESULT iResult = __super::Init_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(m_vStartPos);
	m_pTransformCom->Set_Scale(3.f, 3.f, 3.f);

	m_pColliderCom->SetColLayer(COL_MONSTER);
	m_pColliderCom->SetThisPointer(this);
	m_pColliderCom->SetSize(1.f, 1.f, 1.f);
	m_pColliderCom->SetTransform(m_pTransformCom);

	return iResult;
}

_int Totem::Update_GameObject(const _float& fTimeDelta)
{
	_int iResult = __super::Update_GameObject(fTimeDelta);
	if (!m_bAlive) { return -1; }
	_matrix matWorld, matView, matBill;
	matWorld = *m_pTransformCom->Get_WorldMatrix();

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// 빌 * (스 * 자 * 이)

	_matrix newMatWorld = matBill * matWorld;

	m_pTransformCom->Set_WorldMatrix(&newMatWorld);

	m_pColliderCom->ReBuild();
	Engine::AddCollider(m_pColliderCom);

	if (m_bActivated)
	{
		m_fParticleGenerateTime += fTimeDelta;
		if (m_fParticleGenerateTime > 0.1f)
		{
			m_fParticleGenerateTime = 0.0f;
			PSystem* pPSystem = PCharging::Create(m_pGraphicDev, m_pTarget->Get_Info(INFO_POS));
			pPSystem->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 131, 0, 131), 5.f);
			PSMgr::GetInstance()->AddParticles(pPSystem);
		}
	}

	return iResult;
}

void Totem::LateUpdate_GameObject()
{
	On_Collided();
	__super::LateUpdate_GameObject();
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
}

void Totem::Render_GameObject()
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

void Totem::Destroy()
{
	// 터지는 파티클 색 다르게
	if (m_bActivated)
	{
		// 파티클 제조
		PSystem* pPSystem = PSpread::Create(m_pGraphicDev);
		if (nullptr == pPSystem) { return; }
		PSMgr::GetInstance()->AddParticles(pPSystem);
		for (int i = 0; i < 16; i++)
		{
			pPSystem->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 211, 11, 0));
		}
	}
	else
	{
		PSystem* pPSystem = PSpread::Create(m_pGraphicDev);
		if (nullptr == pPSystem) { return; }
		PSMgr::GetInstance()->AddParticles(pPSystem);
		for (int i = 0; i < 16; i++)
		{
			pPSystem->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 211, 211, 0));
		}
	}

	m_bAlive = false;
}

HRESULT Totem::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

	pComponent = m_pActiveTex = Texture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../../Resource/Texture/Effect/Boss/ActivatedTotem.png");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"ActiveTex", pComponent});

	pComponent = m_pInActiveTex = Texture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../../Resource/Texture/Effect/Boss/InActivatedTotem.png");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"InActiveTex", pComponent});

	m_pTextureCom = m_pActiveTex;
	m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, m_pTextureCom });

	return S_OK;
}

void Totem::On_Collided()
{
	if (m_bActivated)
	{
		if (m_pColliderCom->GetColOBB() & COL_PLAYERATTACK)
		{
			m_bActivated = false;
			m_pTextureCom = m_pInActiveTex;
			m_mapComponent[ID_STATIC][TAG_TEXTURE] = m_pTextureCom;
		}
	}
}

Totem* Totem::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, Transform* pTarget)
{
	Totem* pInstance = new Totem(pGraphicDev, pPos, pTarget);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Totem Create Failed");
		return nullptr;
	}

	return pInstance;
}

void Totem::Free()
{

	m_mapComponent[ID_STATIC].erase(TAG_TEXTURE);
	__super::Free();
}
