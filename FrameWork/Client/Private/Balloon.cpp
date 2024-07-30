#include "stdafx.h"
#include "Balloon.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Effect.h"
#include "MoveWall.h"

Balloon::Balloon(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: EnvironmentBase(_pGraphicDevice)
{
}

Balloon::~Balloon()
{
}

Balloon* Balloon::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const float& _fSpeed, const D3DCOLOR& _dwColor, const D3DXVECTOR3& _vMinDot, const D3DXVECTOR3& _vMaxDot, GameObject* _pObject)
{
	Balloon* pInstance = new Balloon(_pGraphicDevice);

	if (FAILED(pInstance->Init_Balloon(_vPos, _vDir, _fSpeed, _dwColor, _vMinDot, _vMaxDot, _pObject)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_Balloon Failed", L"Balloon::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT Balloon::Init_Balloon(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const float& _fSpeed, const D3DCOLOR& _dwColor, const D3DXVECTOR3& _vMinDot, const D3DXVECTOR3& _vMaxDot, GameObject* _pObject)
{
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	m_pTransformCom->Set_Pos(_vPos);
	m_vDir = _vDir;
	m_fSpeed = _fSpeed;
	m_dwColor = _dwColor;
	m_vMinDot = _vMinDot;
	m_vMaxDot = _vMaxDot;

	if (_pObject)
	{
		m_pGenerateObj = _pObject;
	}

	m_pTransformCom->Set_Scale(0.8f, 0.8f, 0.8f);
	m_pColliderCom->SetSize(0.5f, 0.5f, 0.5f);
	m_pColliderCom->SetColLayer(COL_ENVIRONMENT);
	m_pColliderCom->SetThisPointer(this);

	return S_OK;
}

_int Balloon::Update_GameObject(const float& _fTimeDelta)
{
	if (!m_bAlive) { return -1; }

	D3DXVECTOR3 vPos = m_pTransformCom->Get_Info(INFO_POS);

	if (vPos.x < m_vMinDot.x) { m_pTransformCom->Move_PosX(-vPos.x + m_vMinDot.x); m_vDir.x *= -1.0f; }
	if (vPos.x > m_vMaxDot.x) { m_pTransformCom->Move_PosX(-vPos.x + m_vMaxDot.x); m_vDir.x *= -1.0f; }
	if (vPos.y < m_vMinDot.y) { m_pTransformCom->Move_PosY(-vPos.y + m_vMinDot.y); m_vDir.y *= -1.0f; }
	if (vPos.y > m_vMaxDot.y) { m_pTransformCom->Move_PosY(-vPos.y + m_vMaxDot.y); m_vDir.y *= -1.0f; }
	if (vPos.z < m_vMinDot.z) { m_pTransformCom->Move_PosZ(-vPos.z + m_vMinDot.z); m_vDir.z *= -1.0f; }
	if (vPos.z > m_vMaxDot.z) { m_pTransformCom->Move_PosZ(-vPos.z + m_vMaxDot.z); m_vDir.z *= -1.0f; }

	m_pTransformCom->Move_Pos(D3DXVECTOR3{ m_vDir * _fTimeDelta * m_fSpeed });

	GameObject::Update_GameObject(_fTimeDelta);

	Engine::AddCollider(m_pColliderCom);

	return 0;
}

void Balloon::LateUpdate_GameObject()
{
	if (m_pColliderCom->GetColType() & COL_PLAYER_PROJ)
	{
		list<pair<CINFO, D3DXVECTOR3>>::const_iterator iterBegin = m_pColliderCom->GetColSphere_Info(COL_PLAYER_PROJ).begin();
		list<pair<CINFO, D3DXVECTOR3>>::const_iterator iterEnd = m_pColliderCom->GetColSphere_Info(COL_PLAYER_PROJ).end();
		for (; iterBegin != iterEnd; iterBegin++)
		{
			if (m_dwColor == static_cast<Effect*>((*iterBegin).first.m_pThis)->GetColor())
			{
				PSystem* pPSystem = PSpread::Create(m_pGraphicDev);
				if (nullptr == pPSystem) { return ; }
				PSMgr::GetInstance()->AddParticles(pPSystem);
				for (int i = 0; i < 16; i++)
				{
					pPSystem->AddParticle(m_pTransformCom->Get_Info(INFO_POS), m_dwColor);
				}
				m_bAlive = false;
				if(m_pGenerateObj)
				{
					m_pGenerateObj->AddRef();
					Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_ENVI)->Add_GameObject(L"MoveWall", m_pGenerateObj);
				}
			}
		}
	}

	GameObject::Compute_ViewZ(&(m_pTransformCom->Get_Info(INFO_POS)));
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
}

void Balloon::Render_GameObject()
{
	ComputeBillBoard();
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_dwColor);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

HRESULT Balloon::Add_Component()
{
	m_pBufferCom = dynamic_cast<RcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", m_pBufferCom });

	m_pTransformCom = dynamic_cast<Transform*>(Engine::Clone_Proto(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) { return E_FAIL; }
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", m_pTransformCom });

	m_pColliderCom = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
	if (!m_pColliderCom) { return E_FAIL; }
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Collider",m_pColliderCom });
	m_pColliderCom->SetTransform(m_pTransformCom);

	m_pTextureCom = dynamic_cast<Texture*>(Clone_Proto(L"Proto_Balloon"));
	if (nullptr == m_pTextureCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_Balloon", m_pTextureCom });

	return S_OK;
}

void Balloon::Free()
{
	if (m_pGenerateObj)
	{
		Safe_Release(m_pGenerateObj);
	}
	GameObject::Free();
}
