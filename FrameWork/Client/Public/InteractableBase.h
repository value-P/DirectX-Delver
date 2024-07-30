#pragma once

#include "GameObject.h"

// 움직일 방향
enum InteractableDir
{
	INTDIR_RIGHT,
	INTDIR_UP,
	INTDIR_DOWN,
	INTDIR_LEFT,
	INTDIR_FRONT,
	INTDIR_BACK,

	INTDIR_END,
};

// FSM, 이게 뭔 코드냐...
enum InteractableState
{
	INTSTATE_IDLE,
	INTSTATE_MOVE,
	INTSTATE_AUTOMOVE, // AutoTrack 전용
	INTSTATE_TRIGGER, // InteractionTrigger 전용
	INTSTATE_DESTROY,
	INTSTATE_MOVEUP, // FloatingFloor 전용
	INTSTATE_MOVEDOWN, // FloatingFloor 전용
	
	INTSTATE_END,
};

// 상호작용 했을 때 어떠한 용도인지
enum InteractableType
{
	INTTYPE_WALL,
	INTTYPE_DOOR,
	INTTYPE_EVENT,
	INTTYPE_AUTOMOVE,
	INTTYPE_FLOATINGFLOOR,

	INTTYPE_END,
};

// 이벤트 발생했을 때 어떤 이벤트인지
enum TriggerType : _uint
{
	TRITYPE_WALL = 1 << 0,
	TRITYPE_BLOCK = 1 << 1,
	TRITYPE_SPAWN = 1 << 2,
	TRITYPE_SEAL = 1 << 3,
	TRITYPE_WARP = 1 << 4,

	TRITYPE_END,
};

namespace Engine
{
	class Collider;
}

class InteractableBase : public GameObject
{
	using Super = GameObject;

protected:
	explicit InteractableBase(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit InteractableBase(const InteractableBase& rhs);
	virtual ~InteractableBase();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	Collider* Get_Collider() { return m_pColliderCom; }
	Collider* Get_WallCollider() { return m_pColliderWallCom; }

	wstring Get_TextureKey() { return m_textureKey; }
	void Set_TextureKey(const wstring& textureKey) { m_textureKey = textureKey; }

	InteractableDir Get_IntDir() { return m_eIntDir; }
	void Set_IntDir(InteractableDir interactableDir) { m_eIntDir = interactableDir; }

	InteractableState Get_IntState() { return m_eIntState; }
	void Set_IntState(InteractableState interactableState) { m_eIntState = interactableState; }

	InteractableType Get_IntType() { return m_eIntType; }
	void Set_IntType(InteractableType intType) { m_eIntType = intType; }

	_uint Get_TriggerType() { return m_eTriType; }
	void Set_TriggerType(_uint triggerType) { m_eTriType |= triggerType; }

	_uint Get_Count() { return m_iCount; }
	void Set_Count(_uint count) { m_iCount = count; }

	// AutoTrack
	_vec3 Get_StartPos() { return m_vStartPos; }
	void Set_StartPos(_vec3& startPos) { m_vStartPos = startPos; }

protected:
	HRESULT Add_Component();

protected:
	virtual void Free();

protected:
	Collider* m_pColliderCom{ nullptr };
	Collider* m_pColliderWallCom { nullptr };
	
	wstring m_textureKey;

	InteractableDir m_eIntDir = INTDIR_END;
	InteractableState m_eIntState = INTSTATE_END;
	InteractableType m_eIntType = INTTYPE_END;
	_uint m_eTriType = 0;

	_uint m_iCount = 0; // 이벤트 발생 조건

	// Temp
	_vec3 m_vStartPos {};
};
