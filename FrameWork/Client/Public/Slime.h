#pragma once
#include "Monster.h"

namespace Engine
{
	class Texture;
}

class Slime : public Monster
{
	using Super = Monster;

public:
	explicit Slime(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Slime(const Slime& rhs);
	virtual ~Slime();

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
	static Slime* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free();

private:
	HRESULT Tracking(const _float& fTimeDelta);
};

