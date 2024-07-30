#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL RcTex : public VIBuffer
{
private:
	explicit RcTex();
	explicit RcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit RcTex(const RcTex& rhs);
	virtual ~RcTex();
	
public:
	virtual HRESULT Init_Buffer(void) override;
	virtual void Render_Buffer(void) override;

	virtual void	Set_UVScale(const float& scaleU, const float& scaleV) override;

public:
	virtual Component * Clone() override;
	static RcTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

END