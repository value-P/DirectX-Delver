#pragma once
#include "GameObject.h"

namespace Engine
{
    class RcTex;
    class Texture;
    class Transform;
}

class BillObj :
    public GameObject
{
private:
	explicit BillObj(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit BillObj(const BillObj& rhs);
	virtual ~BillObj();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(const wstring& texturePath, _vec2 vInitScale);

public:
	static BillObj* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& texturePath, _vec2 vInitScale);

private:
	virtual void	Free();
};

