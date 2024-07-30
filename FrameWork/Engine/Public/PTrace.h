#pragma once

#include "PSystem.h"

namespace Engine
{
	class ENGINE_DLL PTrace : public PSystem
	{
	private:
		explicit PTrace(LPDIRECT3DDEVICE9 _pGraphicDeivce);
		virtual ~PTrace();

	public:
		static PTrace* Create(LPDIRECT3DDEVICE9 _pGraphicDeivce);

	private:
		HRESULT Init_PTrace();

	public:
		virtual int		Update(const float& _fTimeDelta);
		virtual void	LateUpdate();

	public:
		virtual void	PreRenderParticles();
		virtual void	PostRenderParticles();

	public:		// Class Function
		virtual void	ResetParticle(Attribute* _pAttribute, const D3DXVECTOR3& _vPos, const D3DCOLOR& _vColor);

	private:
		virtual void Free();
	};
}
