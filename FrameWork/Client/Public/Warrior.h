#pragma once
#include "Monster.h"
namespace Engine
{
	class Texture;
}

class Warrior : public Monster
{
	using Super = Monster;


public:
	explicit Warrior(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Warrior(const Warrior& rhs);
	virtual ~Warrior();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component();

protected:
	virtual void OnCollided() override;
	virtual void OnDamaged() override;

public:
	static Warrior* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free();

private:
	HRESULT Tracking(const _float& fTimeDelta);
};