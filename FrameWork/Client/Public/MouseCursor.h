#pragma once

#include "GameObject.h"

namespace Engine
{
	class Texture;
}

class ItemBase;
class MouseSlot;

class MouseCursor : public GameObject
{
	using Super = GameObject;

protected:
	explicit MouseCursor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit MouseCursor(const MouseCursor& rhs);
	virtual ~MouseCursor();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	static MouseCursor* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT			Add_Component();
	void			UpdatePos();
	virtual void OnCollided() override;

private:
	virtual void Free() override;

protected:
	Texture* m_pShowTextureCom = nullptr;
	Texture* m_pEmptyCursorTextureCom = nullptr;
	// MouseCursor
	MouseSlot* m_pMouseSlot = nullptr;

	bool m_bIsShowCursor = true;
	bool m_bIs_GetItem = false;
};
