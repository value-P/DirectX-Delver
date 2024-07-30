#pragma once

#include "GameObject.h"

namespace Engine
{
	class Collider;
}

class EnvironmentBase : public GameObject
{
	using Super = GameObject;

protected:
	explicit EnvironmentBase(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit EnvironmentBase(const EnvironmentBase& rhs);
	virtual ~EnvironmentBase();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	const wstring& Get_TextureKey() { return m_textureKey; }
	void Set_TextureKey(const wstring& textureKey) { m_textureKey = textureKey; }

	Collider* Get_Collider() { return m_pColliderCom; }

protected:
	HRESULT			Add_Component(wstring textureKey);
	virtual void OnCollided() override;
	void ComputeBillBoard();	// SetTransform 들어있어서 렌더 해주기 전에 해주면됨

public:
	static EnvironmentBase* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer);

protected:
	virtual void Free();

protected:
	Collider* m_pColliderCom{ nullptr };

	wstring m_textureKey;
};
