#pragma once
#include "VIBuffer.h"

namespace Engine
{
    class ENGINE_DLL ColliderFrame : public VIBuffer
    {
	private:
		explicit ColliderFrame();
		explicit ColliderFrame(LPDIRECT3DDEVICE9 _pGraphicDevice);
		explicit ColliderFrame(const ColliderFrame& _rhs);
		virtual ~ColliderFrame();

	public:
		static ColliderFrame* Create(LPDIRECT3DDEVICE9 _pGraphicDevice);
		virtual Component* Clone() override;

	public:
		HRESULT InitBuffer();
		void RenderBuffer(bool _bSpCol, bool _bOBBCol);

	private:
		HRESULT SettingNonColVI();
		HRESULT SettingColSpVI();
		HRESULT SettingColOBBVI();

	private:
		LPDIRECT3DVERTEXBUFFER9 m_pColSpVB{ nullptr };
		LPDIRECT3DINDEXBUFFER9 m_pColSpIB{ nullptr };
		LPDIRECT3DVERTEXBUFFER9 m_pColOBBVB{ nullptr };
		LPDIRECT3DINDEXBUFFER9 m_pColOBBIB{ nullptr };

	private:
		virtual void Free() override;

	};
}


