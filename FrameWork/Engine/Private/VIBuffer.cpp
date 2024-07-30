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
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize, // ������ ������ ũ��
															0, // �����ϰ��� �ϴ� ���ؽ� ������ ����(0�� ��� ���� ���۷� ����, D3DUSAGE_DYNAMIC ���� ���۷� ����)
															m_dwFVF,	// ���ؽ��� �Ӽ� ����
															D3DPOOL_MANAGED,	// �޸� ���� ���(���� ������ ��� MANAGED)
															&m_pVB,		// ������ ���ؽ� ����
															nullptr), 
															E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, // ������ ������ ũ��
															0, // �����ϰ��� �ϴ� ���ؽ� ������ ����(0�� ��� ���� ���۷� ����, D3DUSAGE_DYNAMIC ���� ���۷� ����)
															m_IdxFmt,	// ���ؽ��� �Ӽ� ����
															D3DPOOL_MANAGED,	// �޸� ���� ���(���� ������ ��� MANAGED)
															&m_pIB,		// ������ ���ؽ� ����
															nullptr),
															E_FAIL);

	m_pPos = new D3DXVECTOR3[m_dwVtxCnt];
	m_vecIndex.reserve(m_dwTriCnt);

	return S_OK;
}

void VIBuffer::Render_Buffer(void)
{
	// SetStreamSource : ����̽� ��ġ���� ���� �׸����� �޸� ���۸� ��ũ��Ű�� �Լ�
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	// 1. �� �� ��Ʈ���� ���۸� ������ ���ΰ�
	// 2. � ����� ��ġ�� ������ ���ΰ�
	// 3. ��𼭺��� ������ ���ΰ�
	// 4. � ������ ������ ���ΰ�	


	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	// 1. �ﰢ���� �׸��� ���
	// 2. ���ؽ��� �ε��� ��(���� ���� ���� ������ ����� �ε��� ���� ���ؼ� �׷���)
	// 3. ���ؽ� ���ۿ� �� �ִ� �ε��� �� �� ��°���� �׸� ���ΰ�
	// 4. ���� ����ϰ��� �ϴ� ���ؽ��� ����
	// 5. �ε��� ���ۿ� �� �ִ� �ε��� �� �� ��°���� �׸� ���ΰ�
	// 6. �ﰢ�� ����


	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);

	// 1. �ﰢ���� �׸��� ���
	// 2. �� ��° ���ؽ����� �׸� ���ΰ�
	// 3. �ﰢ�� ����
}

void VIBuffer::Free()
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	Component::Free();
}
