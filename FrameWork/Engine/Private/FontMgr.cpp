#include "..\..\Header\FontMgr.h"

IMPLEMENT_SINGLETON(FontMgr)

FontMgr::FontMgr()
{
}

FontMgr::~FontMgr()
{
	Free();
}

HRESULT FontMgr::Init_Font(LPDIRECT3DDEVICE9 pGraphicDev, const wstring pFontTag, const wstring pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	MyFont* pFont = nullptr;

	pFont = Find_Font(pFontTag);

	if (nullptr != pFont)
		return E_FAIL;

	pFont = MyFont::Create(pGraphicDev, pFontType, iWidth, iHeight, iWeight);
	NULL_CHECK_RETURN(pFont, E_FAIL);

	m_mapFont.insert({ pFontTag, pFont });

	return S_OK;
}

void FontMgr::Render_Font(const wstring pFontTag, const wstring pString, const _vec4* pPos, DWORD _dwFormat, D3DXCOLOR Color)
{
	MyFont* pMyFont = Find_Font(pFontTag);
	NULL_CHECK(pMyFont);

	pMyFont->Render_Font(pString, pPos, _dwFormat, Color);
}

MyFont* FontMgr::Find_Font(const wstring pFontTag)
{
	auto		iter = m_mapFont.find(pFontTag);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void FontMgr::Free()
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}
