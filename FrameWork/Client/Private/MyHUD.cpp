#include "stdafx.h"
//#include "Engine_Define.h"
#include "MyHUD.h"
#include "HUD_QuickSlot.h"

#include "InventoryBase.h"
#include "QuickSlot.h"
#include "ItemBase.h"
#include "HUD_HpBar.h"
#include "DecorIMG.h"
#include "HUD_Hit_Effect.h"

MyHUD::MyHUD(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

MyHUD::~MyHUD()
{
}

// MyHUD* MyHUD::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
MyHUD* MyHUD::Create(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	MyHUD* pInstance = new MyHUD(_pGraphicDevice);

	if (FAILED(pInstance->Init_UI()))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleParent::Create", MB_OK);
	}

	return pInstance;
}



HRESULT MyHUD::Init_UI()
{
	m_arrQuickSlot.resize(5, nullptr);

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	if (FAILED(Add_UI()))
	{
		return E_FAIL;
	}

	m_matBG._11 = 400.0f;
	m_matBG._22 = 300.0f;

	return S_OK;
}

_int MyHUD::Update_UI(const _float& fTimeDelta)
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::Update_UI(fTimeDelta);
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
	QuickSlot* pQuickSlot = dynamic_cast<QuickSlot*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerQuickSlot", ID_DYNAMIC));
	
	if (pQuickSlot)
	{
		const vector<ItemBase*> itemVec = pQuickSlot->GetItemVec();

		for (size_t i = 0; i < m_arrQuickSlot.size(); ++i)
		{
			if (itemVec[i])
			{
				Texture* texture = itemVec[i]->Get_Texture();
				if (texture)
				{
					m_arrQuickSlot[i]->Set_TextureItem(texture);
				}
			}
			else
				m_arrQuickSlot[i]->Set_TextureItem(nullptr);
		}
	}

	if (m_bClose_CrossCursor == false)
	{
		if (UIManager::GetInstance()->GetHUD() == UIManager::GetInstance()->GetCurUI())
		{
			(*m_matCrossCursor)._11 = m_fSizeX_OriginCrossCursor;
			(*m_matCrossCursor)._22 = m_fSizeY_OriginCrossCursor;
		}
		else
		{
			(*m_matCrossCursor)._11 = 0.f;
			(*m_matCrossCursor)._22 = 0.f;
		}
	}
	else
	{
		(*m_matCrossCursor)._11 = 0.f;
		(*m_matCrossCursor)._22 = 0.f;
	}
	return 0;
}

void MyHUD::LateUpdate_UI()
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::LateUpdate_UI();
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
}

void MyHUD::Render_UI()
{
	if (!UIManager::GetInstance()->GetPlayerHittable())
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matBG);
		m_pTextureBG->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
	for_each(m_vecChildUI.begin(), m_vecChildUI.end(), [](UI* Temp)->void {Temp->Render_Icon(); });
}

void MyHUD::Update_Icon(const _float& fTimeDelta)
{
	// 이 클래스가 아이콘으로 표현될때 업데이트 해줘야할게 있다면 여기에
	
}

void MyHUD::LateUpdate_Icon()
{
	// 이 클래스가 아이콘으로 표현될때 업데이트 해줘야할게 있다면 여기에
}

void MyHUD::Render_Icon()
{
	UI::Render_Icon();
}

void MyHUD::OnMouseClicked(_uint index)
{
	// 이 클래스가 부모로서 표현될때 클릭했을때 불릴게 있다면 여기에
	UI::OnMouseClicked(index);
}

void MyHUD::OnIconClicked(_uint index)
{
	// 이 클래스가 아이콘으로 표현될때 클릭했을때 불릴게 있다면 여기에
}

void MyHUD::OnIconClicking(_uint index)
{
}

HRESULT MyHUD::Add_Component()
{
	m_pTextureBG = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_PlayerHit"));
	if (nullptr == m_pTextureBG) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) { return E_FAIL; }
	return S_OK;
}

HRESULT MyHUD::Add_UI()
{
	UI* pUI_InputCheck = nullptr;

	/*pUI_InputCheck = HUD_Hit_Effect::Create(m_pGraphicDev, (float)WINCX * 0.5f, (float)WINCY * 0.5f, 1, 1);
	if (nullptr == pUI_InputCheck)
	{
		return E_FAIL;
	}
	Add_ChildUI(pUI_InputCheck);*/


	//float fRange_HUD_QuickSlotUI = (sizeof(m_arrQuickSlot) / sizeof(UI*));


	float fRange_HUD_QuickSlotUI = 5.f;

	float fFirstSlot_X = (float(WINCX) * 0.5f) - (fRange_HUD_QuickSlotUI * 0.5f * float(INVENTORY_SIZE));

	for (int i = 0; i < int(fRange_HUD_QuickSlotUI); i++)
	{
		m_arrQuickSlot[i] = HUD_QuickSlot::Create(m_pGraphicDev, fFirstSlot_X + (i * INVENTORY_SIZE), 10.0f, INVENTORY_SIZE, INVENTORY_SIZE);
		if (nullptr == m_arrQuickSlot[i])
		{
			return E_FAIL;
		}
		Add_ChildUI(m_arrQuickSlot[i]);
	}

	// [우채] : 그룹별 자식 UI그룹을 보관.(Picking 관련 도움.)
	Add_ChildUI_Group(&m_arrQuickSlot);

	
	
	pUI_InputCheck = HUD_HpBar::Create(m_pGraphicDev, 23, 578, 84, 9);
	if (nullptr == pUI_InputCheck)
	{
		return E_FAIL;
	}
	Add_ChildUI(pUI_InputCheck);
	


	pUI_InputCheck = DecorIMG::Create(m_pGraphicDev, L"Proto_HpBar_Frame_Texture", 10, 570, 100, 24);
	if (nullptr == pUI_InputCheck)
	{
		return E_FAIL;
	}
	Add_ChildUI(pUI_InputCheck);

	pUI_InputCheck = DecorIMG::Create(m_pGraphicDev, L"Proto_CrossCursor_Texture", (WINCX * 0.5) - 16, (WINCY * 0.5) -16, 32, 32);
	if (nullptr == pUI_InputCheck)
	{
		return E_FAIL;
	}
	Add_ChildUI(pUI_InputCheck);
	m_matCrossCursor = pUI_InputCheck->Get_MatIconWorld();
	m_fSizeX_OriginCrossCursor = (*m_matCrossCursor)._11;
	m_fSizeY_OriginCrossCursor = (*m_matCrossCursor)._22;


	return S_OK;

}

void MyHUD::Free()
{
	UI::Free();
}
