#pragma once

#include "GameObject.h"

namespace Engine
{
	class Collider;
}

class Monster;

enum SpawnMarkerRoom
{
	SPAWNMARKER_ARCHER,
	SPAWNMARKER_SLIME,
	SPAWNMARKER_LADDER,

	SPAWNMARKERROOM_END,
};

enum SpawnMarkerType
{
	SPAWNTYPE_MONSTER,
	SPAWNTYPE_ITEM,

	SPAWNTYPE_END,
};

enum SpawnMarkerState
{
	SPAWNMARKER_IDLE,
	SPAWNMARKER_SPAWN, // 한번 소환
	SPAWNMARKER_ACTIVATE, // 루프
	SPAWNMARKER_DEACTIVATE,

	SPAWNMARKER_END,
};

// Monster Spawn Pos 관리용
class SpawnMarker : public GameObject
{
	using Super = GameObject;

protected:
	explicit SpawnMarker(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit SpawnMarker(const SpawnMarker& rhs);
	virtual ~SpawnMarker();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	Collider* Get_Collider() { return m_pColliderCom; }

public:
	bool IsLoop() { return m_bIsLoop; }
	void Set_Loop(bool bIsLoop) { m_bIsLoop = bIsLoop; }

	SpawnMarkerState Get_State() { return m_eState; }
	void Set_State(SpawnMarkerState state) { m_eState = state; }

	SpawnMarkerType Get_Type() { return m_eType; }
	void Set_Type(SpawnMarkerType markerType) { m_eType = markerType; }

	MONSTER_ID Get_MonsterID() { return m_eMonsterID; }
	void Set_MonsterID(MONSTER_ID monsterID) { m_eMonsterID = monsterID; }

	SpawnMarkerRoom Get_MarkerRoom() { return m_eRoom; }
	void Set_MarkerRoom(SpawnMarkerRoom markerRoom) { m_eRoom = markerRoom; }

public:
	void SpawnMonster(); // 딱 한번만 소환
	void Activate(); // 활성화 -> 루프
	void DeActivate(); // 비활성화

public:
	static SpawnMarker* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 colSize, COLLAYER colLayer, _vec3& initPos, MONSTER_ID monsterID, SpawnMarkerRoom spawnMarkerRoom, SpawnMarkerState state = SPAWNMARKER_IDLE);

protected:
	HRESULT Add_Component();

protected:
	virtual void Free() override;

protected:
	Collider* m_pColliderCom { nullptr };

	bool m_bIsLoop { false }; // 필요 있나?
	Monster* m_pMonster { nullptr };

	SpawnMarkerRoom m_eRoom = SPAWNMARKERROOM_END;
	SpawnMarkerState m_eState = SPAWNMARKER_END;
	SpawnMarkerType m_eType = SPAWNTYPE_END;
	MONSTER_ID m_eMonsterID = MI_END;

	// Temp
	_int m_iSpawnCount = 0;
	float m_fDeltaTime = 0.f;
	float m_fSpawnTime { 0.f };
};
