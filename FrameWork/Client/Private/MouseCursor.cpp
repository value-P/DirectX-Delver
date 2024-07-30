#include "stdafx.h"
#include "MouseCursor.h"
#include "Transform.h"
#include "ItemBase.h"
#include "Weapon.h"
#include "MouseSlot.h"

#include "Export_Utility.h"

MouseCursor::MouseCursor(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

MouseCursor::MouseCursor(const MouseCursor& rhs)
	: Super(rhs)
{
}

MouseCursor::~MouseCursor()
{
}

HRESULT MouseCursor::Init_GameObject()
{
	Super::Init_GameObject();
	
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int MouseCursor::Update_GameObject(const _float& fTimeDelta)
{
	_int	iExit = Super::Update_GameObject(fTimeDelta);

	UpdatePos();
	Add_RenderGroup(RENDER_UI, this);

	return iExit;
}

void MouseCursor::LateUpdate_GameObject()
{
	Super::LateUpdate_GameObject();
}

void MouseCursor::Render_GameObject()
{
	Super::Render_GameObject();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	
	
	if (m_bIs_GetItem)
	{
		if(m_bIsShowCursor == true)
		{
		m_bIsShowCursor = false;
		}
		
	}
	else if (!m_bIs_GetItem)
	{
		if (m_bIsShowCursor == false)
		{
			m_bIsShowCursor = true;
		}
		
	}
	
	if (!m_pShowTextureCom->IsVecTextureEmpty())
	{
		m_pShowTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();
	}
}

MouseCursor* MouseCursor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	MouseCursor* pInstance = new MouseCursor(pGraphicDev);
	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MouseCursor Create Failed");
		
		return nullptr;
	}

	return pInstance;
}

HRESULT MouseCursor::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<RcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_MouseCursor_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_MouseCursor_Texture", pComponent });

	pComponent = m_pEmptyCursorTextureCom = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_EmptyCursor_Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_EmptyCursor_Texture", pComponent });

	pComponent = m_pMouseSlot = dynamic_cast<MouseSlot*>(Engine::Clone_Proto(L"Proto_MouseSlot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_MouseSlot", pComponent });

	return S_OK;
}

void MouseCursor::UpdatePos()
{
	POINT mousePt;
	::GetCursorPos(&mousePt);
	::ScreenToClient(g_hWnd, &mousePt);

	_matrix matOrthoProj;
	D3DXMatrixOrthoLH(&matOrthoProj, 800.f, 600.f, 0.0f, 1.0f);

	D3DVIEWPORT9 ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);

	_matrix Proj = matOrthoProj;

	// 뷰포트 변환 -> 투영 변환
	_vec3 vMouse;
	vMouse.x = (static_cast<float>(mousePt.x) / (ViewPort.Width * 0.5f)) - 1.f;
	vMouse.y = (static_cast<float>(mousePt.y) / -(ViewPort.Height * 0.5f)) + 1.f;
	vMouse.z = 0.f;

	// 투영 -> 뷰스페이스
	D3DXMatrixInverse(&Proj, 0, &Proj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &Proj);

	//_vec3 vTemp = { Proj._11, Proj._22, 0.f };
	m_pTransformCom->Set_Info(INFO_POS, &vMouse);
	m_pTransformCom->Set_Scale(20.f, 20.f, 1.f);
	if (m_pMouseSlot->IsSlotEmpty() == false)
	{
		const vector<ItemBase*>& itemVec = m_pMouseSlot->GetItemVec();
		if (itemVec[0])
		{
			m_pShowTextureCom = itemVec[0]->Get_Texture();
			m_bIs_GetItem = true;
			return;
		}
	}

	m_pShowTextureCom = m_pTextureCom;

	if (UIManager::GetInstance()->GetHUD() == UIManager::GetInstance()->GetCurUI())
		m_pShowTextureCom = m_pEmptyCursorTextureCom;

	m_bIs_GetItem = false;
}

void MouseCursor::OnCollided()
{
}

void MouseCursor::Free()
{
	Super::Free();
}
