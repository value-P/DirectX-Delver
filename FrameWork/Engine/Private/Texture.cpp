#include "..\..\Header\Texture.h"

Texture::Texture(LPDIRECT3DDEVICE9 pGraphicDev)
	: Component(pGraphicDev)
{
}

Texture::Texture(const Texture & rhs)
	: Component(rhs)
{
	_uint	iSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iSize);

	m_vecTexture = rhs.m_vecTexture;

	for (_uint i = 0; i < iSize; ++i)
		m_vecTexture[i]->AddRef();

}

Texture::~Texture()
{
}

HRESULT Texture::Init_Texture(TEXTUREID eType, const wstring pPath, const _uint & iCnt)
{
	m_vecTexture.reserve(iCnt);

	m_eTexType = eType;

	IDirect3DBaseTexture9*		pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR		szFileName[128] = L"";

		wsprintf(szFileName, pPath.c_str(), i);

		switch (eType)
		{
		case TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;

		case TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		}

		m_vecTexture.push_back(pTexture);
	}

	return S_OK;
}

void Texture::Set_Texture()
{
	m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
}


void Texture::Set_Texture(const _uint & iIndex)
{
	if (m_vecTexture.size() < iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);

}

IDirect3DBaseTexture9* Texture::Get_Texture(const _uint& iIndex)
{
	if(iIndex >= m_vecTexture.size())
		return nullptr;

	return m_vecTexture[iIndex];
}

bool Texture::IsVecTextureEmpty()
{
	if (m_vecTexture.empty() || m_vecTexture.size() > 10000)
	{
		return true;
	}

	return false;
}



Texture * Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eType, const wstring pPath, const _uint & iCnt)
{
	Texture *	pInstance = new Texture(pGraphicDev);

	if (FAILED(pInstance->Init_Texture(eType, pPath, iCnt)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Texture Create Failed");
		return nullptr;
	}	

	return pInstance;
}

Component* Texture::Clone(void)
{
	return new Texture(*this);
}

void Texture::Free()
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
	m_vecTexture.clear();
	m_vecTexture.shrink_to_fit();

	Component::Free();
}
