#pragma once

#include "PSystem.h"

namespace Engine
{
	class ENGINE_DLL PLaserCharging : public PSystem
	{
	private:
		explicit PLaserCharging(LPDIRECT3DDEVICE9 _pGraphicDeivce);
		virtual ~PLaserCharging();

	public:		
		// Create(디바이스, 모일 위치, 반지름, 파티클 유지시간), Create하고 매니저에 넣으면 알아서 작동함
		static PLaserCharging* Create(LPDIRECT3DDEVICE9 _pGraphicDeivce, const D3DXVECTOR3* _pOrigin, const float& _fRadius, const float& _fAliveTime);

	private:
		HRESULT Init_PLaserCharging(const D3DXVECTOR3* _pOrigin, const float& _fRadius, const float& _fAliveTime);

	public:
		virtual int		Update(const float& _fTimeDelta);
		virtual void	LateUpdate();

	public:
		virtual void	PreRenderParticles();
		virtual void	PostRenderParticles();

	public:		// Class Function
		virtual void	ResetParticle(Attribute* _pAttribute, const D3DXVECTOR3& _vPos);

	private:
		const D3DXVECTOR3* m_pOrigin{ nullptr };
		float		m_fRadius{ 0.0f };
		float		m_fAliveTime{ 0.0f };
		float		m_fAgeTime{ 0.0f };
		
	private:
		virtual void Free();
	};
}
