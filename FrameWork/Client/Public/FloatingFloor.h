#pragma once

#include "InteractableBase.h"

enum FloatingState
{
	FLOATING_IDLE,
	FLOATING_MOVEUP,
	FLOATING_MOVEDOWN,
	FLOATING_DESTROY,

	FLOATING_END,
};

class FloatingFloor : public InteractableBase
{
	using Super = InteractableBase;

private:
	explicit FloatingFloor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit FloatingFloor(const FloatingFloor& rhs);
	virtual ~FloatingFloor();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	_vec3& Get_StartPos() { return m_vStartPos; }
	void Set_StartPos(_vec3& startPos) { m_vStartPos = startPos; }

	_vec3& Get_EndPos() { return m_vEndPos; }
	void Set_EndPos(_vec3& endPos) { m_vEndPos = endPos; }

	FloatingState Get_State() { return m_eFloatState; }
	void Set_State(FloatingState floatState) { m_eFloatState = floatState; }

	bool IsOnFloor() { return m_bIsOnFloor; }
	void SetIsOnFloor(bool isOnFloor) { m_bIsOnFloor = isOnFloor; }

public:
	static FloatingFloor* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& textureKey, _vec3 colSize, COLLAYER colLayer, _vec3& startPos, _vec3& endPos, InteractableType intType, FloatingState floatingState = FLOATING_END);

private:
	void FFIdle();
	void FFMoveUp();
	void FFMoveDown();
	void FFDestroy();

private:
	virtual void OnCollided() override;

private:
	HRESULT Add_Component(const wstring& textureKey);

private:
	virtual void Free() override;

private:
	float m_fDeltaTime { 0.f };

	_vec3 m_vStartPos {};
	_vec3 m_vEndPos {};

	// Test
	float m_fDisTime { 0.f };
	FloatingState m_eFloatState = FLOATING_END;
	bool m_bIsOnFloor = false;
};
