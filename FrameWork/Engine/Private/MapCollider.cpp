#include "MapCollider.h"

Engine::MapCollider::MapCollider()
    : VIBuffer()
{
}

Engine::MapCollider::MapCollider(LPDIRECT3DDEVICE9 _pGraphicDevice)
    : VIBuffer(_pGraphicDevice)
{
}

Engine::MapCollider::MapCollider(const MapCollider& _rhs)
    : VIBuffer(_rhs)
{
}

Engine::MapCollider::~MapCollider()
{
}

MapCollider* Engine::MapCollider::Create(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	MapCollider* pInstance = new MapCollider(_pGraphicDevice);

	if (FAILED(pInstance->Init_Buffer()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MapCollider Create Failed");
		return nullptr;
	}

	return pInstance;
}

Component* Engine::MapCollider::Clone()
{
	return new MapCollider(*this);
}

HRESULT Engine::MapCollider::Init_Buffer()
{
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwTriCnt = 12;
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(VIBuffer::Init_Buffer(), E_FAIL);

	VTXCOL* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 전면
	pVertex[0].vPosition = m_pPos[0] = { -1.f, 1.f, -1.f };
	pVertex[0].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[1].vPosition = m_pPos[1] = { 1.f, 1.f, -1.f };
	pVertex[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[2].vPosition = m_pPos[2] = { 1.f, -1.f, -1.f };
	pVertex[2].dwColor = D3DXCOLOR(0.f,1.f,0.f,1.f); 

	pVertex[3].vPosition = m_pPos[3] = { -1.f, -1.f, -1.f };
	pVertex[3].dwColor = D3DXCOLOR(0.f,1.f,0.f,1.f); 

	// 후면
	pVertex[4].vPosition = m_pPos[4] = { -1.f, 1.f, 1.f };
	pVertex[4].dwColor = D3DXCOLOR(0.f,1.f,0.f,1.f); 

	pVertex[5].vPosition = m_pPos[5] = { 1.f, 1.f, 1.f };
	pVertex[5].dwColor = D3DXCOLOR(0.f,1.f,0.f,1.f); 

	pVertex[6].vPosition = m_pPos[6] = { 1.f, -1.f, 1.f };
	pVertex[6].dwColor = D3DXCOLOR(0.f,1.f,0.f,1.f); 

	pVertex[7].vPosition = m_pPos[7] = { -1.f, -1.f, 1.f };
	pVertex[7].dwColor = D3DXCOLOR(0.f,1.f,0.f,1.f); 

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;
	m_vecIndex.push_back(pIndex[0]);

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;
	m_vecIndex.push_back(pIndex[1]);

	// X-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;
	m_vecIndex.push_back(pIndex[2]);

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;
	m_vecIndex.push_back(pIndex[3]);

	// Y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;
	m_vecIndex.push_back(pIndex[4]);

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;
	m_vecIndex.push_back(pIndex[5]);

	// Y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;
	m_vecIndex.push_back(pIndex[6]);

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;
	m_vecIndex.push_back(pIndex[7]);

	// Z+
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;
	m_vecIndex.push_back(pIndex[8]);

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;
	m_vecIndex.push_back(pIndex[9]);

	// Z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;
	m_vecIndex.push_back(pIndex[10]);

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;
	m_vecIndex.push_back(pIndex[11]);

	m_pIB->Unlock();

	return S_OK;
}

void Engine::MapCollider::Render_Buffer()
{
	VIBuffer::Render_Buffer();
}

void Engine::MapCollider::Free()
{
	VIBuffer::Free();
}
