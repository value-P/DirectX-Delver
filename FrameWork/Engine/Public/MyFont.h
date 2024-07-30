#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL MyFont : public Base
{
private:
	explicit MyFont(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~MyFont();

public:
	HRESULT		Init_Font(const wstring pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
	void		Render_Font(const wstring pString, const _vec4* pPos, DWORD _dwFormat, D3DXCOLOR Color);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPD3DXSPRITE				m_pSprite;
	LPD3DXFONT					m_pFont;

public:
	static MyFont* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);

private:
	virtual void	Free();
};

END