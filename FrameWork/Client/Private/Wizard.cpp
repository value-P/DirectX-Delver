#include "stdafx.h"
#include "Wizard.h"
#include "Export_System.h"
#include "Export_Utility.h"

Wizard::Wizard(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

Wizard::Wizard(const Wizard& rhs)
	: Super(rhs)
{
}

Wizard::~Wizard()
{
}

HRESULT Wizard::Init_GameObject()
{
	Super::Init_GameObject();
	m_eMonsterID = MI_WIZARD;
	return S_OK;
}

_int Wizard::Update_GameObject(const _float& fTimeDelta)
{
	_int	iExit = Super::Update_GameObject(fTimeDelta);

	// �߷� ����
	m_pTransformCom->Move_Forward(&D3DXVECTOR3(0.0f, -1.0f, 0.0f), fTimeDelta, 3.0f);

	if (m_creatureStat->iHp > 0) // ���Ͱ� ����ִٸ�!
		if (!Lock_Tracking)
			Tracking(fTimeDelta);

	return _int();
}

void Wizard::LateUpdate_GameObject()
{
	Super::LateUpdate_GameObject();
}

void Wizard::Render_GameObject()
{
	Super::Render_GameObject();
}

HRESULT Wizard::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pIdle_TextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_Wizard_Idle_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Wizard_Idle_Texture", pComponent });

	pComponent = m_pAttack_TextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_Wizard_Attack_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Wizard_Attack_Texture", pComponent });

	pComponent = m_pHit_TextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_Wizard_Hit_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Wizard_Hit_Texture", pComponent });

	pComponent = m_pDeath_TextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_Wizard_Death_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Wizard_Death_Texture", pComponent });

	m_pTextureCom = m_pIdle_TextureCom;

	dynamic_cast<AnimationTexture*>(m_pTextureCom)->SetFrameSpeed(10.0f);

	return S_OK;
}

void Wizard::OnCollided()
{
	//check();

	if (m_pColliderCom == nullptr)
		return;

	// ����, �� �浹
	if (m_pColliderCom->GetColOBB() & COL_WALL)
	{
		PlaneCollision();
	}

	if (m_creatureStat->iHp > 0) // ���Ͱ� ����ִٸ�!
	{
		_int iHitCheck = m_creatureStat->iHp;

		// �÷��̾� ���ݰ��� �浹 �ڵ� ���⼭ ȣ�� �Ǵ� ����
		if (m_pColliderCom->GetColOBB() & COL_PLAYERATTACK)
		{
			m_creatureStat->iHp -= m_pColliderCom->GetColOBB_Info(COL_PLAYERATTACK).front().first.m_iDamage;
			
			CreateParticles(D3DCOLOR_ARGB(255, 255, 0, 0));
			CreateHitEffect(m_pColliderCom->GetColOBB_Info(COL_PLAYERATTACK).front().second);
		}
		if (m_pColliderCom->GetColOBB() & COL_PLAYER_PROJ)
		{
			m_creatureStat->iHp -= m_pColliderCom->GetColOBB_Info(COL_PLAYER_PROJ).front().first.m_iDamage;
			
			CreateParticles(D3DCOLOR_ARGB(255, 255, 0, 0));
			CreateHitEffect(m_pColliderCom->GetColOBB_Info(COL_PLAYER_PROJ).front().second);
		}
		if (m_pColliderCom->GetColSphere() & COL_EXPLOSION)
		{
			m_creatureStat->iHp -= m_pColliderCom->GetColSphere_Info(COL_EXPLOSION).front().first.m_iDamage;
			CreateParticles(D3DCOLOR_ARGB(255, 255, 0, 0));
		}

		if (m_creatureStat->iHp < iHitCheck)
			m_bIs_HitStunState = true;
	}
}

void Wizard::OnDamaged()
{
}

Wizard* Wizard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Wizard* pInstance = new Wizard(pGraphicDev);

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

void Wizard::Free()
{
	Super::Free();
}

HRESULT Wizard::Tracking(const _float& fTimeDelta)
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