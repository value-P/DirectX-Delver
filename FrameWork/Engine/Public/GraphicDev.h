#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)
	
class ENGINE_DLL GraphicDev : public Base
{
	DECLARE_SINGLETON(GraphicDev)

private:
	explicit GraphicDev();
	virtual ~GraphicDev();

public:
	LPDIRECT3DDEVICE9		Get_GraphicDev() { return m_pGraphicDev; }

public:
	HRESULT			Init_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, GraphicDev** ppGraphicDev);

	void			Render_Begin(D3DXCOLOR Color);
	void			Render_End();


private:
	LPDIRECT3D9				m_pSDK;
	LPDIRECT3DDEVICE9		m_pGraphicDev;

private:
	virtual void		Free();

};

END