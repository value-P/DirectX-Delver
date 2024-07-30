#pragma once

#include "InteractableBase.h"

enum RoomType
{
	ROOMTYPE_ARCHER,
	ROOMTYPE_SLIME,
	ROOMTYPE_LADDER,
	ROOMTYPE_CENTER,
	ROOMTYPE_STAGE2,
	ROOMTYPE_VILLAGE,

	ROOMTYPE_END,
};

class InteractableBase;
class SpawnMarker;
class MovableWall;
class Player;

class InteractionTrigger : public InteractableBase
{
	using Super = InteractableBase;

private:
	explicit InteractionTrigger(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit InteractionTrigger(const InteractionTrigger& rhs);
	virtual ~InteractionTrigger();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	InteractableBase* Get_LinkedInt() { return m_pLinkedInt; }

	RoomType Get_RoomType() { return m_eRoomType; }
	void Set_RoomType(RoomType roomType) { m_eRoomType = roomType; }

	bool IsTriggered() { return m_bIsTriggered; }
	void SetIsTriggered(bool isTriggered) { m_bIsTriggered = isTriggered; }

public:
	static InteractionTrigger* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 colSize, COLLAYER colLayer, _vec3& initPos, RoomType roomType, InteractableType intType, _uint triType, _uint count = 0);

private:
	void TriggerAction();
	void TriggerBlock();
	void TriggerSpawn();
	void TriggerSeal();
	void TriggerWarp();

	void ArcherRoomSpawn();
	void SlimeRoomSpawn();
	void LadderRoomSpawn();

private:
	virtual void OnCollided() override;

private:
	HRESULT Add_Component(const wstring& textureKey);

private:
	virtual void Free() override;

private:
	InteractableBase* m_pLinkedInt { nullptr };
	vector<SpawnMarker*> m_vecSpawnMarkers;

	// Temp
	float m_fDeltaTime { 0.f };
	bool m_bIsTriggered = false;
	bool m_bIsWarped = false;

	RoomType m_eRoomType = ROOMTYPE_END;

	MovableWall* m_pSeal0{ nullptr };
	MovableWall* m_pSeal1{ nullptr };
	MovableWall* m_pSeal2{ nullptr };

	_uint m_iSealCount { 0 };

	Player* m_pPlayer { nullptr };
};
