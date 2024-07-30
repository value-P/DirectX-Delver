#pragma once
#include "GameObject.h"

namespace Engine
{
    class Collider;
}

class BossPortal :
    public GameObject
{
private:
	explicit BossPortal(LPDIRECT3DDEVICE9 _pGraphicDevice, Transform* pPlayer, _vec3* pPos);
	virtual ~BossPortal();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;


private:
	HRESULT	Add_Component();

private:
	virtual void OnCollided() override;

private:
	Transform*		m_pPlayer;
	Collider*		m_pColliderCom = nullptr;

private:
	_bool			m_bActivated = false;
	_bool			m_bAlive = true;
	_float			m_fScaleY = 0.f;
	_vec3			m_StartPos;
private:
	virtual void Free();

public:
	static BossPortal* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, Transform* pPlayer, _vec3* pPos);

};

