#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CubeTex;
class Texture;
class Transform;

END

class Cube :
    public GameObject
{
private:
	explicit Cube(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitPos, const wstring& szTexPath, const wstring& szOriginName);
	explicit Cube(const Cube& rhs);
	virtual ~Cube();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	wstring Get_OriginName() { return m_szOriginName; }

private:
	HRESULT			Add_Component();

private:
	wstring m_wsTexPath;

	_vec3 m_vInitPos;

public:
	static Cube* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitPos, const wstring& szTexPath, const wstring& szOriginName);

private:
	virtual void	Free();
};

