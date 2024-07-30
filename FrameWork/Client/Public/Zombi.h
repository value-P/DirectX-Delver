#pragma once
#include "Monster.h"
namespace Engine
{
	class Texture;
}

class Zombi :public Monster
{
	using Super = Monster;

public:
	explicit Zombi(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Zombi(const Zombi& rhs);
	virtual ~Zombi();

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
	static Zombi* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free();

private:
	HRESULT Tracking(const _float& fTimeDelta);
};

