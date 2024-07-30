#pragma once
#include "GameObject.h"

namespace Engine
{
    class Collider;
}

class DebuffCol :
    public GameObject
{
private:
	explicit DebuffCol(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~DebuffCol();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

private:
	HRESULT	Add_Component();

private:
	Collider* m_pColliderCom = nullptr;

private:
	virtual void Free();

public:
	static DebuffCol* Create(LPDIRECT3DDEVICE9 _pGraphicDevice);

};

