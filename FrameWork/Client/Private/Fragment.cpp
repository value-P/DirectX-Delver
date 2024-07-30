#include "stdafx.h"
#include "Fragment.h"
#include "Export_System.h"
#include "Export_Utility.h"

Fragment::Fragment(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: Effect(_pGraphicDevice)
{
}

Fragment::~Fragment()
{
}

Fragment* Fragment::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const FRAG& _eType)
{
	Fragment* pInstance = new Fragment(_pGraphicDevice);

	if (FAILED(pInstance->Init_Fragment(_vPos, _vDir, _eType)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_Fragment Failed", L"Fragment::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT Fragment::Init_Fragment(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const FRAG& _eType)
{
	if (FAILED(Effect::Init_Effect()))
	{
		return E_FAIL;
	}

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	m_pColliderCom->SetSize(0.7f, 0.7f, 0.7f);
	m_pColliderCom->SetColLayer(COL_FRAGMENT);

	m_pTransformCom->Set_Scale(0.6f, 0.6f, 0.6f);
	m_pTransformCom->Set_Pos(_vPos.x + (rand() % 11 - 5) * 0.1f, _vPos.y + (rand() % 11 - 5) * 0.1f + 0.8f, _vPos.z + (rand() % 11 - 5) * 0.1f);
	m_vMoveDir = { _vDir.x * float(rand() % 10) * 0.2f,0.5f,_vDir.z * float(rand() % 10) * 0.2f };
	//D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);
	/*if (1.0f == m_vMoveDir.y)
	{
		m_vMoveDir.y = 0.0f;
	}*/
	m_fAliveTime = 20.0f;

	switch (_eType)
	{
	case FRAG_BOX:
	{
		m_iTexNum = rand() % 3;
	}
	break;
	case FRAG_CERAMIC:
	{
		m_iTexNum = rand() % 3 + 3;
	}
	break;
	case FRAG_ARROW:
	{
		m_iTexNum = rand() % 2 + 6;
	}
	break;
	}
	
	return S_OK;
}

int Fragment::Update_GameObject(const _float& _fTimeDelta)
{
	GameObject::Update_GameObject(_fTimeDelta);
	if (Effect::Update_GameObject(_fTimeDelta) & -1) { return -1; }

	if (!m_bOnTerrain)
	{
		m_pTransformCom->Move_Forward(&m_vMoveDir, _fTimeDelta, 9.f);
		m_vMoveDir.y -= _fTimeDelta * 4.0f;
		m_pColliderCom->ReBuild();
		Engine::AddCollider(m_pColliderCom);
	}
	else
	{
		m_fTime += _fTimeDelta;
		if ((UINT)m_fTime % 2 == 0)
		{
			m_pTransformCom->Move_Forward(&D3DXVECTOR3{ 0.0f,-1.0f,0.0f }, _fTimeDelta, 0.1f);
		}
		else
		{
			m_pTransformCom->Move_Forward(&D3DXVECTOR3{ 0.0f,1.0f,0.0f }, _fTimeDelta, 0.1f);
		}
	}


	return 0;
}

void Fragment::LateUpdate_GameObject()
{
	if (!m_bOnTerrain || m_pColliderCom->GetColLayer() & COL_WALL)
	{
		PlaneCollision(&m_bOnTerrain, &m_bOnWall);
		if (m_bOnTerrain)
		{
			memset(&m_vMoveDir, 0, sizeof(D3DXVECTOR3));
		}
		else if (m_bOnWall)
		{
			m_vMoveDir.x = 0.0f;
			m_vMoveDir.z = 0.0f;
		}
	}
	

	Effect::LateUpdate_GameObject();
}

void Fragment::Render_GameObject()
{
	ComputeBillBoard();
	m_pTextureCom->Set_Texture(m_iTexNum);
	m_pBufferCom->Render_Buffer();
}

HRESULT Fragment::Add_Component()
{
	m_pTextureCom = dynamic_cast<Texture*>(Clone_Proto(L"Proto_Fragment"));
	if (nullptr == m_pTextureCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_Fragment", m_pTextureCom });

	m_pColliderCom = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
	if (nullptr == m_pColliderCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_Collider",m_pColliderCom });
	m_pColliderCom->SetTransform(m_pTransformCom);

	return S_OK;
}

void Fragment::PlaneCollision(bool* _bOnTerrain,bool* _bOnWall)
{
	list<pair<CINFO, D3DXVECTOR3>>::const_iterator iter;

	for (iter = m_pColliderCom->GetColOBB_Info(COL_WALL).begin(); iter != m_pColliderCom->GetColOBB_Info(COL_WALL).end(); iter++)
	{
		D3DXVECTOR3 PlayerPos{ m_pTransformCom->Get_Info(INFO_POS) };
		float fRadius{ m_pColliderCom->GetRadius() };
		D3DXVECTOR3 Dir{ PlayerPos - (*iter).second };
		float LengthSq = D3DXVec3LengthSq(&Dir);
		if (LengthSq < fRadius * fRadius)
		{
			D3DXVECTOR3 vNormalDir{};
			D3DXVec3Normalize(&vNormalDir, &Dir);
			float fRadian{ D3DXVec3Dot(&vNormalDir, &D3DXVECTOR3{ 0.0f,1.0f,0.0f }) };
			D3DXVECTOR3 fSize{ m_pColliderCom->GetSize() };
			float fDistance{ fRadius - D3DXVec3Length(&Dir) };
			if (fRadian * fRadius >= fSize.y)
			{
				if ((*iter).second.y >= PlayerPos.y - fSize.y)
				{
					m_pTransformCom->Move_PosY(fSize.y - Dir.y);
					if (_bOnTerrain)
					{
						*_bOnTerrain = true;
					}
				}
			}
			else if (fRadian * fRadius < fSize.y && fRadian * fRadius >= -fSize.y)
			{
				float fr{ fSize.x * fSize.x + fSize.z * fSize.z };
				if (LengthSq < fr)
				{
					m_pTransformCom->Move_Pos(vNormalDir * (sqrt(fr) - sqrt(LengthSq)));
					if (_bOnWall)
					{
						*_bOnWall = true;
					}
				}
			}
			else
			{
				m_pTransformCom->Move_Pos(vNormalDir * fDistance);
			}
		}
	}
}

void Fragment::Free()
{
	GameObject::Free();
}

