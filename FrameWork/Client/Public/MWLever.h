#pragma once

#include "GameObject.h"

namespace Engine
{
	class Collider;
}

class MoveWall;

class MWLever : public GameObject
{
private:
	explicit MWLever(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~MWLever();

public:
	static MWLever* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vPullDir, const D3DXVECTOR3& _vUp, MoveWall* _pMoveWall = nullptr, const D3DXVECTOR3& _vSize = { 0.5f,0.3f,0.7f });

public:
	HRESULT Init_MWLever(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vPullDir, const D3DXVECTOR3& _vUp, MoveWall* _pMoveWall, const D3DXVECTOR3& _vSize);

public:
	virtual int Update_GameObject(const float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT	Add_Component();
	void WorkingLever();

private:
	Transform* m_pLeverTrans{ nullptr };
	Texture* m_pLeverTex{ nullptr };
	Collider* m_pCollider{ nullptr };

	MoveWall* m_pMoveWall{ nullptr };

	D3DXVECTOR3 m_vAxis{ 0.0f,0.0f,0.0f };
	float m_fRadian{ 0.0f };
	float m_fOmega{ 0.0f };
	bool m_bUsable{ true };
	bool m_bActivating{ false };

private:
	virtual void Free();
};

