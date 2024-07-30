#pragma once

#include "Creature.h"

BEGIN(Engine)

class Texture;
class Light;

END

class ItemBase;
class InventoryBase;
class QuickSlot;
class Rope;
class EquipmentSlot;
class InteractableBase;
class InteractionTrigger;

enum PlayerState
{
	PLAYER_IDLE = 1 << 0,
	PLAYER_MOVE = 1 << 1,
	PLAYER_JUMP = 1 << 2,
	PLAYER_WEAKATTACK = 1 << 3,
	PLAYER_SHOOTROPE = 1 << 4,
	PLAYER_RIDEROPE = 1 << 5,
	PLAYER_STRONGATTACK = 1 << 6,

	PLAYERSTATEEND = 7,
};

class Player : public Creature
{
	using Super = Creature;

private:
	explicit Player(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Player(const Player& rhs);
	virtual ~Player();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void PlayerDefaultSetting();

public:
	InventoryBase* Get_Inventory() { return m_pInv; }

	bool IsUIOpened() { return m_bIsUIOpened; }
	void SetIsUIOpened(bool isUIOpened) { m_bIsUIOpened = isUIOpened; }

	bool IsUIPicking() { return m_bIsUIPicking; }
	void SetIsUIPicking(bool isUIPicking) { m_bIsUIPicking = isUIPicking; }

	UINT Get_PlayerState() { return m_iPlayerState; }
	void Add_PlayerState(UINT playerState) { m_iPlayerState |= playerState; }

	bool IsOnTrack() { return m_bIsOnTrack; }
	void SetIsOnTrack(bool isOnTrack) { m_bIsOnTrack = isOnTrack; }

	_int Get_SealCount() { return m_iSealCount; }
	void Add_SealCount() { m_iSealCount++; }
	void Set_SealCount(_int sealCount) { m_iSealCount = sealCount; }

	int GetKillCount() { return m_iKillCount; }
	void AddKillCount() { m_iKillCount++; }
	void ResetKillCount() { m_iKillCount = 0; }

	void Set_Is_closed_LevelUpBG(bool _bIs_closed_LevelUpBG) { m_bIs_closed_LevelUpBG = _bIs_closed_LevelUpBG; }

private:
	HRESULT			Add_Component(wstring textureKey);
	void			Key_Input(const _float& fTimeDelta);
	void			Mouse_Move();
	void			Mouse_Fix();

	void			Throw_Rope();

	void			UI_Picking();
private:
	void PlayerAction();
	void PlayerRideRope();
	void PlayerShootRope();

	void UpdateStat();

	void LevelUp();

public:
	static Player* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey);

protected:
	// 충돌 견본 함수
	// 다른 클래스에 충돌 필요할 경우 비슷하게 만들 것
	virtual void OnCollided() override;

private:
	virtual void	Free();

private:
	InventoryBase* m_pInv = nullptr;
	QuickSlot* m_pQuick = nullptr;
	EquipmentSlot* m_pEquip = nullptr;
	
	//vector<ItemBase*> m_vecItem; // HUD의 슬롯과 연동될 아이템들

	// RopeAction
	Rope*	m_pRope = nullptr;
	_vec3	m_vRopeTargetPos;
	_float	m_fRopeLength = 0.f;

	UINT m_iPlayerState{ 1 };

	_bool	m_bFix = false;
	_bool	m_bCheck = false;

	_float  m_fDebuffSpeed = 0.f;
	_float	m_fRealTimeDelta = 0.f;
	_float	m_fDeltaTime{ 0.2f };
	_float	FallSpeed{ 0.0f };
	bool	m_bOnTerrain{ false };

	bool	m_bAttack{ false };
	float	m_fAttackDelayTime{ 0.0f };

	float m_fFrame = 0.f;

	bool m_bPlayerHittable{ true };
	float m_fHittableDelay{ 0.0f };
	int m_iPastFrameHP{ 100 };
	bool m_bPlayerShakable{ true };
	float m_fShakableDelay{ 0.0f };

	bool m_bIsUIOpened = false;
	bool m_bIsPressed = false; // 나중에 수정할 예정
	bool m_bIsPressed1 = false; // 나중에 수정할 예정 22
	bool m_bIsPressed2 = false; // 나중에 수정할 예정 33
	bool m_bIsPressed3 = false; // 나중에 수정할 예정 44
	bool m_bIsPressed4 = false; // 나중에 수정할 예정 55
	bool m_bIsPressed5 = false; // 나중에 수정할 예정 66
	bool m_bIsPressed6 = false; // 나중에 수정할 예정 77
	bool m_bIsPressed7 = false; // 나중에 수정 안함 이제

	bool m_bIsPressed8 = false;

	// Interaction
	InteractionTrigger* m_pCurTriggered { nullptr };
	bool m_bIsOnTrack = false;
	_int m_iSealCount = 0;
	int m_iKillCount = 0;

	bool m_bIsUIPicking = false; // UI Picking Test용도
	bool m_bmouseSlotFull = false;

	bool m_bMouse_Lock = false;
	bool Popup_Font = false; // [우채] 테스트 용도 추후 삭제!

	bool m_bIs_closed_LevelUpBG = false;
};
