#pragma once

#include "VIBuffer.h"

namespace Engine
{
	class ENGINE_DLL PickCube : public VIBuffer
	{
	private:
		explicit PickCube();
		explicit PickCube(LPDIRECT3DDEVICE9 _pGraphicDevice);
		explicit PickCube(const PickCube& _rhs);
		virtual ~PickCube();

	public:
		static PickCube* Create(LPDIRECT3DDEVICE9 _pGraphicDevice);
		virtual Component* Clone() override;

	public:
		virtual HRESULT Init_Buffer();
		virtual void Render_Buffer();

	private:

	private:
		virtual void Free() override;
	};
}