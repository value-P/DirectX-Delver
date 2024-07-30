#pragma once
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class VIBuffer;
class Transform;
class Texture;

class ENGINE_DLL GameObject : public Base
{
protected:
	explicit GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit GameObject(const GameObject& rhs);
	virtual ~GameObject();

public:
	Component*			Get_Component(const wstring pComponentTag, COMPONENTID eID);
 			Transform*			Get_Transform()		{ return m_pTransformCom;}					// 트랜스폼 컴포넌트를 받아오는 함수
	VIBuffer*			Get_Buffer()		{ return m_pBufferCom;}						// 각자의 버퍼 컴포넌트를 받아오는 함수
	Texture*			Get_Texture()		{ return m_pTextureCom; }					// Texture를 받아오는 함수
	_float				Get_ViewZ()			{ return m_fViewZ; }
	void				Compute_ViewZ(const _vec3* pPos);

public:
	virtual HRESULT			Init_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual void			LateUpdate_GameObject();
	virtual void			Render_GameObject()							{}

	virtual wstring Get_OriginName()		{ return m_szOriginName; }

protected:
	virtual void OnCollided() {}

private:	
	Component*			Find_Component(const wstring pComponentTag, COMPONENTID eID);

protected:
	map<const wstring, Component*>		m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9					m_pGraphicDev;

	Transform*							m_pTransformCom = nullptr;
	VIBuffer*							m_pBufferCom = nullptr;
	Texture*							m_pTextureCom = nullptr;

	_float								m_fViewZ;

	// 맵툴때문에 추가
	wstring m_szOriginName;

public:
	virtual void		Free();
};

END

