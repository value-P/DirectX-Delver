#include "stdafx.h"
#include "SpawnMarker.h"
#include "Slime.h"
#include "Bat.h"
#include "Zombi.h"
#include "Warrior.h"
#include "Wizard.h"
#include "Druid.h"
#include "Monk.h"

#include "Export_Utility.h"

SpawnMarker::SpawnMarker(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{

}

SpawnMarker::SpawnMarker(const SpawnMarker& rhs)
	: Super(rhs)
{
	
}

SpawnMarker::~SpawnMarker()
{
	
}

HRESULT SpawnMarker::Init_GameObject()
{
	Super::Init_GameObject();

	return S_OK;
}

Engine::_int SpawnMarker::Update_GameObject(const _float& fTimeDelta)
{
	m_fDeltaTime = fTimeDelta;

	_int	iExit = Super::Update_GameObject(fTimeDelta);

	AddCollider(m_pColliderCom);

	return iExit;
}

void SpawnMarker::LateUpdate_GameObject()
{
	switch (m_eState)
	{
	case SPAWNMARKER_IDLE:
	{

	}
	break;
	case SPAWNMARKER_SPAWN:
	{
		SpawnMonster();
	}
	break;
	case SPAWNMARKER_ACTIVATE:
	{
		Activate();
	}
	break;
	case SPAWNMARKER_DEACTIVATE:
	{
		DeActivate();
	}
	break;
	}

	Super::LateUpdate_GameObject();
}

void SpawnMarker::Render_GameObject()
{
	
}

void SpawnMarker::SpawnMonster()
{
	if (m_pMonster)
		return;

	// 한번 소환
	switch (m_eMonsterID)
	{
	case MI_SLIME:
	{
		m_pMonster = Slime::Create(m_pGraphicDev);
		if (m_pMonster == nullptr)
			break;

		Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SpawnedSlime", m_pMonster);
		CreatureStat monsterStat = CreatureStat(100, 100, 1, 0, 0, 20);
		m_pMonster->Set_CreatureStat(&monsterStat);
		(m_pMonster)->Set_BehaviorStat(15.f, 2.f, 2.f, 0.f);
		m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
	}
	break;
	case MI_ZOMBI:
	{
		if (m_eRoom == SPAWNMARKER_ARCHER)
		{
			m_pMonster = Zombi::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SpawnedZombie", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 2.f, 2.f, 0.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}

		else
		{
			m_pMonster = Zombi::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SpawnedZombie", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 2.f, 2.f, 4.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}
	}
	break;
	case MI_WARRIOR:
	{
		if (m_eRoom == SPAWNMARKER_ARCHER)
		{
			m_pMonster = Warrior::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SpawnedWarrior", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 2.f, 2.f, 0.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}

		else
		{
			m_pMonster = Warrior::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SpawnedWarrior", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 2.f, 2.f, 4.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}
	}
	break;
	case MI_BAT:
	{
		if (m_eRoom == SPAWNMARKER_ARCHER)
		{
			m_pMonster = Bat::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;
			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SpawnedBat", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 2.f, 2.f, 5.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}

		else
		{
			m_pMonster = Bat::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;
			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SpawnedBat", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 2.f, 2.f, 5.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}
	}
	break;
	case MI_WIZARD:
	{
		if (m_eRoom == SPAWNMARKER_ARCHER)
		{
			m_pMonster = Wizard::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"Wizard", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 10.f, 2.f, 0.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}

		else if (m_eRoom == SPAWNMARKER_LADDER)
		{
			m_pMonster = Wizard::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"Wizard", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(25.f, 20.f, 2.f, 0.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}
		
		else
		{
			m_pMonster = Wizard::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"Wizard", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 10.f, 2.f, 4.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}
	}
	break;
	case MI_DRUID:
	{
		if (m_eRoom == SPAWNMARKER_ARCHER)
		{
			m_pMonster = Druid::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"Druid", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 10.f, 2.f, 0.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}

		else if (m_eRoom == SPAWNMARKER_LADDER)
		{
			m_pMonster = Druid::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"Druid", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(25.f, 20.f, 2.f, 0.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}

		else
		{
			m_pMonster = Druid::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"Druid", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 10.f, 2.f, 4.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}
	}
	break;
	case MI_MONK:
	{
		if (m_eRoom == SPAWNMARKER_ARCHER)
		{
			m_pMonster = Monk::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"Monk", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 10.f, 2.f, 0.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}

		else if (m_eRoom == SPAWNMARKER_LADDER)
		{
			m_pMonster = Monk::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"Monk", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(25.f, 20.f, 2.f, 0.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}

		else
		{
			m_pMonster = Monk::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"Monk", m_pMonster);
			CreatureStat monsterStat = CreatureStat(12, 12, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 10.f, 2.f, 4.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}
	}
	break;
	}

	m_eState = SPAWNMARKER_DEACTIVATE;
}

void SpawnMarker::Activate()
{
	// 루프
	m_fSpawnTime += m_fDeltaTime;
	if (m_fSpawnTime >= 2.f)
	{
		switch (m_eMonsterID)
		{	
		case MI_SLIME:
		{
			m_pMonster = Slime::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SpawnedSlime", m_pMonster);
			CreatureStat monsterStat = CreatureStat(30, 30, 1, 0, 0, 20);
			m_pMonster->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 2.f, 2.f, 3.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}
		break;
		case MI_ZOMBI:
		{
			m_pMonster = Zombi::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SpawnedZombie", m_pMonster);
			CreatureStat monsterStat = CreatureStat(10, 10, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 2.f, 2.f, 4.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}
		break;
		case MI_WARRIOR:
		{
			m_pMonster = Warrior::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SpawnedWarrior", m_pMonster);
			CreatureStat monsterStat = CreatureStat(10, 10, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 2.f, 2.f, 4.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}
		break;
		case MI_BAT:
		{
			m_pMonster = Bat::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;
			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SpawnedBat", m_pMonster);
			CreatureStat monsterStat = CreatureStat(10, 10, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 2.f, 2.f, 5.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}
		break;
		case MI_WIZARD:
		{
			m_pMonster = Wizard::Create(m_pGraphicDev);
			if (m_pMonster == nullptr)
				break;

			Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"Wizard", m_pMonster);
			CreatureStat monsterStat = CreatureStat(10, 10, 1, 0, 0, 20);
			(m_pMonster)->Set_CreatureStat(&monsterStat);
			(m_pMonster)->Set_BehaviorStat(15.f, 10.f, 2.f, 4.f);
			m_pMonster->Get_Transform()->Set_Pos(this->Get_Transform()->Get_Info(INFO_POS));
		}
		break;
		}

		m_fSpawnTime = 0.f;
		m_iSpawnCount++;
	}

	if (m_iSpawnCount == 4)
	{
		m_eState = SPAWNMARKER_DEACTIVATE;
	}
}

void SpawnMarker::DeActivate()
{
	// 

}

SpawnMarker* SpawnMarker::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 colSize, COLLAYER colLayer, _vec3& initPos, MONSTER_ID monsterID, SpawnMarkerRoom spawnMarkerRoom, SpawnMarkerState state)
{
	SpawnMarker* pInstance = new SpawnMarker(pGraphicDev);
	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SpawnMarker Create Failed");
		return nullptr;
	}

	if (FAILED(pInstance->Add_Component()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SpawnMarker Create Failed");
		return nullptr;
	}

	pInstance->Get_Collider()->SetSize(colSize);
	pInstance->Get_Collider()->SetColLayer(colLayer);
	pInstance->Get_Collider()->SetThisPointer(pInstance);

	pInstance->Get_Transform()->Set_Pos(initPos);

	pInstance->Set_MonsterID(monsterID);
	pInstance->Set_MarkerRoom(spawnMarkerRoom);
	pInstance->Set_State(state);

	return pInstance;
}

HRESULT SpawnMarker::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pColliderCom = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Collider",pComponent });
	m_pColliderCom->SetTransform(m_pTransformCom);

	return S_OK;
}

void SpawnMarker::Free()
{
	m_pMonster = nullptr;

	Super::Free();
}
