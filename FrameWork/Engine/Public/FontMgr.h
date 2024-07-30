#pragma once

#include "Base.h"
#include "MyFont.h"

BEGIN(Engine)

class ENGINE_DLL FontMgr : public Base
{
	DECLARE_SINGLETON(FontMgr)

private:
	explicit FontMgr();
	virtual ~FontMgr();

public:
	HRESULT		Init_Font(LPDIRECT3DDEVICE9 pGraphicDev,
		const wstring pFontTag,
		const wstring pFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

	void		Render_Font(const wstring pFontTag,
		const wstring pString,
		const _vec4* pPos,
		DWORD _dwFormat,
		D3DXCOLOR Color);

private:
	map<const wstring, MyFont*>			m_mapFont;

private:
	MyFont* Find_Font(const wstring pFontTag);

public:
	virtual		void	Free();
};

END
