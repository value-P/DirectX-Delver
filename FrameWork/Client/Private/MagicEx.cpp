#include "stdafx.h"
#include "MagicEx.h"
#include "Export_System.h"
#include "Export_Utility.h"

MagicEx::MagicEx(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: Effect(_pGraphicDevice)
{
}

MagicEx::~MagicEx()
{
}

MagicEx* MagicEx::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const MAGIC_EX& _eMAGIC_EX, const D3DXVECTOR3& _vPos, const int& _iDamage, const D3DCOLOR& _dwColor)
{
	MagicEx* pInstance = new MagicEx(_pGraphicDevice);

	if (FAILED(pInstance->Init_MagicEx(_eMAGIC_EX, _vPos, _iDamage, _dwColor)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_MagicEx Failed", L"MagicEx::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT MagicEx::Init_MagicEx(const MAGIC_EX& _eMAGIC_EX, const D3DXVECTOR3& _vPos, const int& _iDamage, const D3DCOLOR& _dwColor)
{
	if (FAILED(Effect::Init_Effect()))
	{
		return E_FAIL;
	}

	m_eEXType = _eMAGIC_EX;
	m_iDamage = _iDamage;

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	m_pTransformCom->Set_Pos(_vPos);
	m_dwColor = _dwColor;

	SettingMagicEX();

	if (m_eEXType == MAGIC_EX_0)
	{
		SoundMgr::GetInstance()->Play_Sound_NoInterval(L"mg_ice_impact_01", SOUND_EFFECT_2, 0.4f);
	}

	if (m_eEXType == MAGIC_EX_1)
	{
		SoundMgr::GetInstance()->Play_Sound_NoInterval(L"mg_green_impact_03", SOUND_EFFECT_2, 0.4f);
	}

	if (m_eEXType == MAGIC_EX_2)
	{
		SoundMgr::GetInstance()->Play_Sound_NoInterval(L"explode", SOUND_EFFECT_2, 0.4f);
	}

	return S_OK;
}

int MagicEx::Update_GameObject(const _float& _fTimeDelta)
{
	GameObject::Update_GameObject(_fTimeDelta);
	if (1 & m_pAniTexCom->GetRepeatCnt()) { return -1; }

	if (m_eEXType == MAGIC_EX_2 && !m_bBoom)
	{
		m_pColliderCom->ReBuild();
		Engine::AddCollider(m_pColliderCom);
		m_bBoom = true;
	}

	return 0;
}

void MagicEx::LateUpdate_GameObject()
{
	Effect::LateUpdate_GameObject();
}

void MagicEx::Render_GameObject()
{
	ComputeBillBoard();

	if (m_eEXType == MAGIC_EX_2)
	{
		m_pAniTexCom->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
	else
	{
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
}

HRESULT MagicEx::Add_Component()
{
	switch (m_eEXType)
	{
	case MAGIC_EX_0:
	{
		m_pAniTexCom = dynamic_cast<AnimationTexture*>(Engine::Clone_Proto(L"Proto_Magic_Ex_0"));
		if (nullptr == m_pAniTexCom) { return E_FAIL; }
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Magic_Ex", m_pAniTexCom });
	}
	break;
	case MAGIC_EX_1:
	{
		m_pAniTexCom = dynamic_cast<AnimationTexture*>(Engine::Clone_Proto(L"Proto_Magic_Ex_1"));
		if (nullptr == m_pAniTexCom) { return E_FAIL; }
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Magic_Ex", m_pAniTexCom });
	}
	break;
	case MAGIC_EX_2:
	{
		m_pColliderCom = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
		if (!m_pColliderCom) { return E_FAIL; }
		m_mapComponent[ID_STATIC].insert({ L"Proto_Collider",m_pColliderCom });
		m_pColliderCom->SetTransform(m_pTransformCom);

		m_pAniTexCom = dynamic_cast<AnimationTexture*>(Engine::Clone_Proto(L"Proto_Magic_Ex_2"));
		if (nullptr == m_pAniTexCom) { return E_FAIL; }
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Magic_Ex", m_pAniTexCom });
	}
	break;
	}
	
	return S_OK;
}

void MagicEx::SettingMagicEX()
{
	switch (m_eEXType)
	{
	case MAGIC_EX_0:
	{
		m_pAniTexCom->SetFrameSpeed(12.0f);
	}
	break;
	case MAGIC_EX_1:
	{
		m_pAniTexCom->SetFrameSpeed(12.0f);
	}
	break;
	case MAGIC_EX_2:
	{
		m_pTransformCom->Set_Scale(1.5f, 1.5f, 1.5f);
		m_pAniTexCom->SetFrameSpeed(12.0f);
		m_pColliderCom->SetColLayer(COL_EXPLOSION);
		m_pColliderCom->SetSize(D3DXVECTOR3{ 1.5f,1.5f,1.5f });
		m_pColliderCom->SetThisPointer(this);
		m_pColliderCom->SetDamage(m_iDamage);
	}
	break;
	}
}

void MagicEx::Free()
{
	GameObject::Free();
}
