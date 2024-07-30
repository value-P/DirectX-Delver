#pragma once
#include "GameObject.h" 

BEGIN(Engine)

class RcTex;
class Texture;

END

class CBackGround : public GameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBackGround(const CBackGround& rhs);
	virtual ~CBackGround();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	
public:
	void Set_Is_FinishedLoading(bool _bIs_FinishedLoading) { m_bIs_FinishedLoading = _bIs_FinishedLoading; }

private:
	HRESULT			Add_Component();

private:
	RcTex*		m_pBufferCom = nullptr;
	Texture*	m_pTextureCom = nullptr;
	Texture*	m_pLogoTitle_TextureCom = nullptr;

public:
	static CBackGround*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free();

private:
	bool m_bIs_FinishedLoading = false;
};

