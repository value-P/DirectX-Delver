#pragma once
#include "GameObject.h"


class Rope :
    public GameObject
{
private:
	explicit Rope(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Rope(const Rope& rhs);
	virtual ~Rope();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void	Rope_Update(_vec3* pStart, _vec3* pEnd, _float fThickness);

protected:
	HRESULT	Add_Component();

private:
	VTXTEX m_tVertices[6];
	_vec3  m_vPoints[4];
	_vec3  m_vStartPos;
	_vec3  m_vEndPos;
	_float m_fThickness = 0.f;

public:
	static Rope* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free();


};

