#include "stdafx.h"
#include "Effect_Bundle.h"
#include "Export_System.h"
#include "Export_Utility.h"

Effect_Bundle::Effect_Bundle(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: Effect(_pGraphicDevice)
{
}

Effect_Bundle::~Effect_Bundle()
{
}

Effect_Bundle* Effect_Bundle::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const EFFECT& _eType,const D3DXVECTOR3& _vPos)
{
	Effect_Bundle* pInstance = new Effect_Bundle(_pGraphicDevice);

	if (FAILED(pInstance->Init_Effect_Bundle(_eType, _vPos)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_Effect_Bundle Failed", L"Effect_Bundle::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT Effect_Bundle::Init_Effect_Bundle(const EFFECT& _eType,const D3DXVECTOR3& _vPos)
{
	if (FAILED(Effect::Init_Effect()))
	{
		return E_FAIL;
	}

	m_eType = _eType;

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	SettingEffect();
	m_pTransformCom->Set_Pos(_vPos);

	return S_OK;
}

int Effect_Bundle::Update_GameObject(const _float& _fTimeDelta)
{
	if (Effect::Update_GameObject(_fTimeDelta) & 0x80000000) { return -1; }
	GameObject::Update_GameObject(_fTimeDelta);
	if (m_eType == EFFECT_0 && 1 & m_pAniTexCom->GetRepeatCnt()) { return -1; }

	return 0;
}

void Effect_Bundle::LateUpdate_GameObject()
{
	Effect::LateUpdate_GameObject();
}

void Effect_Bundle::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	ComputeBillBoard();
	m_pAniTexCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}

HRESULT Effect_Bundle::Add_Component()
{
	switch (m_eType)
	{
	case EFFECT_0:
	{m_pAniTexCom = dynamic_cast<AnimationTexture*>(Engine::Clone_Proto(L"Proto_Effect0"));	}
	break;
	case EFFECT_1:
	{m_pAniTexCom = dynamic_cast<AnimationTexture*>(Engine::Clone_Proto(L"Proto_Effect1")); }
	break;
	case EFFECT_2:
	{m_pAniTexCom = dynamic_cast<AnimationTexture*>(Engine::Clone_Proto(L"Proto_Effect2")); }
	break;
	case EFFECT_3:
	{m_pAniTexCom = dynamic_cast<AnimationTexture*>(Engine::Clone_Proto(L"Proto_Effect3")); }
	break;
	// ´õ ´Ã¸®¸éµÊ
	}
	
	if (nullptr == m_pAniTexCom) { return E_FAIL; }
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Effect",m_pAniTexCom });

	return S_OK;
}

void Effect_Bundle::SettingEffect()
{
	switch (m_eType)
	{
	case EFFECT_0:
	{
		m_pTransformCom->Set_Scale(1.7f, 1.7f, 1.7f);
		m_pAniTexCom->SetFrameSpeed(45.0f);
	}
	break;
	case EFFECT_1:
	{
		m_pTransformCom->Set_Scale(1.7f, 1.7f, 1.7f);
		m_pAniTexCom->SetFrameSpeed(20.0f);
		m_fAliveTime = 2.5f;
	}
	break;
	case EFFECT_2:
	{
		//m_pTransformCom->Set_Scale(0.3f, 0.3f, 0.3f);
		//m_pAniTexCom->SetFrameSpeed(2.0f);
	}
	break;
	}
}

void Effect_Bundle::Free()
{
	GameObject::Free();
}
