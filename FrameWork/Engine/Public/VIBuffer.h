#pragma once
#include "Component.h"

namespace Engine
{
	class ENGINE_DLL VIBuffer : public Component
	{
	protected:
		explicit VIBuffer();
		explicit VIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
		explicit VIBuffer(const VIBuffer& rhs);
		virtual ~VIBuffer();

	public:
		virtual HRESULT				Init_Buffer(void);
		virtual void				Render_Buffer(void);

		_vec3* Get_VtxPos() { return m_pPos; }
		const vector<INDEX32>& GetIndex() { return m_vecIndex; }
		virtual void	Set_UVScale(const float& scaleU, const float& scaleV) {};
	protected:
		LPDIRECT3DVERTEXBUFFER9			m_pVB;
		LPDIRECT3DINDEXBUFFER9			m_pIB;

		_ulong				m_dwVtxCnt;			// ������ ������ ����
		_ulong				m_dwVtxSize;		// ������ ũ��
		_ulong				m_dwTriCnt;			// �׸����� �ϴ� �ﰢ���� ����
		_ulong				m_dwFVF;			// ���ؽ��� �Ӽ�

		_ulong				m_dwIdxSize;
		D3DFORMAT			m_IdxFmt;

		_vec3* m_pPos{ nullptr };
		vector<INDEX32>		m_vecIndex;

	protected:
		virtual void		Free();
	};
}
