#pragma once

#include "Effect.h"

namespace Engine
{
    class AnimationTexture;
}

enum EFFECT
{
	EFFECT_0,
	EFFECT_1,
	EFFECT_2,
	EFFECT_3,
	EFFECT_4,
	EFFECT_5,

	EFFECT_END
};

class Effect_Bundle : public Effect
{
private:
	explicit Effect_Bundle(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~Effect_Bundle();

public:
	static Effect_Bundle* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const EFFECT& _eEFFECT, const D3DXVECTOR3& _vPos);

public:
	HRESULT Init_Effect_Bundle(const EFFECT& _eType, const D3DXVECTOR3& _vPos);

public:
	virtual int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT	Add_Component();
	void	SettingEffect();

private:
	AnimationTexture* m_pAniTexCom{ nullptr };

	EFFECT	m_eType{ EFFECT_END };

private:
	virtual void	Free();
};

