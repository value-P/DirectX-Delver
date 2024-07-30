#pragma once

#include "Effect.h"
#include "MagicEx.h"

namespace Engine
{
	class AnimationTexture;
	class Collider;
	class PSystem;
}

enum MAGIC
{
	MAGIC_0,
	MAGIC_1,

	MAGIC_END
};

class Magic : public Effect
{
private:
	explicit Magic(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~Magic();

public:
	static Magic* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const MAGIC& _eMAGIC, const MAGIC_EX& _eMAGIC_EX, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const float& _fSpeed, const int& _iDamage = 1, const D3DCOLOR& _dwColor = { 0xFFFFFFFF });

public:
	HRESULT Init_Magic(const MAGIC& _eMAGIC, const MAGIC_EX& _eMAGIC_EX, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const float& _fSpeed, const int& _iDamage, const D3DCOLOR& _dwColor);

public:
	virtual int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

public:
	virtual D3DCOLOR GetColor() { return m_dwColor; }

	// Added By Gwan
	int GetDamage() { return m_iDamage + m_iEXDamage; }
	virtual void SetColType(COLLAYER _eType);

private:
	HRESULT	Add_Component();
	void	CreateMagicEx();
	void	SettingMagic();

private:
	AnimationTexture* m_pAniTexCom{ nullptr };
	Collider* m_pColliderCom{ nullptr };

	PSystem* m_pParticles{ nullptr };
	float m_fParticleGenerateTime{ 0.0f };

	MAGIC		m_eType{ MAGIC_END };
	MAGIC_EX	m_eEXType{ MAGIC_EX_END };
	D3DXVECTOR3 m_vDir{ 0.0f,0.0f,0.0f };
	float m_fSpeed{ 0.0f };
	int m_iDamage{ 1 };
	int m_iEXDamage{ 5 };
	D3DCOLOR m_dwColor{ 0xFFFFFFFF };

private:
	virtual void Free();
};

