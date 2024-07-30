#pragma once

#include "Effect.h"

namespace Engine
{
	class Texture;
	class Collider;
}

enum FRAG
{
	FRAG_BOX,
	FRAG_CERAMIC,
	FRAG_ARROW,
	FRAG_END
};

class Fragment : public Effect
{
private:
	explicit Fragment(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~Fragment();

public:
	static Fragment* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const FRAG& _eType);

public:
	HRESULT Init_Fragment(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const FRAG& _eType);

public:
	virtual int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT	Add_Component();
	void PlaneCollision(bool* _bOnTerrain = nullptr, bool* _bOnWall = nullptr);

private:
	Collider* m_pColliderCom{ nullptr };

	D3DXVECTOR3 m_vMoveDir{ 0.0f,0.0f,0.0f };
	UINT	m_iTexNum{ 0 };
	bool	m_bOnTerrain{ false };
	bool	m_bOnWall{ false };
	float	m_fTime{ 0.0f };

private:
	virtual void	Free();
};

