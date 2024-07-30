#include "stdafx.h"
#include "Magic.h"
#include "Export_System.h"
#include "Export_Utility.h"

Magic::Magic(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: Effect(_pGraphicDevice)
{
}

Magic::~Magic()
{
}

Magic* Magic::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const MAGIC& _eMAGIC, const MAGIC_EX& _eMAGIC_EX, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const float& _fSpeed, const int& _iDamage, const D3DCOLOR& _dwColor)
{
	Magic* pInstance = new Magic(_pGraphicDevice);

	if (FAILED(pInstance->Init_Magic(_eMAGIC, _eMAGIC_EX, _vPos, _vDir, _fSpeed, _iDamage, _dwColor)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_Magic Failed", L"Magic::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT Magic::Init_Magic(const MAGIC& _eMAGIC, const MAGIC_EX& _eMAGIC_EX, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, const float& _fSpeed, const int& _iDamage, const D3DCOLOR& _dwColor)
{
	if (FAILED(Effect::Init_Effect()))
	{
		return E_FAIL;
	}

	m_eType = _eMAGIC;
	m_eEXType = _eMAGIC_EX;

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	m_pTransformCom->Set_Pos(_vPos);
	m_vDir = _vDir;
	m_fSpeed = _fSpeed;
	m_iDamage = _iDamage;
	m_dwColor = _dwColor;
	m_pColliderCom->SetThisPointer(this);
	m_pColliderCom->SetDamage(m_iDamage);

	SettingMagic();

	m_pParticles = PTrace::Create(m_pGraphicDev);
	if (nullptr == m_pParticles) { return E_FAIL; }
	PSMgr::GetInstance()->AddParticles(m_pParticles);
	m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), m_dwColor);

	SoundMgr::GetInstance()->Play_Sound_NoInterval(L"mg_fwoosh", SOUND_EFFECT_2, 0.4f);

	return S_OK;
}

int Magic::Update_GameObject(const _float& _fTimeDelta)
{
	if (m_pColliderCom->GetColType() & COL_ENVIRONMENT || m_pColliderCom->GetColOBB())
	{
		return -1; 
	}

	if (Effect::Update_GameObject(_fTimeDelta) & 0x80000000) { return -1; }

	if (m_eType == MAGIC_0 && m_fSpeed > 5.0f)
	{
		m_pAniTexCom->Update_Component(_fTimeDelta);
	}

	m_fParticleGenerateTime += _fTimeDelta;
	if (m_fParticleGenerateTime > 0.05f)
	{
		m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), m_dwColor);
		m_fParticleGenerateTime = 0.0f;
	}

	m_pTransformCom->Move_Pos(D3DXVECTOR3{ m_vDir * m_fSpeed * _fTimeDelta });

	GameObject::Update_GameObject(_fTimeDelta);
	m_pColliderCom->ReBuild();
	m_pColliderCom->ClearInfo();
	Engine::AddCollider(m_pColliderCom);

	return 0;
}

void Magic::LateUpdate_GameObject()
{
	if (m_pColliderCom->GetColType() & COL_ENVIRONMENT || m_pColliderCom->GetColOBB())
	{
		CreateMagicEx();
	}
	Effect::LateUpdate_GameObject();
}

void Magic::Render_GameObject()
{
	ComputeBillBoard();
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, m_dwColor);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pAniTexCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

}

void Magic::SetColType(COLLAYER _eType)
{
	if (nullptr != m_pColliderCom)
	{
		m_pColliderCom->SetColLayer(_eType);
	}
}

HRESULT Magic::Add_Component()
{
	m_pColliderCom = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
	if (!m_pColliderCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_Collider",m_pColliderCom });
	m_pColliderCom->SetTransform(m_pTransformCom);

	switch (m_eType)
	{
	case MAGIC_0:
	{
		m_pAniTexCom = dynamic_cast<AnimationTexture*>(Engine::Clone_Proto(L"Proto_Magic0"));
		if (!m_pAniTexCom) { return E_FAIL; }
		m_mapComponent[ID_STATIC].insert({ L"Proto_Magic",m_pAniTexCom });
	}
	break;
	case MAGIC_1:
	{
		m_pAniTexCom = dynamic_cast<AnimationTexture*>(Engine::Clone_Proto(L"Proto_Magic1"));
		if (!m_pAniTexCom) { return E_FAIL; }
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Magic",m_pAniTexCom });
	}
	break;
	}

	return S_OK;
}

void Magic::CreateMagicEx()
{
	Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->Add_GameObject(L"MagicEx", MagicEx::Create(m_pGraphicDev, m_eEXType, m_pTransformCom->Get_Info(INFO_POS), m_iEXDamage, m_dwColor));
}

void Magic::SettingMagic()
{
	switch (m_eType)
	{
	case MAGIC_0:
	{
		m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);
		m_pColliderCom->SetSize(0.18f, 0.18f, 0.18f);
		m_pAniTexCom->SetFrameSpeed(30.0f);
		
	}
	break;
	case MAGIC_1:
	{
		m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);
		m_pColliderCom->SetSize(0.18f, 0.18f, 0.18f);
		m_pAniTexCom->SetFrameSpeed(30.0f);
	}
	break;
	}
}

void Magic::Free()
{
	GameObject::Free();
}
