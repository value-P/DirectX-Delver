#pragma once
#include "BossSkillObject.h"
class ToTemBomb :
    public BossSkillObject
{
private:
	explicit ToTemBomb(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos);
	virtual ~ToTemBomb();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void	Bomb();
	void	Destroy_Bomb();

protected:
	HRESULT	Add_Component();

private:
	_float InitScale = 0.2f;

public:
	static ToTemBomb* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos);

private:
	virtual void	Free();
};

