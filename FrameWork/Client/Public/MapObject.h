#pragma once
#include "GameObject.h"

class MapObject :
    public GameObject
{
protected:
    explicit MapObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit MapObject(const MapObject& rhs);
    virtual  ~MapObject();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(const wstring& texturePath, _vec2 vInitScale, PREFABTYPE eType);

public:
	static MapObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& texturePath, _vec2 vInitScale, PREFABTYPE eType);

private:
	virtual void	Free();

};

