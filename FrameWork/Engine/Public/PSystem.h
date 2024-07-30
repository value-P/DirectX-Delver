#pragma once

#include "Base.h"
#include "Engine_Define.h"

namespace Engine
{
	class ENGINE_DLL PSystem : public Base
	{
	protected:
		explicit PSystem(LPDIRECT3DDEVICE9 _pGraphicDevice);
		virtual ~PSystem();

	protected:
		HRESULT Init_Particles();

	public:
		virtual int		Update(const float& _fTimeDelta) abstract;
		virtual void	LateUpdate() abstract;

	public:
		virtual void	PreRenderParticles();
		virtual void	RenderParticles();
		virtual void	PostRenderParticles();

	public:		// Class Function		// 각 클래스에 자기가 원하는대로 만들면 됨
		virtual void	AddParticle();
		virtual void	AddParticle(const D3DXVECTOR3& _vPos);
		virtual void	AddParticle(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir);
		virtual void	AddParticle(const D3DXVECTOR3& _vPos, const D3DCOLOR& _vColor);
		virtual void	AddParticle(const D3DXVECTOR3& _vPos, const D3DCOLOR& _vColor, const float& _fSpeed);
		virtual void	AddParticle(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const D3DCOLOR& _vColor);
		bool			IsEmpty();
		bool			IsDead();

	public:		// Class Function		// 각 클래스에 자기가 원하는대로 만들면 됨
		virtual void	ResetParticle(Attribute* _pAttribute) {};
		virtual void	ResetParticle(Attribute* _pAttribute, const D3DXVECTOR3& _vPos) {};
		virtual void	ResetParticle(Attribute* _pAttribute, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir) {};
		virtual void	ResetParticle(Attribute* _pAttribute, const D3DXVECTOR3& _vPos, const D3DCOLOR& _vColor) {};
		virtual void	ResetParticle(Attribute* _pAttribute, const D3DXVECTOR3& _vPos, const D3DCOLOR& _vColor, const float& _fSpeed) {};
		virtual void	ResetParticle(Attribute* _pAttribute, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const D3DCOLOR& _vColor) {};

	public:
		virtual void RemoveDeadParticles();

	protected:
		LPDIRECT3DDEVICE9	m_pGraphicDevice{ nullptr };
		D3DXVECTOR3			m_vOrigin{ 0.0f, 0.0f, 0.0f };
		BoundingBox			m_tBoundingBox;
		float				m_fEmitRate{ 0.0f };
		float				m_fSize{ 0.0f };
		LPDIRECT3DBASETEXTURE9 m_pTexture{ nullptr };
		LPDIRECT3DVERTEXBUFFER9 m_pVB{ nullptr };
		list<Attribute>		m_lstParticles;
		int					m_iMaxParticles{ 0 };

		DWORD				m_vbSize{ 0 };
		DWORD				m_vbOffset{ 0 };
		DWORD				m_vbBatchSize{ 0 };

	protected:
		virtual void Free();
	};
}
