#include "stdafx.h"
#include "Arrow.h"
#include "Export_System.h"
#include "Export_Utility.h"

Arrow::Arrow(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: Effect(_pGraphicDevice)
{
}

Arrow::~Arrow()
{
}

Arrow* Arrow::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const float& _fSpeed, const int& _iDamage)
{
	Arrow* pInstance = new Arrow(_pGraphicDevice);

	if (FAILED(pInstance->Init_Arrow(_vPos, _vDir, _fSpeed, _iDamage)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_Arrow Failed", L"Arrow::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT Arrow::Init_Arrow(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const float& _fSpeed, const int& _iDamage)
{
	if (FAILED(Effect::Init_Effect()))
	{
		return E_FAIL;
	}
	
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	m_pTransformCom->Set_Pos(_vPos);
	m_vDir = _vDir;
	m_fSpeed = _fSpeed;
	m_iDamage = _iDamage;

	m_pTransformCom->Set_Scale(0.4f, 0.4f, 0.4f);
	m_pColliderCom->SetSize(0.15f, 0.15f, 0.15f);
	m_pColliderCom->SetThisPointer(this);
	m_pColliderCom->SetDamage(m_iDamage);
	m_fAliveTime = 6.0f;

	m_pParticles = PTrace::Create(m_pGraphicDev);
	if (nullptr == m_pParticles) { return E_FAIL; }
	PSMgr::GetInstance()->AddParticles(m_pParticles);
	m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), m_dwColor);

	SoundMgr::GetInstance()->Play_Sound_NoInterval(L"bow", SOUND_EFFECT_2, 0.6f);

	return S_OK;
}

int Arrow::Update_GameObject(const _float& _fTimeDelta)
{
	if (Effect::Update_GameObject(_fTimeDelta) & 0x80000000) { return -1; }

	if ((m_pColliderCom->GetColType() & COL_ENVIRONMENT) || (m_pColliderCom->GetColOBB() & (COL_MONSTER | COL_PLAYER))) { return -1; }

	if (m_pColliderCom->GetColOBB() & COL_WALL) { return 0; }

	m_fParticleGenerateTime += _fTimeDelta;
	if (m_fParticleGenerateTime > 0.05f)
	{
		m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), m_dwColor);
		m_fParticleGenerateTime = 0.0f;
	}

	m_vDir.y -= 0.12f * _fTimeDelta;

	m_pTransformCom->Move_Pos(D3DXVECTOR3{ m_vDir * m_fSpeed * _fTimeDelta });
	
	D3DXMATRIX matScale, matRot, matRot2;
	D3DXMatrixIdentity(&matRot);
	D3DXVECTOR3 vScale{ m_pTransformCom->Get_Scale() };
	D3DXVECTOR3 vRight{ -m_vDir }, vUp, vLook, vPos{ m_pTransformCom->Get_Info(INFO_POS) };
	
	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXVec3Cross(&vUp, &D3DXVECTOR3{ 0.0f,-1.0f,0.0f }, &vRight);
	D3DXVec3Cross(&vLook, &vRight, &vUp);
	memcpy_s(&matRot.m[0][0], 12, &vRight, 12);
	memcpy_s(&matRot.m[1][0], 12, &vUp, 12);
	memcpy_s(&matRot.m[2][0], 12, &vLook, 12);
	memcpy_s(&matRot.m[3][0], 12, &vPos, 12);
	D3DXMatrixMultiply(&m_matCrossWorld, &matScale, &matRot);
	m_pTransformCom->Set_WorldMatrix(&m_matCrossWorld);
	m_pColliderCom->ReBuild();

	D3DXMatrixRotationAxis(&matRot2, &m_vDir, 1.570796327f);
	memset(&matRot.m[3][0], 0, 12);
	D3DXMatrixMultiply(&matRot, &matRot, &matRot2);
	memcpy_s(&matRot.m[3][0], 12, &vPos, 12);
	D3DXMatrixMultiply(&m_matCrossWorld, &matScale, &matRot);

	m_pColliderCom->ClearInfo();
	Engine::AddCollider(m_pColliderCom);

	return 0;
}

void Arrow::LateUpdate_GameObject()
{
	Effect::LateUpdate_GameObject();
}

void Arrow::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matCrossWorld);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Arrow::SetColType(COLLAYER _eType)
{
	if (nullptr != m_pColliderCom)
	{
		m_pColliderCom->SetColLayer(_eType);
	}
}

HRESULT Arrow::Add_Component()
{
	m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_Arrow"));
	if (!m_pTextureCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_Arrow",m_pTextureCom });

	m_pColliderCom = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
	if (!m_pColliderCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_Collider",m_pColliderCom });
	m_pColliderCom->SetTransform(m_pTransformCom);

	return S_OK;
}

void Arrow::Free()
{
	GameObject::Free();
}
