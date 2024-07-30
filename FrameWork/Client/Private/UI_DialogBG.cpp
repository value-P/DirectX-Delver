#include "stdafx.h"
#include "UI_DialogBG.h"
#include "DecorIMG.h"
#include "NonPlayerCharacter.h"

UI_DialogBG::UI_DialogBG(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

UI_DialogBG::~UI_DialogBG()
{
}

UI_DialogBG* UI_DialogBG::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, NonPlayerCharacter* _pNPC, vector<vector<wstring>> _pNPCDialogVec, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	UI_DialogBG* pInstance = new UI_DialogBG(_pGraphicDevice);

	if (FAILED(pInstance->Init_UI(_pNPCDialogVec, _pNPC, _fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleParent::Create", MB_OK);
	}

	return pInstance;
}

HRESULT UI_DialogBG::Init_UI(vector<vector<wstring>> _pNPCDialogVec, NonPlayerCharacter* _pNPC, const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
{
	UI::Init_UI(_fX, _fY, _fSizeX, _fSizeY);

	m_eUIReturn_Type = URT_DELETE;

	m_pNPC = _pNPC;
	m_pNPCDialogVec = _pNPCDialogVec;

	if (FAILED(Add_UI()))
	{
		return E_FAIL;
	}
	return S_OK;
}

_int UI_DialogBG::Update_UI(const _float& fTimeDelta)
{
	m_fFrameContinue_Icon += fTimeDelta;

	if (m_fFrameContinue_Icon >= 0.4f)
		m_fFrameContinue_Icon = 0.f;

	if (m_pMatContinue_Icon && m_fFrameContinue_Icon == 0.f)
	{
		if (m_bIsDownIcon)
		{
			(*m_pMatContinue_Icon)._42 += 1.f;
			m_bIsDownIcon = false;
		}
		else
		{
			(*m_pMatContinue_Icon)._42 += -1.f;
			m_bIsDownIcon = true;
		}
	}
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::Update_UI(fTimeDelta);
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
	if (!(UIManager::GetInstance()->Key_Lock_UI(L"UI_DialogBG")))
	{
		Key_Input(fTimeDelta);
	}
	return 0;
}

void UI_DialogBG::LateUpdate_UI()
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::LateUpdate_UI();
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
}

void UI_DialogBG::Render_UI()
{
	m_pNPCDialogVec;

	for_each(m_vecChildUI.begin(), m_vecChildUI.end(), [](UI* Temp)->void {Temp->Render_Icon(); });
	
	float fFirstDialog_Line_Y = (float(WINCY) * 0.5f) - (float(iCurNPCDialog_Line) * float(SHOPITEM_SIZE) * 0.5f);
	
	for (int i = 0; i < iCurNPCDialog_Line; i++)
	{

		Engine::Render_Font(L"Font_RoundShape", strCurNPCDialogVec[i], &_vec4(232, fFirstDialog_Line_Y + (SHOPITEM_SIZE * i), 567, fFirstDialog_Line_Y + (SHOPITEM_SIZE * (i + 1))), DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}

}

void UI_DialogBG::Update_Icon(const _float& fTimeDelta)
{
}

void UI_DialogBG::LateUpdate_Icon()
{
}

void UI_DialogBG::Render_Icon()
{
	UI::Render_Icon();
}

void UI_DialogBG::OnMouseClicked(_uint index)
{
	UI::OnMouseClicked(index);
}

void UI_DialogBG::OnIconClicked(_uint index)
{
	// 이 클래스가 아이콘으로 표현될때 클릭했을때 불릴게 있다면 여기에
}

void UI_DialogBG::OnIconClicking(_uint index)
{

}

HRESULT UI_DialogBG::Add_Component()
{
	return S_OK;
}

HRESULT UI_DialogBG::Add_UI()
{
	

	if ((size_t)Idx_CurNPCDialog >= m_pNPCDialogVec.size())
	{
		m_pNPC->Set_IsDialogClosed(true);

 		UIManager::GetInstance()->Return_UI();
		
		UIManager::GetInstance()->SelectDelete_UI(L"UI_DialogBG");
		
		
		return S_OK;
	}

	strCurNPCDialogVec = m_pNPCDialogVec[Idx_CurNPCDialog];
	
	iCurNPCDialog_Line = strCurNPCDialogVec.size();

	UI* pUI_InputCheck = nullptr;
	wstring strWindowProtoKey = L"Proto_Window_" + to_wstring(iCurNPCDialog_Line - 1) + L"_Texture";
	pUI_InputCheck = DecorIMG::Create(m_pGraphicDev, strWindowProtoKey, 144, 44, 512, 512);
	if (nullptr == pUI_InputCheck)
	{
		return E_FAIL;
	}
	Add_ChildUI(pUI_InputCheck);
	
	float fLastDialog_Line_Y = (float(WINCY) * 0.5f) + (float(iCurNPCDialog_Line) * float(SHOPITEM_SIZE) * 0.5f);

	pUI_InputCheck = DecorIMG::Create(m_pGraphicDev, L"Proto_Continue_Texture", 384, fLastDialog_Line_Y + 2, 32, 32);
	if (nullptr == pUI_InputCheck)
	{
		return E_FAIL;
	}
	Add_ChildUI(pUI_InputCheck);
	m_pMatContinue_Icon = pUI_InputCheck->Get_MatIconWorld();
	return S_OK;
}



void UI_DialogBG::Free()
{
	UI::Free();
}

void UI_DialogBG::Key_Input(const _float& fTimeDelta)
{
	if (Engine::Key_Down(VK_RETURN))
	{
		Next_Dialog();
	}
}

void UI_DialogBG::Next_Dialog()
{
	for_each(m_vecChildUI.begin(), m_vecChildUI.end(), [](UI* Temp)->void {Safe_Release(Temp); });
	m_vecChildUI.clear();

		++Idx_CurNPCDialog;
	Add_UI();
}
