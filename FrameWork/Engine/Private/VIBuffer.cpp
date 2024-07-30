#include "..\..\Header\VIBuffer.h"

VIBuffer::VIBuffer() : m_pVB(nullptr), m_pIB(nullptr)
{
}

VIBuffer::VIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Component(pGraphicDev), m_pVB(nullptr), m_pIB(nullptr),
	m_dwVtxSize(0), m_dwFVF(0), m_dwVtxCnt(0), m_dwTriCnt(0)
	, m_dwIdxSize(0)
{
}

VIBuffer::VIBuffer(const VIBuffer & rhs)
	: Component(rhs), m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB), 
	m_dwVtxSize(rhs.m_dwVtxSize), m_dwFVF(rhs.m_dwFVF), 
	m_dwVtxCnt(rhs.m_dwVtxCnt), m_dwTriCnt(rhs.m_dwTriCnt), m_dwIdxSize(rhs.m_dwIdxSize), m_IdxFmt(rhs.m_IdxFmt), m_pPos(rhs.m_pPos)
	,m_vecIndex(rhs.m_vecIndex)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}


VIBuffer::~VIBuffer()
{
	if (false == m_bClone)
	{
		Safe_Delete_Array(m_pPos);
	}
	m_vecIndex.clear();
}

HRESULT VIBuffer::Init_Buffer(void)
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize, // 생성할 버퍼의 크기
															0, // 생성하고자 하는 버텍스 버퍼의 종류(0인 경우 정적 버퍼로 생성, D3DUSAGE_DYNAMIC 동적 버퍼로 생성)
															m_dwFVF,	// 버텍스의 속성 옵현
															D3DPOOL_MANAGED,	// 메모리 저장 방식(정적 버퍼인 경우 MANAGED)
															&m_pVB,		// 생성한 버텍스 버퍼
															nullptr), 
															E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, // 생성할 버퍼의 크기
															0, // 생성하고자 하는 버텍스 버퍼의 종류(0인 경우 정적 버퍼로 생성, D3DUSAGE_DYNAMIC 동적 버퍼로 생성)
															m_IdxFmt,	// 버텍스의 속성 옵현
															D3DPOOL_MANAGED,	// 메모리 저장 방식(정적 버퍼인 경우 MANAGED)
															&m_pIB,		// 생성한 버텍스 버퍼
															nullptr),
															E_FAIL);

	m_pPos = new D3DXVECTOR3[m_dwVtxCnt];
	m_vecIndex.reserve(m_dwTriCnt);

	return S_OK;
}

void VIBuffer::Render_Buffer(void)
{
	// SetStreamSource : 디바이스 장치에게 현재 그리려는 메모리 버퍼를 링크시키는 함수
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	// 1. 몇 번 스트림에 버퍼를 전송할 것인가
	// 2. 어떤 대상을 장치에 연결할 것인가
	// 3. 어디서부터 전달할 것인가
	// 4. 어떤 단위로 전달할 것인가	


	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	// 1. 삼각형을 그리는 방식
	// 2. 버텍스의 인덱스 값(넣은 값에 따라 기존에 저장된 인덱스 값에 더해서 그려줌)
	// 3. 버텍스 버퍼에 들어가 있는 인덱스 중 몇 번째부터 그릴 것인가
	// 4. 내가 사용하고자 하는 버텍스의 개수
	// 5. 인덱스 버퍼에 들어가 있는 인덱스 중 몇 번째부터 그릴 것인가
	// 6. 삼각형 개수


	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);

	// 1. 삼각형을 그리는 방식
	// 2. 몇 번째 버텍스부터 그릴 것인가
	// 3. 삼각형 개수
}

void VIBuffer::Free()
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	Component::Free();
}
