#include "stdafx.h"
#include "InteractionTrigger.h"
#include "MovableWall.h"
#include "Player.h"
#include "SpawnMarker.h"
#include "FloatingFloor.h"
#include "Stage2.h"
#include "MainApp.h"

#include "Export_System.h"
#include "Export_Utility.h"

InteractionTrigger::InteractionTrigger(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

InteractionTrigger::InteractionTrigger(const InteractionTrigger& rhs)
	: Super(rhs)
{
}

InteractionTrigger::~InteractionTrigger()
{
}

HRESULT InteractionTrigger::Init_GameObject()
{
	Super::Init_GameObject();

	m_eIntState = INTSTATE_IDLE;

	return S_OK;
}

_int InteractionTrigger::Update_GameObject(const _float& fTimeDelta)
{
	if (m_eTriType != 0)
	{
		m_fDeltaTime = fTimeDelta;

		_int	iExit = Super::Update_GameObject(fTimeDelta);

		AddCollider(m_pColliderCom);

		Add_RenderGroup(RENDER_NONALPHA, this);

		return iExit;
	}

	//if (m_iCount < 1)
	//{
	//	if (m_pLinkedInt)
	//		m_pLinkedInt->Set_IntState(INTSTATE_MOVE);
	//}

	_int	iExit = Super::Update_GameObject(fTimeDelta);

	return iExit;
}

void InteractionTrigger::LateUpdate_GameObject()
{
	if (m_bIsWarped)
		return;

	if (m_bIsTriggered == false)
	{
		switch (m_eIntState)
		{
		case INTSTATE_TRIGGER:
		{
			TriggerAction();
		}
		break;
		}
	}

	if (m_eTriType & TRITYPE_SEAL)
		TriggerSeal();

	OnCollided();

	Super::LateUpdate_GameObject();
}

void InteractionTrigger::Render_GameObject()
{
	if (m_pColliderCom->GetColSphere() & COL_PLAYER)
	{
		if (m_eRoomType == ROOMTYPE_CENTER && !(m_pPlayer->Get_SealCount() == 3))
		{
			Engine::Render_Font(L"Font_LogoText", L"열 수 없다", &_vec4(0, 0, WINCX, WINCY), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}

		else if (m_eRoomType == ROOMTYPE_CENTER && (m_pPlayer->Get_SealCount() == 3))
		{
			Engine::Render_Font(L"Font_LogoText", L"E : 열기", &_vec4(0, 0, WINCX, WINCY), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}

		if (m_eRoomType == ROOMTYPE_LADDER)
		{
			Engine::Render_Font(L"Font_LogoText", L"E : 레버를 당긴다", &_vec4(0, 0, WINCX, WINCY), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
	}
}

InteractionTrigger* InteractionTrigger::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 colSize, COLLAYER colLayer, _vec3& initPos, RoomType roomType, InteractableType intType, _uint triType, _uint count)
{
	InteractionTrigger* pInstance = new InteractionTrigger(pGraphicDev);
	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("InteractionTrigger Create Failed");
		return nullptr;
	}

	pInstance->Get_Collider()->SetSize(colSize);
	pInstance->Get_Collider()->SetColLayer(colLayer);
	pInstance->Get_Collider()->SetThisPointer(pInstance);

	pInstance->Get_Transform()->Set_Pos(initPos);

	pInstance->Set_RoomType(roomType);
	pInstance->Set_IntType(intType);
	pInstance->Set_TriggerType(triType);
	pInstance->Set_Count(count);

	return pInstance;
}

void InteractionTrigger::TriggerAction()
{
	if (m_eTriType & TRITYPE_BLOCK)
	{
		TriggerBlock();
	}

	if (m_eTriType & TRITYPE_SPAWN)
	{
		TriggerSpawn();
	}

	if (m_eTriType & TRITYPE_WARP)
	{
		TriggerWarp();
	}

	//if (m_eTriType & TRITYPE_SEAL) // 필요한가? 고민필요
	//{
	//	TriggerSeal();
	//}

	m_bIsTriggered = true;
}

void InteractionTrigger::TriggerBlock()
{
	// 여기에서 던전을 나가는 길목을 막는다
	// 아마 다른 이미지랑 로직 필요
	switch (m_eRoomType)
	{
	case ROOMTYPE_ARCHER:
	{
		GameObject* pGameObject = m_pLinkedInt = MovableWall::Create(m_pGraphicDev, L"Proto_MovableWall", _vec3(2.f, 2.f, 0.5f), COL_INTERACTION, _vec3(7.f, 0.f, 69.01f), INTDIR_UP, INTTYPE_WALL, WALLTYPE_WALL, TRITYPE_BLOCK);
		if (pGameObject == nullptr)
			return;

		m_pLinkedInt->Set_IntState(INTSTATE_IDLE);

		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom_BlockWall", pGameObject);
		pGameObject->Get_Transform()->Set_Scale(2.f, 2.f, 2.f);

		SoundMgr::GetInstance()->Play_Sound_NoInterval(L"door_stone_close", SOUND_INTERACTION, 1.f);
		SingleCamera::GetInstance()->PlayCameraShake(0.2f, 0.1f);

		m_eTriType -= TRITYPE_BLOCK;
	}
	break;
	// 왼
	case ROOMTYPE_SLIME:
	{
		

		m_eTriType -= TRITYPE_BLOCK;
	}
	break;
	// 오
	case ROOMTYPE_LADDER:
	{
		// 아마 옮겨야 될 코드
		GameObject* pGameObject = FloatingFloor::Create(m_pGraphicDev, L"Proto_FloatingFloor", _vec3(2.f, 1.f, 0.1f), COL_INTERACTION, _vec3(67.f, -4.f, 39.f), _vec3(67.f, 2.f, 39.f), INTTYPE_FLOATINGFLOOR);
		if (pGameObject == nullptr)
			return;

		Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"LadderRoom_FloatingFloor", pGameObject);
		pGameObject->Get_Transform()->Rotation(ROT_X, D3DXToRadian(90.f));
		pGameObject->Get_Transform()->Rotation(ROT_Y, D3DXToRadian(90.f));
		pGameObject->Get_Transform()->Set_Scale(2.f, 1.f, 1.f);

		// Ladder
		pGameObject = MovableWall::Create(m_pGraphicDev, L"Proto_Ladder", _vec3(1.f, 5.f, 0.1f), COL_INTERACTION, _vec3(57.1f, -3.f, 32.f), INTDIR_END, INTTYPE_WALL, WALLTYPE_LADDER, TRITYPE_BLOCK);
		if (pGameObject == nullptr)
			return;

		Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"LadderRoom_Ladder", pGameObject);
		pGameObject->Get_Transform()->Rotation(ROT_Y, D3DXToRadian(90.f));
		pGameObject->Get_Transform()->Set_Scale(1.f, 6.f, 1.f);
		
		SingleCamera::GetInstance()->PlayCameraShake(0.1f, 0.1f);
		SoundMgr::GetInstance()->Play_Sound_NoInterval(L"trap_activate", SOUND_INTERACTION, 1.f);

		m_eTriType -= TRITYPE_BLOCK;
	}
	break;

	case ROOMTYPE_CENTER:
	{
		GameObject* pGameObject = m_pSeal0 = MovableWall::Create(m_pGraphicDev, L"Proto_Seal0", _vec3(2.f, 5.f, 0.1f), COL_INTERACTION, _vec3(7.f, -1.f, 32.9f), INTDIR_LEFT, INTTYPE_WALL, WALLTYPE_SEAL0, TRITYPE_SEAL);
		if (pGameObject == nullptr)
			return;
		
		Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"CenterRoom_Seal0", pGameObject);
		pGameObject->Get_Transform()->Set_Scale(2.f, 1.f, 1.f);

		pGameObject = m_pSeal1 = MovableWall::Create(m_pGraphicDev, L"Proto_Seal1", _vec3(2.f, 5.f, 0.1f), COL_INTERACTION, _vec3(7.f, 1.f, 32.9f), INTDIR_RIGHT, INTTYPE_WALL, WALLTYPE_SEAL1, TRITYPE_SEAL);
		if (pGameObject == nullptr)
			return;

		Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"CenterRoom_Seal1", pGameObject);
		pGameObject->Get_Transform()->Set_Scale(2.f, 1.f, 1.f);

		pGameObject = m_pSeal2 = MovableWall::Create(m_pGraphicDev, L"Proto_Seal2", _vec3(2.f, 5.f, 0.1f), COL_INTERACTION, _vec3(7.f, 3.f, 32.9f), INTDIR_UP, INTTYPE_WALL, WALLTYPE_SEAL2, TRITYPE_SEAL);
		if (pGameObject == nullptr)
			return;

		Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"CenterRoom_Seal2", pGameObject);
		pGameObject->Get_Transform()->Set_Scale(2.f, 1.f, 1.f);

		SoundMgr::GetInstance()->Play_Sound_NoInterval(L"door_stone_close", SOUND_INTERACTION, 1.f);

		m_eTriType -= TRITYPE_BLOCK;
	}
	break;
	}

}

void InteractionTrigger::TriggerSpawn()
{
	switch (m_eRoomType)
	{
	case ROOMTYPE_ARCHER:
	{
		ArcherRoomSpawn();

		m_eTriType -= TRITYPE_SPAWN;
	}
	break;
	// 왼
	case ROOMTYPE_SLIME:
	{
		SlimeRoomSpawn();

		m_eTriType -= TRITYPE_SPAWN;
	}
	break;
	// 오
	case ROOMTYPE_LADDER:
	{
		LadderRoomSpawn();

		m_eTriType -= TRITYPE_SPAWN;
	}
	break;
	}

}

void InteractionTrigger::TriggerSeal()
{
	switch (m_eRoomType)
	{
	case ROOMTYPE_ARCHER:
	{
		if (m_pPlayer)
		{
			if (m_iCount == m_pPlayer->GetKillCount())
			{
				if (m_pLinkedInt)
					m_pLinkedInt->Set_IntState(INTSTATE_MOVE);

				m_pPlayer->ResetKillCount();

				m_eTriType -= TRITYPE_SEAL;
			}
		}
	}
	break;

	case ROOMTYPE_SLIME:
	{
		if (m_pPlayer)
		{
			if (m_iCount == m_pPlayer->GetKillCount())
			{
				m_pPlayer->Add_SealCount();

				m_pPlayer->ResetKillCount();

				m_eTriType -= TRITYPE_SEAL;
			}
		}
	}
	break;

	case ROOMTYPE_LADDER:
	{
		if (m_pPlayer)
		{
			if (Engine::Get_DIKeyState(DIK_E) & 0x80)
			{
				m_pPlayer->Add_SealCount();

				m_pPlayer->ResetKillCount();

				SoundMgr::GetInstance()->Play_Sound_NoInterval(L"wood-door-open", SOUND_INTERACTION, 1.f);

				m_eTriType -= TRITYPE_SEAL;
			}
		}
	}
	break;

	case ROOMTYPE_CENTER:
	{
		if (Engine::Key_Down('E'))
		{
			if (m_pPlayer)
			{
				if (m_pPlayer->Get_SealCount() >= 3)
				{
					SoundMgr::GetInstance()->Play_Sound_NoInterval(L"door_beginning", SOUND_MISC, 1.f);

					m_pSeal0->Set_IntState(INTSTATE_MOVE);
					m_pSeal1->Set_IntState(INTSTATE_MOVE);
					m_pSeal2->Set_IntState(INTSTATE_MOVE);

					SingleCamera::GetInstance()->PlayCameraShake(0.3f, 0.2f);

					m_eTriType -= TRITYPE_SEAL;
				}
			}
		}
	}
	break;
	}
}

void InteractionTrigger::TriggerWarp()
{
	switch (m_eRoomType)
	{
	case ROOMTYPE_CENTER:
	{
		MainApp::SetIsTriggered(true);
		MainApp::SetStageType(STAGE_2);

		m_bIsWarped = true;

		m_eTriType -= TRITYPE_WARP;
	}
	break;
	case ROOMTYPE_STAGE2:
	{
		MainApp::SetIsTriggered(true);
		MainApp::SetStageType(STAGE_BOSS);

		m_bIsWarped = true;

		m_eTriType -= TRITYPE_WARP;
	}
	break;
	case ROOMTYPE_VILLAGE:
	{
		MainApp::SetIsTriggered(true);
		MainApp::SetStageType(STAGE_1);

		m_bIsWarped = true;

		m_eTriType -= TRITYPE_WARP;
	}
	break;
	}
}

void InteractionTrigger::ArcherRoomSpawn()
{
	SpawnMarker* spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(13.f, 2.f, 78.f), MI_WIZARD, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner0", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(13.f, 2.f, 81.f), MI_DRUID, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner1", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(13.f, 2.f, 84.f), MI_BAT, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner2", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(13.f, 2.f, 93.f), MI_MONK, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner3", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(11.f, 2.f, 95.f), MI_DRUID, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner4", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(5.f, 2.f, 95.5f), MI_WIZARD, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner5", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(2.5f, 2.f, 93.f), MI_MONK, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner6", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(3.f, 2.f, 87.f), MI_WIZARD, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner7", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(3.f, 2.f, 83.f), MI_DRUID, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner8", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(3.f, 2.f, 79.f), MI_WIZARD, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner9", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(18.f, 2.f, 106.f), MI_MONK, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner10", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(12.5f, 2.f, 106.f), MI_BAT, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner11", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(7.f, 2.f, 106.f), MI_DRUID, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner12", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(1.5f, 2.f, 106.f), MI_BAT, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner13", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(-4.f, 2.f, 106.f), MI_WIZARD, SPAWNMARKER_ARCHER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"ArcherRoom Spawner14", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}
}

void InteractionTrigger::SlimeRoomSpawn()
{
	SpawnMarker* spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(-52.f, -40.f, 34.f), MI_SLIME, SPAWNMARKER_SLIME, SPAWNMARKER_ACTIVATE);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SlimeRoom_Spawner0", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(-42.f, -40.f, 32.f), MI_SLIME, SPAWNMARKER_SLIME, SPAWNMARKER_ACTIVATE);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SlimeRoom_Spawner1", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(-40.f, -40.f, 45.f), MI_SLIME, SPAWNMARKER_SLIME, SPAWNMARKER_ACTIVATE);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"SlimeRoom_Spawner2", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}
}

// LadderRoom 꼭 수정해라 몬스터 종류
void InteractionTrigger::LadderRoomSpawn()
{
	SpawnMarker* spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(62.f, -6.f, 36.f), MI_WARRIOR, SPAWNMARKER_LADDER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"LadderRoom_Spawner0", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(62.f, -6.f, 28.f), MI_ZOMBI, SPAWNMARKER_LADDER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"LadderRoom_Spawner1", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(50.f, 4.f, 38.f), MI_WIZARD, SPAWNMARKER_LADDER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"LadderRoom_Spawner2", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(78.f, 4.f, 40.f), MI_WIZARD, SPAWNMARKER_LADDER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"LadderRoom_Spawner3", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(72.f, 4.f, 30.f), MI_WIZARD, SPAWNMARKER_LADDER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"LadderRoom_Spawner4", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(72.f, 4.f, 38.f), MI_MONK, SPAWNMARKER_LADDER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"LadderRoom_Spawner5", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}

	// 아이템??
	spawnMarker = SpawnMarker::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(78.f, 4.f, 28.f), MI_DRUID, SPAWNMARKER_LADDER, SPAWNMARKER_SPAWN);
	if (spawnMarker)
	{
		::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"LadderRoom_Spawner6", spawnMarker);
		m_vecSpawnMarkers.push_back(spawnMarker);
	}
}

void InteractionTrigger::OnCollided()
{
	if (m_pColliderCom->GetColSphere() & COL_PLAYER)
	{
		list<pair<CINFO, _vec3>> playerList = m_pColliderCom->GetColSphere_Info(COL_PLAYER);
		for (auto& it : playerList)
		{
			Player* player = static_cast<Player*>(it.first.m_pThis);
			if (player)
			{
				m_pPlayer = player;
				m_iSealCount = player->Get_SealCount();

				if (m_eRoomType == ROOMTYPE_LADDER)
				{
					if (m_eTriType & TRITYPE_BLOCK)
					{
						if (Engine::Get_DIKeyState(DIK_E) & 0x80)
						{
							m_eIntState = INTSTATE_TRIGGER;

							return;
						}
					}
					
					else if (m_eTriType & TRITYPE_SPAWN)
					{
						m_eIntState = INTSTATE_TRIGGER;

						return;
					}
				}
				else
				{
					m_eIntState = INTSTATE_TRIGGER;

					return;
				}
			}
		}
	}
}

HRESULT InteractionTrigger::Add_Component(const wstring& textureKey)
{
	

	return S_OK;
}

void InteractionTrigger::Free()
{
	m_pLinkedInt = nullptr;
	for (auto& it : m_vecSpawnMarkers)
	{
		it = nullptr;
	}
	m_vecSpawnMarkers.clear();
	m_vecSpawnMarkers.shrink_to_fit();

	Super::Free();
}
