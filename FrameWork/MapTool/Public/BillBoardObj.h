#pragma once
#include "GameObject.h"

namespace Engine
{
    class RcTex;
    class Texture;
    class Transform;
}

class BillBoardObj :
    public GameObject
{
private:
	explicit BillBoardObj(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vInitPos, _vec2 vInitScale, const wstring& szTexturePath, const wstring& szOriginName);
	explicit BillBoardObj(const BillBoardObj& rhs);
	virtual ~BillBoardObj();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component();

private:
	const wstring m_szTexturePath;
	_vec3 m_vInitPos;
	_vec2 m_vInitScale;


public:
	static BillBoardObj* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vInitPos, _vec2 vInitScale, const wstring& szTexturePath, const wstring& szOriginName);

private:
	virtual void	Free();
};

