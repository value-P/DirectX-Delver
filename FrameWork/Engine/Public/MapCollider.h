#pragma once
#include "VIBuffer.h"

namespace Engine
{
	class ENGINE_DLL MapCollider : public VIBuffer
	{
	private:
		explicit MapCollider();
		explicit MapCollider(LPDIRECT3DDEVICE9 _pGraphicDevice);
		explicit MapCollider(const MapCollider& _rhs);
		virtual ~MapCollider();

	public:
		static MapCollider* Create(LPDIRECT3DDEVICE9 _pGraphicDevice);
		virtual Component* Clone() override;

	public:
		virtual HRESULT Init_Buffer() override;
		virtual void Render_Buffer() override;

	private:
		virtual void Free() override;
	};
}

