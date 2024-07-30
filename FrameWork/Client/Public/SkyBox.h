#pragma once
#include "GameObject.h" 

BEGIN(Engine)

class CubeTex;
class Texture;
class Transform;
class Calculator;

END

class CSkyBox : public GameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component();

private:
	CubeTex*			m_pBufferCom = nullptr;
	Texture*		m_pTextureCom = nullptr;
	Transform*		m_pTransformCom = nullptr;

public:
	static CSkyBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free();



};


