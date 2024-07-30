#include "stdafx.h"
#include "..\Header\Logo.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "Villeage.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage_Boss.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::Scene(pGraphicDev)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Init_Logo(GameObject** _ppPlayer)
{
	FAILED_CHECK_RETURN(Init_Prototype(), E_FAIL);

	m_pLoadingImg = CBackGround::Create(m_pGraphicDev);
	if (nullptr == m_pLoadingImg) { return E_FAIL; }
	Engine::Management::GetInstance()->SetLoadingImg(m_pLoadingImg);

	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	m_ppPlayer = _ppPlayer;

	Engine::Play_BGM(L"01_call_of_adventure", 1.f);

	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			Engine::Stop_All();
			FAILED_CHECK_RETURN(Engine::Set_Scene(Villeage::Create(m_pGraphicDev, m_ppPlayer)), E_FAIL);
			//FAILED_CHECK_RETURN(Engine::Set_Scene(Stage1::Create(m_pGraphicDev, m_ppPlayer)), E_FAIL);
			//FAILED_CHECK_RETURN(Engine::Set_Scene(Stage2::Create(m_pGraphicDev, m_ppPlayer)), E_FAIL);
			//FAILED_CHECK_RETURN(Engine::Set_Scene(Stage_Boss::Create(m_pGraphicDev, m_ppPlayer)), E_FAIL);
			return 0;
		}		
		dynamic_cast<CBackGround*>(m_pLoadingImg)->Set_Is_FinishedLoading(true);
	}
	else
	{
		dynamic_cast<CBackGround*>(m_pLoadingImg)->Set_Is_FinishedLoading(false);
	}

	return iExit;
}

void CLogo::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CLogo::Render_Scene()
{
	
	m_pLoadingImg->Render_GameObject();
	Engine::Render_Font(L"Font_LogoText", m_pLoading->Get_String(), &_vec4(0.f, 510.f, WINCX, 534.f), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

void CLogo::CheckLoading()
{
	if (m_pLoading)
	{
		m_pLoading->Get_Finish() ? m_bLoading = true : m_bLoading = false;
	}
}

HRESULT CLogo::Init_Prototype()
{	
	FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_RcTex", RcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_LogoTexture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Logo/Delver-Menu-BG.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_LogoTitleTexture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Logo/Delver-Logo.png")), E_FAIL);
	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev, GameObject** _ppPlayer)
{
	CLogo* pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Init_Logo(_ppPlayer)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLogo::Free()
{	
	Safe_Release(m_pLoading);
	Scene::Free();
}
