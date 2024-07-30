#include "stdafx.h"
#include "BossMonster.h"

#include "Export_Utility.h"

BossMonster::BossMonster(LPDIRECT3DDEVICE9 pGrahicDev)
	:Creature(pGrahicDev)
{
}

BossMonster::BossMonster(const BossMonster& rhs)
	:Creature(rhs)
{
}

BossMonster::~BossMonster()
{
}

HRESULT BossMonster::Init_GameObject()
{
	__super::Init_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int BossMonster::Update_GameObject(const _float& fTimeDelta)
{
	switch (m_ePhase)
	{
	case PHASE1:
		Phase1(fTimeDelta);
		break;
	case PHASE2:
		Phase2(fTimeDelta);
		break;
	case PHASE3:
		Phase3(fTimeDelta);
		break;
	case PHASE4:
		Phase4(fTimeDelta);
		break;
	}

	_int	iExit = __super::Update_GameObject(fTimeDelta);

	return iExit;
}

void BossMonster::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

}

void BossMonster::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT BossMonster::Add_Component()
{
	Component* pComponent = nullptr;

	

	return S_OK;
}

void BossMonster::OnCollided()
{
	__super::OnCollided();
}

void BossMonster::OnDamaged()
{
	__super::OnDamaged();
}

// ÆäÀÌÁî
void BossMonster::Phase1(const _float& fTimeDelta)
{
	switch (m_eState)
	{
	case BOSS_SLEEP:
		break;
	case BOSS_IDLE:
		break;
	case BOSS_PATTERN1:
		break;
	case BOSS_PATTERN2:
		break;
	case BOSS_PATTERN3:
		break;
	case BOSS_GROGGY:
		break;
	case BOSS_HIT:
		break;
	case BOSS_DEAD:
		break;
	case BOSS_END:
		break;
	default:
		break;
	}
}

void BossMonster::Phase2(const _float& fTimeDelta)
{
	switch (m_eState)
	{
	case BOSS_SLEEP:
		break;
	case BOSS_IDLE:
		break;
	case BOSS_PATTERN1:
		break;
	case BOSS_PATTERN2:
		break;
	case BOSS_PATTERN3:
		break;
	case BOSS_GROGGY:
		break;
	case BOSS_HIT:
		break;
	case BOSS_DEAD:
		break;
	case BOSS_END:
		break;
	default:
		break;
	}
}

void BossMonster::Phase3(const _float& fTimeDelta)
{
	switch (m_eState)
	{
	case BOSS_SLEEP:
		break;
	case BOSS_IDLE:
		break;
	case BOSS_PATTERN1:
		break;
	case BOSS_PATTERN2:
		break;
	case BOSS_PATTERN3:
		break;
	case BOSS_GROGGY:
		break;
	case BOSS_HIT:
		break;
	case BOSS_DEAD:
		break;
	case BOSS_END:
		break;
	default:
		break;
	}
}

void BossMonster::Phase4(const _float& fTimeDelta)
{
	switch (m_eState)
	{
	case BOSS_SLEEP:
		break;
	case BOSS_IDLE:
		break;
	case BOSS_PATTERN1:
		break;
	case BOSS_PATTERN2:
		break;
	case BOSS_PATTERN3:
		break;
	case BOSS_GROGGY:
		break;
	case BOSS_HIT:
		break;
	case BOSS_DEAD:
		break;
	case BOSS_END:
		break;
	default:
		break;
	}
}

void BossMonster::Laser(const _float& fTimeDelta)
{
}

void BossMonster::RandomTeleport(const _float& fTimeDelta)
{
}

void BossMonster::ThrowCube(const _float& fTimeDelta)
{
}

void BossMonster::ActivateColumn(const _float& fTimeDelta)
{
}

void BossMonster::StunAndWave(const _float& fTimeDelta)
{
}

BossMonster* BossMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	BossMonster* pInstance = new BossMonster(pGraphicDev);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BossMonster Create Failed");
		return nullptr;
	}

	return pInstance;
}

void BossMonster::Free()
{
	__super::Free();
}
