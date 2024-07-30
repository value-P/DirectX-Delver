#pragma once

#include "Armor.h"

class LeatherHead : public Armor
{
	using Super = Armor;

private:
	explicit LeatherHead(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit LeatherHead(const LeatherHead& rhs);
	virtual ~LeatherHead();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	virtual Texture* GetTexture() override { return m_pTextureCom; }

private:
	HRESULT			Add_Component();

public:
	static LeatherHead* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ItemBase* Clone() override;

private:
	virtual void Free() override;

private:
	Texture* m_pTextureCom = nullptr;
};
