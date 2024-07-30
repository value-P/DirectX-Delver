#include "stdafx.h"
#include "Slime.h"
#include "Export_System.h"
#include "Export_Utility.h"

Slime::Slime(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

Slime::Slime(const Slime& rhs)
	: Super(rhs)
{
}

Slime::~Slime()
{
}

HRESULT Slime::Init_GameObject()
{
	Super::Init_GameObject();
	m_eMonsterID = MI_SLIME;

	return S_OK;
}

_int Slime::Update_GameObject(const _float& fTimeDelta)
{
	_int	iExit = Super::Update_GameObject(fTimeDelta);

	// 중력 적용
	m_pTransformCom->Move_Forward(&D3DXVECTOR3(0.0f, -1.0f, 0.0f), fTimeDelta, 3.0f);

	if (m_creatureStat->iHp > 0) // 몬스터가 살아있다면!
		if (!Lock_Tracking)
			Tracking(fTimeDelta);

	return _int();
}

void Slime::LateUpdate_GameObject()
{
	Super::LateUpdate_GameObject();
}

void Slime::Render_GameObject()
{
	Super::Render_GameObject();
}

HRESULT Slime::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pIdle_TextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_Slime_Idle_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Slime_Idle_Texture", pComponent });

	pComponent = m_pAttack_TextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_Slime_Attack_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Slime_Attack_Texture", pComponent });

	pComponent = m_pHit_TextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_Slime_Hit_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Slime_Hit_Texture", pComponent });

	pComponent = m_pDeath_TextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_Slime_Death_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Slime_Death_Texture", pComponent });

	m_pTextureCom = m_pIdle_TextureCom;

	dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrameSpeed(10.0f);

	return S_OK;
}

void Slime::OnCollided()
{
	if (m_pColliderCom == nullptr)
		return;

	// 지형, 벽 충돌
	if (m_pColliderCom->GetColOBB() & COL_WALL)
	{
		PlaneCollision();
	}

	if (m_creatureStat->iHp > 0 && m_bIs_HitStunState == false) // 몬스터가 살아있다면!
	{
		_int iHitCheck = m_creatureStat->iHp;

		// 플레이어 공격과의 충돌 코드 여기서 호출 또는 구현
		if (m_pColliderCom->GetColOBB() & COL_PLAYERATTACK)
		{
			m_creatureStat->iHp -= m_pColliderCom->GetColOBB_Info(COL_PLAYERATTACK).front().first.m_iDamage;
			CreateParticles(D3DCOLOR_ARGB(255, 0, 255, 0));
			CreateHitEffect(m_pColliderCom->GetColOBB_Info(COL_PLAYERATTACK).front().second);
		}
		if (m_pColliderCom->GetColOBB() & COL_PLAYER_PROJ)
		{
			m_creatureStat->iHp -= m_pColliderCom->GetColOBB_Info(COL_PLAYER_PROJ).front().first.m_iDamage;
			CreateParticles(D3DCOLOR_ARGB(255, 0, 255, 0));
			CreateHitEffect(m_pColliderCom->GetColOBB_Info(COL_PLAYER_PROJ).front().second);
		}
		if (m_pColliderCom->GetColSphere() & COL_EXPLOSION)
		{
			m_creatureStat->iHp -= m_pColliderCom->GetColSphere_Info(COL_EXPLOSION).front().first.m_iDamage;
			CreateParticles(D3DCOLOR_ARGB(255, 0, 255, 0));
		}

		if (m_creatureStat->iHp < iHitCheck)
			m_bIs_HitStunState = true;
	}
}

void Slime::OnDamaged()
{
}

Slime* Slime::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Slime* pInstance = new Slime(pGraphicDev);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	if (FAILED(pInstance->Add_Component()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	return pInstance;
}

void Slime::Free()
{
	Super::Free();
}

HRESULT Slime::Tracking(const _float& fTimeDelta)
{
	_vec3 vPlayerPos;
	_vec3 vMyPos;
	_vec3 vMovePos;

	m_pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);

	_vec3 vDir = vPlayerPos - vMyPos;

	vMovePos = *D3DXVec3Normalize(&vDir, &vDir) * fTimeDelta * m_fSpeed;

	m_pTransformCom->Move_Pos(vMovePos);

	return S_OK;
}
