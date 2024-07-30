#include "..\..\Header\CubeTex.h"



CubeTex::CubeTex()
{
}

CubeTex::CubeTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: VIBuffer(pGraphicDev)
{
}

CubeTex::CubeTex(const CubeTex & rhs)
	: VIBuffer(rhs)
{
}

CubeTex::~CubeTex()
{
}

CubeTex * CubeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CubeTex *	pInstance = new CubeTex(pGraphicDev);

	if (FAILED(pInstance->Init_Buffer()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CubeTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CubeTex::Free()
{
	VIBuffer::Free();
}

Component * CubeTex::Clone()
{
	return new CubeTex(*this);
}

HRESULT Engine::CubeTex::Init_Buffer(void)
{
	m_dwVtxCnt	= 8;
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwTriCnt	= 12;
	m_dwFVF		= FVF_CUBE;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;
	
	FAILED_CHECK_RETURN(VIBuffer::Init_Buffer(), E_FAIL);

	VTXCUBE*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 전면
	pVertex[0].vPosition = m_pPos[0] = { -1.f, 1.f, -1.f };
	pVertex[0].vTexUV = pVertex[0].vPosition;

	pVertex[1].vPosition = m_pPos[1] = { 1.f, 1.f, -1.f };
	pVertex[1].vTexUV = pVertex[1].vPosition;

	pVertex[2].vPosition = m_pPos[2] = { 1.f, -1.f, -1.f };
	pVertex[2].vTexUV = pVertex[2].vPosition;

	pVertex[3].vPosition = m_pPos[3] = { -1.f, -1.f, -1.f };
	pVertex[3].vTexUV = pVertex[3].vPosition;

	// 후면
	pVertex[4].vPosition = m_pPos[4] = { -1.f, 1.f, 1.f };
	pVertex[4].vTexUV = pVertex[4].vPosition;

	pVertex[5].vPosition = m_pPos[5] = { 1.f, 1.f, 1.f };
	pVertex[5].vTexUV = pVertex[5].vPosition;

	pVertex[6].vPosition = m_pPos[6] = { 1.f, -1.f, 1.f };
	pVertex[6].vTexUV = pVertex[6].vPosition;

	pVertex[7].vPosition = m_pPos[7] = { -1.f, -1.f, 1.f };
	pVertex[7].vTexUV = pVertex[7].vPosition;

	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;

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
	pIndex[4]._0 =  4;
	pIndex[4]._1 =  5;
	pIndex[4]._2 =  1;
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

void Engine::CubeTex::Render_Buffer(void)
{
	VIBuffer::Render_Buffer();
}


