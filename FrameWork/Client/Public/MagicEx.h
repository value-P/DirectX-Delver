#pragma once

#include "Effect.h"

namespace Engine
{
	class AnimationTexture;
	class Collider;
}

enum MAGIC_EX
{
	MAGIC_EX_0,
	MAGIC_EX_1,
	MAGIC_EX_2,

	MAGIC_EX_END
};

class MagicEx : public Effect
{
private:
	explicit MagicEx(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~MagicEx();

public:
	static MagicEx* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const MAGIC_EX& _eMAGIC_EX, const D3DXVECTOR3& _vPos, const int& _iDamage = 1, const D3DCOLOR& _tColor = { 0xFFFFFFFF });

public:
	HRESULT Init_MagicEx(const MAGIC_EX& _eMAGIC_EX, const D3DXVECTOR3& _vPos, const int& _iDamage, const D3DCOLOR& _tColor);

public:
	virtual int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

public:
	virtual D3DCOLOR GetColor() { return m_dwColor; }

private:
	HRESULT	Add_Component();
	void	SettingMagicEX();

private:
	AnimationTexture* m_pAniTexCom{ nullptr };
	Collider* m_pColliderCom{ nullptr };

	MAGIC_EX m_eEXType{ MAGIC_EX_END };
	D3DCOLOR m_dwColor{ 0xFFFFFFFF };
	bool m_bBoom{ false };
	int m_iDamage{ 5 };

private:
	virtual void	Free();

};

