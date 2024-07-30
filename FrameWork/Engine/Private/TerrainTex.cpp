#include "..\..\Header\TerrainTex.h"



TerrainTex::TerrainTex()
{
}

TerrainTex::TerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: VIBuffer(pGraphicDev), m_fH({}), m_iH({}), m_hFile(nullptr)
{
}

TerrainTex::TerrainTex(const TerrainTex & rhs)
	: VIBuffer(rhs), m_hFile(rhs.m_hFile), m_fH(rhs.m_fH), m_iH(rhs.m_iH)
{
}

TerrainTex::~TerrainTex()
{
}

TerrainTex * TerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	TerrainTex *	pInstance = new TerrainTex(pGraphicDev);

	if (FAILED(pInstance->Init_Buffer(dwCntX, dwCntZ, dwVtxItv)))
	{
		Safe_Release(pInstance);

		MSG_BOX("TerrainTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

void TerrainTex::Free()
{
	VIBuffer::Free();
}

Component * TerrainTex::Clone()
{
	return new TerrainTex(*this);
}

HRESULT Engine::TerrainTex::Init_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	m_dwVtxCnt	= dwCntX * dwCntZ;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwTriCnt	= (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwFVF		= FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(VIBuffer::Init_Buffer(), E_FAIL);

	m_hFile = CreateFile(L"../../Resource/Texture/Terrain/Height.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	_ulong	dwByte(0);

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	_ulong*		pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biWidth * m_iH.biHeight, &dwByte, nullptr);

	CloseHandle(m_hFile);


	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_ulong		dwIndex(0);

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPosition = _vec3(_float(j * dwVtxItv),
				(pPixel[dwIndex] & 0x000000ff) / 20.f,
				_float(i * dwVtxItv));

			m_pPos[dwIndex] = pVertex[dwIndex]
				.vPosition;

			pVertex[dwIndex].vNormal = { 0.f, 0.f, 0.f };

			pVertex[dwIndex].vTexUV = _vec2(_float(j) / (dwCntX - 1) * 20.f, 
											_float(i) / (dwCntZ - 1) * 20.f);
		}
	}
	
	Safe_Delete_Array(pPixel);

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong		dwTriCnt(0);

	_vec3		vDst, vSrc, vNormal;

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// 오른쪽 위
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;
			m_vecIndex.push_back(pIndex[dwTriCnt]);

			vDst = pVertex[pIndex[dwTriCnt]._1].vPosition - pVertex[pIndex[dwTriCnt]._0].vPosition;
			vSrc = pVertex[pIndex[dwTriCnt]._2].vPosition - pVertex[pIndex[dwTriCnt]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDst, &vSrc);

			pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;

			dwTriCnt++;

			// 왼쪽 아래
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;
			m_vecIndex.push_back(pIndex[dwTriCnt]);

			vDst = pVertex[pIndex[dwTriCnt]._1].vPosition - pVertex[pIndex[dwTriCnt]._0].vPosition;
			vSrc = pVertex[pIndex[dwTriCnt]._2].vPosition - pVertex[pIndex[dwTriCnt]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDst, &vSrc);

			pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;

			dwTriCnt++;
		}
	}
	
	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();


	return S_OK;
}

void Engine::TerrainTex::Render_Buffer(void)
{
	VIBuffer::Render_Buffer();
}


