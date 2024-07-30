#include "..\..\Header\Renderer.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(Renderer)

Renderer::Renderer()
{
	
}

Renderer::~Renderer()
{
	Free();
}

void Renderer::Add_RenderGroup(RENDERID eType, GameObject * pGameObject)
{
	if (RENDER_END <= eType || nullptr == pGameObject)
		return;

	m_RenderGroup[eType].push_back(pGameObject);
	pGameObject->AddRef();
}

void Renderer::Render_GameObject(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Render_Priority(pGraphicDev);
	Render_Nonalpha(pGraphicDev);
	Engine::PSMgr::GetInstance()->RenderPSMgr(pGraphicDev);
	Render_Alpha(pGraphicDev);

	if (!SingleCamera::GetInstance()->IsPlaying())
	{
		Render_UI(pGraphicDev);
	}

	Clear_RenderGroup();

}

void Renderer::Clear_RenderGroup()
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}

}

void Renderer::Render_Priority(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void Renderer::Render_Nonalpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_GameObject();
}

void Renderer::Render_Alpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//D3DRS_SRCBLEND : 첫 번째 이미지, D3DBLEND_SRCALPHA : 첫 번째 이미지의 알파 값
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//D3DRS_DESTBLEND: 두 번째 이미지, D3DBLEND_INVSRCALPHA : 첫 번째 이미지 알파 값의 역원(1 - SRCALPHA)
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_RenderGroup[RENDER_ALPHA].sort([](GameObject* pDst, GameObject* pSrc)->bool { return pDst->Get_ViewZ() > pSrc->Get_ViewZ();  });

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_GameObject();	
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

void Renderer::Render_UI(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	D3DXMATRIX matIdentity, matProj, matPreView, matPreProj;

	pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	D3DXMatrixIdentity(&matIdentity);

	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	pGraphicDev->SetTransform(D3DTS_WORLD, &matIdentity);
	pGraphicDev->SetTransform(D3DTS_VIEW, &matIdentity);

	D3DXMatrixOrthoLH(&matProj, 800.0f, 600.0f, 0.0f, 1.0f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	// Test
	m_orthoProj = matProj;

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	
	//D3DRS_SRCBLEND : 첫 번째 이미지, D3DBLEND_SRCALPHA : 첫 번째 이미지의 알파 값
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//D3DRS_DESTBLEND: 두 번째 이미지, D3DBLEND_INVSRCALPHA : 첫 번째 이미지 알파 값의 역원(1 - SRCALPHA)
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
	Engine::UIManager::GetInstance()->Render_UI();

	for (auto& iter : m_RenderGroup[RENDER_UI])
	{
		iter->Render_GameObject();
	}

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);

	pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);
}

void Renderer::Free()
{
	Clear_RenderGroup();
}
