#include "..\..\Header\MyFont.h"

MyFont::MyFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_pSprite(nullptr), m_pFont(nullptr)
{
	m_pGraphicDev->AddRef();
}

MyFont::~MyFont()
{
}

HRESULT MyFont::Init_Font(const wstring pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	D3DXFONT_DESC			Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.CharSet = HANGEUL_CHARSET;
	lstrcpy(Font_Desc.FaceName, pFontType.c_str());

	Font_Desc.Width = iWidth;
	Font_Desc.Height = iHeight;
	Font_Desc.Weight = iWeight;

	FAILED_CHECK_RETURN(D3DXCreateFontIndirect(m_pGraphicDev, &Font_Desc, &m_pFont), E_FAIL);
	FAILED_CHECK_RETURN(D3DXCreateSprite(m_pGraphicDev, &m_pSprite), E_FAIL);

	return S_OK;
}
	
void MyFont::Render_Font(const wstring pString, const _vec4* pPos, DWORD _dwFormat, D3DXCOLOR Color)
{
	RECT	rc = { (long)pPos->x, (long)pPos->y, (long)pPos->z, (long)pPos->w };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, pString.c_str(), pString.length(), &rc, _dwFormat, Color);

	m_pSprite->End();
}

MyFont* MyFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	MyFont* pInstance = new MyFont(pGraphicDev);

	if (FAILED(pInstance->Init_Font(pFontType, iWidth, iHeight, iWeight)))
	{
		Safe_Release(pInstance);

		MSG_BOX("MyFont Create Failed");
		return nullptr;
	}

	return pInstance;
}

void MyFont::Free()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);

	Safe_Release(m_pGraphicDev);
}
