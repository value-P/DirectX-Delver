#pragma once

#include "GameObject.h"

namespace Engine
{
	class Collider;
}

class MoveWall : public GameObject
{
private:
	explicit MoveWall(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~MoveWall();

public:
	static MoveWall* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, bool _bAuto, bool _bRepeat, const wstring _wstrTag, const D3DXVECTOR3& _vPosBegin, const D3DXVECTOR3& _vPosEnd, const D3DXVECTOR3& _vSize, const float& _fSpeed, const float& _fStopTime, const D3DXVECTOR3& _vRot = { 0.0f,0.0f,0.f });

public:
	HRESULT Init_MoveWall(bool _bAuto, bool _bRepeat, const wstring _wstrTag, const D3DXVECTOR3& _vPosBegin, const D3DXVECTOR3& _vPosEnd, const D3DXVECTOR3& _vSize, const float& _fSpeed, const float& _fStopTime, const D3DXVECTOR3& _vRot);

public:
	virtual int Update_GameObject(const float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

public:		// Get Function
	bool GetMove() { return m_bMove; }


public:		// Set Function
	void SetMove();
	void SetLinkedWall(MoveWall* _pMoveWall);

private:
	HRESULT	Add_Component(const wstring _wstrTag);
	void Move(const float& _fTimeDelta);


private:
	Collider* m_pCollider{ nullptr };

	bool m_bAuto{ false };
	bool m_bRepeat{ false };
	bool m_bGoEnd{ true };
	bool m_bMove{ true };
	bool m_bOnce{ true };
	D3DXVECTOR3 m_vBegin{ 0.0f,0.0f,0.0f };
	D3DXVECTOR3 m_vEnd{ 0.0f,0.0f,0.0f };
	D3DXVECTOR3 m_vDir{ 0.0f,0.0f,0.0f };
	float m_fSpeed{ 0.0f };
	float m_fStopTime{ 0.0f };
	float m_fDeltaTime{ 0.0f };

	MoveWall* m_pLinkedWall{ nullptr };

private:
	virtual void Free();
};

