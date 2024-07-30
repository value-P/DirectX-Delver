#pragma once

#include "InteractableBase.h"

enum MovableWallType
{
	WALLTYPE_WALL,
	WALLTYPE_LADDER,
	WALLTYPE_SEAL0,
	WALLTYPE_SEAL1,
	WALLTYPE_SEAL2,

	WALLTYPE_END,
};

class Player;

class MovableWall : public InteractableBase
{
	using Super = InteractableBase;

private:
	explicit MovableWall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit MovableWall(const MovableWall& rhs);
	virtual ~MovableWall();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void Set_DesiredPos(_vec3& desiredPos);
	void Set_DesiredPos(InteractableDir intDir);

	void Set_WallType(MovableWallType wallType) { m_eWallType = wallType; }

public:
	static MovableWall* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, _vec3& initPos, InteractableDir intDir, InteractableType intType, MovableWallType wallType, TriggerType triType = TRITYPE_WALL);

private:
	virtual void OnCollided() override;

private:
	void MovableWallIdle();
	void MovableWallMove();

private:
	HRESULT Add_Component(const wstring& textureKey);

private:
	virtual void Free() override;

private:
	_vec3 m_vDesiredPos {};

	// Temp
	float m_fDeltaTime { 0.f };
	bool m_bIsOpened = false;

	MovableWallType m_eWallType = WALLTYPE_WALL;

	Player* pPlayer { nullptr };
};
