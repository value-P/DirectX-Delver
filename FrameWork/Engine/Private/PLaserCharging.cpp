#include "PLaserCharging.h"

Engine::PLaserCharging::PLaserCharging(LPDIRECT3DDEVICE9 _pGraphicDeivce)
	:PSystem(_pGraphicDeivce)
{
}

Engine::PLaserCharging::~PLaserCharging()
{
}

PLaserCharging* Engine::PLaserCharging::Create(LPDIRECT3DDEVICE9 _pGraphicDeivce, const D3DXVECTOR3* _pOrigin, const float& _fRadius, const float& _fAliveTime)
{
	PLaserCharging* pInstance = new PLaserCharging(_pGraphicDeivce);

	if (FAILED(pInstance->Init_PLaserCharging(_pOrigin,_fRadius,_fAliveTime)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_PLaserCharging Failed", L"PLaserCharging::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT Engine::PLaserCharging::Init_PLaserCharging(const D3DXVECTOR3* _pOrigin, const float& _fRadius, const float& _fAliveTime)
{
	m_fSize = 0.3f;
	m_vbSize = 64;
	m_vbBatchSize = 64;
	m_pOrigin = _pOrigin;
	m_fRadius = _fRadius;
	m_fAliveTime = _fAliveTime;

	for (int i = 0; i < 64; i++)
	{
		AddParticle(*m_pOrigin);
	}

	if (FAILED(PSystem::Init_Particles()))
	{
		return E_FAIL;
	}

	return S_OK;
}

int Engine::PLaserCharging::Update(const float& _fTimeDelta)
{
	list<Attribute>::iterator iter{ m_lstParticles.begin() };

	for (; iter != m_lstParticles.end(); iter++)
	{
		if (iter->bAlive)
		{
			D3DXVec3Lerp(&iter->vPos, &iter->vPosLinear, m_pOrigin, iter->fAge);
			iter->fAge += _fTimeDelta;
			if (iter->fAge > 1.0f)
			{
				if (m_fAliveTime > 0.0f)
				{
					ResetParticle(&(*iter), *m_pOrigin);
				}
				else
				{
					iter->bAlive = false;
				}
			}
		}
	}
	if (m_fAliveTime > 0.0f)
	{
		m_fAliveTime -= _fTimeDelta;
	}

	return 0;
}

void Engine::PLaserCharging::LateUpdate()
{
}

void Engine::PLaserCharging::PreRenderParticles()
{
	PSystem::PreRenderParticles();
}

void Engine::PLaserCharging::PostRenderParticles()
{
	PSystem::PostRenderParticles();
}

void Engine::PLaserCharging::ResetParticle(Attribute* _pAttribute, const D3DXVECTOR3& _vPos)
{
	_pAttribute->vPosLinear.x = _vPos.x + GetRandomFloat(-m_fRadius, m_fRadius);
	_pAttribute->vPosLinear.y = _vPos.y + GetRandomFloat(-m_fRadius, m_fRadius);
	_pAttribute->vPosLinear.z = _vPos.z + GetRandomFloat(-m_fRadius, m_fRadius);
	_pAttribute->fAge = GetRandomFloat(0.0f, 0.5f);
	_pAttribute->dwColor = D3DXCOLOR{ GetRandomFloat(0.0f,1.0f),GetRandomFloat(0.0f,1.0f),GetRandomFloat(0.0f,1.0f),1.0f };
	_pAttribute->fSize = 1.0f;
	_pAttribute->bAlive = true;
}

void Engine::PLaserCharging::Free()
{
	PSystem::Free();
}
