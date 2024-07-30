#include "stdafx.h"
#include "..\Header\BackGround.h"

#include "Export_System.h"
#include "Export_Utility.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::GameObject(pGraphicDev)
{
}

CBackGround::CBackGround(const CBackGround & rhs)
	: Engine::GameObject(rhs)
{
}

CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Add_Component()
{
	Component*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<RcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_LogoTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LogoTexture", pComponent });

	pComponent = m_pLogoTitle_TextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_LogoTitleTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LogoTitleTexture", pComponent });

	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGround *		pInstance = new CBackGround(pGraphicDev);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BackGround Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();
}

HRESULT CBackGround::Init_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CBackGround::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);

	return __super::Update_GameObject(fTimeDelta);
}

void CBackGround::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBackGround::Render_GameObject()
{
	D3DXMATRIX matWorld, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixOrthoLH(&matProj, 2.0f, 2.0f, 0.0f, 1.0f);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer(); 
	
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//D3DRS_SRCBLEND : 첫 번째 이미지, D3DBLEND_SRCALPHA : 첫 번째 이미지의 알파 값
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//D3DRS_DESTBLEND: 두 번째 이미지, D3DBLEND_INVSRCALPHA : 첫 번째 이미지 알파 값의 역원(1 - SRCALPHA)
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	matWorld._11 *= 1.04f;
	matWorld._22 *= 0.93f;
	matWorld._42 += 0.3f;
	D3DXMatrixIdentity(&matView);
	D3DXMatrixOrthoLH(&matProj, 3.0f, 3.0f, 0.0f, 1.0f);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	m_pLogoTitle_TextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	if(m_bIs_FinishedLoading == false)
	{
		
		Engine::Render_Font(L"Font_LogoText", L"게임 환경을 설정하고 있습니다.", &_vec4(0.f, 498.f, WINCX, 522.f), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Engine::Render_Font(L"Font_LogoText", L"잠시만 기다려주세요.", &_vec4(0.f, 522.f, WINCX, 546.f), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
}