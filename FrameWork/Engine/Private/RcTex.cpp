#include "..\..\Header\RcTex.h"



RcTex::RcTex()
{
}

RcTex::RcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: VIBuffer(pGraphicDev)
{
}

RcTex::RcTex(const RcTex & rhs)
	: VIBuffer(rhs)
{
}

RcTex::~RcTex()
{
}

RcTex * RcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	RcTex *	pInstance = new RcTex(pGraphicDev);

	if (FAILED(pInstance->Init_Buffer()))
	{
		Safe_Release(pInstance);

		MSG_BOX("RcTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

void RcTex::Free()
{
	VIBuffer::Free();
}

Component * RcTex::Clone()
{
	return new RcTex(*this);
}

HRESULT Engine::RcTex::Init_Buffer(void)
{
	m_dwVtxCnt	= 4;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwTriCnt	= 2;
	m_dwFVF		= FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(VIBuffer::Init_Buffer(), E_FAIL);

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 위쪽
	pVertex[0].vPosition = m_pPos[0] = { -1.f, 1.f, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPosition = m_pPos[1] = { 1.f, 1.f, 0.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPosition = m_pPos[2] = { 1.f, -1.f, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPosition = m_pPos[3] = { -1.f, -1.f, 0.f };
	pVertex[3].vTexUV = { 0.f, 1.f };

	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;
	m_vecIndex.push_back(pIndex[0]);


	// 왼쪽 아래
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;
	m_vecIndex.push_back(pIndex[1]);

	m_pIB->Unlock();



	return S_OK;
}

void Engine::RcTex::Render_Buffer(void)
{
	VIBuffer::Render_Buffer();
}

void RcTex::Set_UVScale(const float& scaleU, const float& scaleV)
{
	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[1].vTexUV.x = scaleU;

	pVertex[2].vTexUV.x = scaleU;
	pVertex[2].vTexUV.y = scaleV;

	pVertex[3].vTexUV.y = scaleV;

	m_pVB->Unlock();
}


