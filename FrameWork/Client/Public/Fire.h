#pragma once

#include "EnvironmentBase.h"

namespace Engine
{
	class AnimationTexture;
}

class Fire : public EnvironmentBase
{
private:
	explicit Fire(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~Fire();

public:
	static Fire* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3& _vPos);

public:
	HRESULT Init_Fire(const D3DXVECTOR3& _vPos);

public:
	virtual _int Update_GameObject(const float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT	Add_Component();

private:
	AnimationTexture* m_pAniTexCom{ nullptr };

private:
	virtual void Free();
};

