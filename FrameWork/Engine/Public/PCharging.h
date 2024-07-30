#pragma once

#include "PSystem.h"

namespace Engine
{
    class ENGINE_DLL PCharging : public PSystem
    {
	private:
		explicit PCharging(LPDIRECT3DDEVICE9 _pGraphicDeivce);
		virtual ~PCharging();

	public:		// vOrigin�� ��ƼŬ ���� ���� ��ġ�� �־���, AddParticle(const D3DXVECTOR3& _vPos, const D3DCOLOR& _vColor, const float& _fSpeed)�� ��ƼŬ�� �־��ָ� ��
		static PCharging* Create(LPDIRECT3DDEVICE9 _pGraphicDeivce, const D3DXVECTOR3& _vOrigin);

	private:
		HRESULT Init_PCharging(const D3DXVECTOR3& _vOrigin);

	public:
		virtual int		Update(const float& _fTimeDelta);
		virtual void	LateUpdate();

	public:
		virtual void	PreRenderParticles();
		virtual void	PostRenderParticles();

	public:		// Class Function
		virtual void	ResetParticle(Attribute* _pAttribute, const D3DXVECTOR3& _vPos, const D3DCOLOR& _vColor, const float& _fSpeed);

	private:
		virtual void Free();
    };
}


