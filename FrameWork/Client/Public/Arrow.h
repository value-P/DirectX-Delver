#pragma once

#include "Effect.h"

namespace Engine
{
	class AnimationTexture;
	class Collider;
	class PSystem;
}


class Arrow : public Effect
{
private:
	explicit Arrow(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~Arrow();

public:
	static Arrow* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const float& _fSpeed, const int& _iDamage = 1);

public:
	HRESULT Init_Arrow(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const float& _fSpeed, const int& _iDamage);

public:
	virtual int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

public:
	virtual D3DCOLOR GetColor() { return m_dwColor; }

	// Added By Gwan
	int GetDamage() { return m_iDamage; }
	virtual void SetColType(COLLAYER _eType);

private:
	HRESULT	Add_Component();

private:
	Collider* m_pColliderCom{ nullptr };

	PSystem* m_pParticles{ nullptr };
	float m_fParticleGenerateTime{ 0.0f };


	D3DXMATRIX m_matCrossWorld;
	D3DXVECTOR3 m_vDir{ 0.0f,0.0f,0.0f };
	float m_fSpeed{ 0.0f };
	int m_iDamage{ 1 };
	D3DCOLOR m_dwColor{ 0xFFFFFFFF };

private:
	virtual void Free();
};

