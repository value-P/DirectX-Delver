#include "PickCube.h"

Engine::PickCube::PickCube()
	: VIBuffer()
{
}

Engine::PickCube::PickCube(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: VIBuffer(_pGraphicDevice)
{
}

Engine::PickCube::PickCube(const PickCube& _rhs)
	: VIBuffer(dynamic_cast<const VIBuffer&>(_rhs))
{
}

Engine::PickCube::~PickCube()
{
}

PickCube* Engine::PickCube::Create(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	PickCube* pInstance = new PickCube(_pGraphicDevice);

	if (FAILED(pInstance->Init_Buffer()))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"InitBuffer Failed", L"PickCube::Create", MB_OK);
		return nullptr;
	}
	return pInstance;
}

Component* Engine::PickCube::Clone()
{
	return new PickCube(*this);
}

HRESULT Engine::PickCube::Init_Buffer()
{
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwTriCnt = 12;
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwTriCnt * m_dwIdxSize, 0, m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, nullptr)))
	{
		return E_FAIL;
	}

	VTXCOL* vertices{ nullptr };
	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = { { -1.0f,1.0f,-1.0f },D3DXCOLOR(1.0f,1.0f,0.0f,1.0f) };
	vertices[1] = { { 1.0f,1.0f,-1.0f }, D3DXCOLOR(1.0f,1.0f,0.0f,1.0f) };
	vertices[2] = { { 1.0f,-1.0f,-1.0f } ,D3DXCOLOR(1.0f,1.0f,0.0f,1.0f) };
	vertices[3] = { { -1.0f,-1.0f,-1.0f } ,D3DXCOLOR(1.0f,1.0f,0.0f,1.0f) };
	vertices[4] = { { -1.0f,1.0f,1.0f },D3DXCOLOR(1.0f,1.0f,0.0f,1.0f) };
	vertices[5] = { { 1.0f,1.0f,1.0f } ,D3DXCOLOR(1.0f,1.0f,0.0f,1.0f) };
	vertices[6] = { { 1.0f,-1.0f,1.0f } ,D3DXCOLOR(1.0f,1.0f,0.0f,1.0f) };
	vertices[7] = { { -1.0f,-1.0f,1.0f } ,D3DXCOLOR(1.0f,1.0f,0.0f,1.0f) };

	m_pVB->Unlock();


	INDEX32* indices{ nullptr };

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	// ÁßÁ¡ 0,0,0 À» ±âÁØÀ¸·Î x y z ¹æÇâÀÇ »ï°¢ÇüÀ¸·Î ³ª´®
	// -X ¹æÇâ
	// ¿ÞÂÊ ¹Ø
	indices[0]._0 = 7;	indices[0]._1 = 4;	indices[0]._2 = 3;
	// ¿À¸¥ÂÊ À§
	indices[1]._0 = 0;	indices[1]._1 = 3; indices[1]._2 = 4;

	// +X ¹æÇâ
	// ¿ÞÂÊ ¹Ø
	indices[2]._0 = 2;	indices[2]._1 = 1; indices[2]._2 = 6;
	// ¿À¸¥ÂÊ À§
	indices[3]._0 = 5;	indices[3]._1 = 6; indices[3]._2 = 1;

	// -Y ¹æÇâ
	// ¿ÞÂÊ ¹Ø
	indices[4]._0 = 7;	indices[4]._1 = 3; indices[4]._2 = 6;
	// ¿À¸¥ÂÊ À§
	indices[5]._0 = 2;	indices[5]._1 = 6; indices[5]._2 = 3;

	// +Y ¹æÇâ
	// ¿ÞÂÊ ¹Ø
	indices[6]._0 = 0;	indices[6]._1 = 4; indices[6]._2 = 1;
	// ¿À¸¥ÂÊ À§
	indices[7]._0 = 5;	indices[7]._1 = 1; indices[7]._2 = 4;

	// -Z ¹æÇâ
	// ¿ÞÂÊ ¹Ø
	indices[8]._0 = 3;	indices[8]._1 = 0; indices[8]._2 = 2;
	// ¿À¸¥ÂÊ À§
	indices[9]._0 = 1;	indices[9]._1 = 2; indices[9]._2 = 0;

	// +Z ¹æÇâ
	// ¿ÞÂÊ ¹Ø
	indices[10]._0 = 6;	indices[10]._1 = 5; indices[10]._2 = 7;
	// ¿À¸¥ÂÊ À§
	indices[11]._0 = 4;	indices[11]._1 = 7; indices[11]._2 = 5;

	m_pIB->Unlock();

	return S_OK;
}

void Engine::PickCube::Render_Buffer()
{
	VIBuffer::Render_Buffer();
}

void Engine::PickCube::Free()
{
	VIBuffer::Free();
}
