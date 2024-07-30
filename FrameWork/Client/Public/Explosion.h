#pragma once

#include "Effect.h"

namespace Engine
{
    class AnimationTexture;
    class Collider;
}

class Explosion : public Effect
{
private:
	explicit Explosion(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~Explosion();

public:
	static Explosion* Create(LPDIRECT3DDEVICE9 _pGraphicDevice,const D3DXVECTOR3& _vPos);

public:
	HRESULT Init_Explosion(const D3DXVECTOR3& _vPos);

public:
	virtual int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT	Add_Component();

private:
	AnimationTexture* m_pAniTexCom{ nullptr };
	Collider* m_pColliderCom{ nullptr };

	bool m_bBoom{ false };

private:
	virtual void	Free();
};

