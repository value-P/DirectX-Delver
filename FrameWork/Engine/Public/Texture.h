#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL Texture : public Component
{
protected:
	explicit Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Texture(const Texture& rhs);
	virtual ~Texture();

public:
	HRESULT		Init_Texture(TEXTUREID eType, const wstring pPath, const _uint& iCnt);
	virtual void		Set_Texture();
	virtual void		Set_Texture(const _uint& iIndex);
	virtual IDirect3DBaseTexture9* Get_Texture(const _uint& iIndex = 0);
	TEXTUREID	Get_TexType() { return m_eTexType; }
	bool IsVecTextureEmpty();
	
protected:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	TEXTUREID							m_eTexType;
	
public:
	static Texture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTUREID eType, const wstring pPath, const _uint& iCnt = 1);
	virtual  Component*	Clone(void);

protected:
	virtual void Free();

};

END