#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CubeTex : public VIBuffer
{
private:
	explicit CubeTex();
	explicit CubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CubeTex(const CubeTex& rhs);
	virtual ~CubeTex();
	
public:
	virtual HRESULT Init_Buffer(void) override;
	virtual void Render_Buffer(void) override;

public:
	virtual Component * Clone() override;
	static CubeTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

END