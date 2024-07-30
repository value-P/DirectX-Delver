#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL TerrainTex : public VIBuffer
{
private:
	explicit TerrainTex();
	explicit TerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit TerrainTex(const TerrainTex& rhs);
	virtual ~TerrainTex();

public:
	_ulong					Get_VtxCntX() { return m_iH.biWidth; }
	_ulong					Get_VtxCntZ() { return m_iH.biHeight; }
	
public:
	HRESULT Init_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual void Render_Buffer(void) override;


private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

public:
	virtual Component * Clone() override;
	static TerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX = VTXCNTX, const _ulong& dwCntZ = VTXCNTZ, const _ulong& dwVtxItv = VTXITV);

private:
	virtual void Free() override;

};

END