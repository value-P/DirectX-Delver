#include "PSystem.h"

PSystem::PSystem(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: Base(), m_pGraphicDevice(_pGraphicDevice)
{
	m_pGraphicDevice->AddRef();
}

PSystem::~PSystem()
{
}

HRESULT PSystem::Init_Particles()
{
	if (FAILED(m_pGraphicDevice->CreateVertexBuffer(m_vbSize * sizeof(PTC), D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, FVF_PARTICLE, D3DPOOL_DEFAULT, &m_pVB, 0)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void PSystem::PreRenderParticles()
{
	m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);

	m_pGraphicDevice->SetRenderState(D3DRS_POINTSIZE, FtoDW(m_fSize));
	m_pGraphicDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.0f));
	m_pGraphicDevice->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDW(15.0f));

	m_pGraphicDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(1.0f));
	m_pGraphicDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(4.0f));
	m_pGraphicDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(3.0f));

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

void PSystem::RenderParticles()
{
	if (!m_lstParticles.empty())
	{
		PreRenderParticles();

		D3DXMATRIX matIdentity;
		D3DXMatrixIdentity(&matIdentity);

		m_pGraphicDevice->SetTexture(0, m_pTexture);
		m_pGraphicDevice->SetFVF(FVF_PARTICLE);
		m_pGraphicDevice->SetStreamSource(0, m_pVB, 0, sizeof(PTC));
		m_pGraphicDevice->SetTransform(D3DTS_WORLD, &matIdentity);

		if (m_vbOffset >= m_vbSize)
		{
			m_vbOffset = 0;
		}

		PTC* pParticle{ nullptr };

		m_pVB->Lock(m_vbOffset * sizeof(PTC), m_vbBatchSize * sizeof(PTC), (void**)&pParticle, m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD NumParticlesInBatch{ 0 };

		list<Attribute>::iterator iter;

		for (iter = m_lstParticles.begin(); iter != m_lstParticles.end(); iter++)
		{
			if (iter->bAlive)
			{
				pParticle->vPos = iter->vPos;
				pParticle->dwColor = (D3DCOLOR)iter->dwColor;
				pParticle++;
				NumParticlesInBatch++;

				if (NumParticlesInBatch == m_vbBatchSize)
				{
					m_pVB->Unlock();

					m_pGraphicDevice->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, m_vbBatchSize);

					m_vbOffset += m_vbBatchSize;

					if (m_vbOffset >= m_vbSize)
					{
						m_vbOffset = 0;
					}

					m_pVB->Lock(m_vbOffset * sizeof(PTC), m_vbBatchSize * sizeof(PTC), (void**)&pParticle, m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					NumParticlesInBatch = 0;

				}
			}
		}

		m_pVB->Unlock();

		if (NumParticlesInBatch)
		{
			m_pGraphicDevice->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, m_vbBatchSize);
		}

		m_vbOffset += m_vbBatchSize;

		PostRenderParticles();
	}
}

void PSystem::PostRenderParticles()
{
	m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void Engine::PSystem::AddParticle()
{
	Attribute attribute;

	ResetParticle(&attribute);

	m_lstParticles.push_back(attribute);
}

void Engine::PSystem::AddParticle(const D3DXVECTOR3& _vPos)
{
	Attribute attribute;

	ResetParticle(&attribute, _vPos);

	m_lstParticles.push_back(attribute);
}

void Engine::PSystem::AddParticle(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir)
{
	Attribute attribute;

	ResetParticle(&attribute, _vPos, _vDir);

	m_lstParticles.push_back(attribute);
}

void Engine::PSystem::AddParticle(const D3DXVECTOR3& _vPos, const D3DCOLOR& _vColor)
{
	Attribute attribute;

	ResetParticle(&attribute, _vPos, _vColor);

	m_lstParticles.push_back(attribute);
}

void Engine::PSystem::AddParticle(const D3DXVECTOR3& _vPos, const D3DCOLOR& _vColor, const float& _fSpeed)
{
	Attribute attribute;

	ResetParticle(&attribute, _vPos, _vColor, _fSpeed);

	m_lstParticles.push_back(attribute);
}

void PSystem::AddParticle(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const D3DCOLOR& _vColor)
{
	Attribute attribute;

	ResetParticle(&attribute, _vPos, _vDir, _vColor);

	m_lstParticles.push_back(attribute);
}

bool PSystem::IsEmpty()
{
	return m_lstParticles.empty();
}

bool PSystem::IsDead()
{
	list<Attribute>::iterator iter{ m_lstParticles.begin() };
	for (; iter != m_lstParticles.end(); iter++)
	{
		if (true == iter->bAlive)
		{
			return false;
		}
	}

	return true;
}

void PSystem::RemoveDeadParticles()
{
	list<Attribute>::iterator iter = m_lstParticles.begin();
	while (iter != m_lstParticles.end())
	{
		if (false == iter->bAlive)
		{
			iter = m_lstParticles.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void PSystem::Free()
{
	m_pVB->Release();
	Safe_Release(m_pGraphicDevice);
}
