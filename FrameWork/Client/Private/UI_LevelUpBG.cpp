#include "stdafx.h"
#include "UI_LevelUpBG.h"
#include "UI_AbilityCard.h"
#include <unordered_set>

UI_LevelUpBG::UI_LevelUpBG(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

UI_LevelUpBG::~UI_LevelUpBG()
{
}

UI_LevelUpBG* UI_LevelUpBG::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	UI_LevelUpBG* pInstance = new UI_LevelUpBG(_pGraphicDevice);

	if (FAILED(pInstance->Init_UI( _fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleParent::Create", MB_OK);
	}

	return pInstance;
}

HRESULT UI_LevelUpBG::Init_UI(const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
{
	UI::Init_UI(_fX, _fY, _fSizeX, _fSizeY);

	m_eUIReturn_Type = URT_DELETE;

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	if (FAILED(Add_UI()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int UI_LevelUpBG::Update_UI(const _float& fTimeDelta)
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::Update_UI(fTimeDelta);
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
	if (!(UIManager::GetInstance()->Key_Lock_UI(L"UI_LevelUpBG")))
	{
		Key_Input(fTimeDelta);
	}
	return 0;
}

void UI_LevelUpBG::LateUpdate_UI()
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::LateUpdate_UI();
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
}

void UI_LevelUpBG::Render_UI()
{
	UI::Render_UI();

	Engine::Render_Font(L"Font_RoundShape_LevelUp_Title", L"Level Up!", &_vec4(0, 98, WINCX, 114), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Engine::Render_Font(L"Font_RoundShape_LevelUp_Text", L"성장시킬 능력치를 선택해주세요.", &_vec4(0, 123, WINCX, 135), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

void UI_LevelUpBG::Update_Icon(const _float& fTimeDelta)
{
}

void UI_LevelUpBG::LateUpdate_Icon()
{
}

void UI_LevelUpBG::Render_Icon()
{
	UI::Render_Icon();
}

void UI_LevelUpBG::OnMouseClicked(_uint index)
{
}

void UI_LevelUpBG::OnIconClicked(_uint index)
{
}

void UI_LevelUpBG::OnIconClicking(_uint index)
{

}

HRESULT UI_LevelUpBG::Add_Component()
{
	Component* pComponent = Clone_Proto(L"Proto_RcTex");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(pComponent);

	pComponent = Clone_Proto(L"Proto_LevelUpBG_Texture");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pTextureBG = dynamic_cast<Texture*>(pComponent);
	return S_OK;
}

HRESULT UI_LevelUpBG::Add_UI()
{

	unordered_set<int> _setUniqueValues;
	_int m_iIdx_AbilityCard = 0;
	AbilityCard_ID _eAbilityCard_ID;

	while (_setUniqueValues.size() < 3)
	{
		_int _iRandomValue = rand() % 6; // 0부터 5까지의 무작위 값 생성
		_setUniqueValues.insert(_iRandomValue); // set에 값을 삽입 (중복은 무시됨)
	}

	for (_int _iRandomValue : _setUniqueValues) 
	{
		_eAbilityCard_ID = AbilityCard_ID(_iRandomValue);

		m_vecAbilityCard.push_back(UI_AbilityCard::Create(m_pGraphicDev, m_iIdx_AbilityCard, _eAbilityCard_ID, 118, 164, 171, 283));
		if (nullptr == m_vecAbilityCard[m_iIdx_AbilityCard])
		{
			return E_FAIL;
		}
		Add_ChildUI(m_vecAbilityCard[m_iIdx_AbilityCard]);
		m_iIdx_AbilityCard++;
	}
	Add_ChildUI_Group(&m_vecAbilityCard);
	

	return S_OK;
}



void UI_LevelUpBG::Free()
{
	UI::Free();
}

void UI_LevelUpBG::Key_Input(const _float& fTimeDelta)
{
}
