#include "stdafx.h"
#include "UI_AbilityCard.h"
#include "Creature.h"
#include "Player.h"

UI_AbilityCard::UI_AbilityCard(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

UI_AbilityCard::~UI_AbilityCard()
{
}

UI_AbilityCard* UI_AbilityCard::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, _int _iIdx_AbilityCard, AbilityCard_ID _eAbilityCard_ID, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	UI_AbilityCard* pInstance = new UI_AbilityCard(_pGraphicDevice);

	if (FAILED(pInstance->Init_UI(_iIdx_AbilityCard, _eAbilityCard_ID, _fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleChild::Create", MB_OK);
		return nullptr;
	}
	
	return pInstance;
}

HRESULT UI_AbilityCard::Init_UI(_int _iIdx_AbilityCard, AbilityCard_ID _eAbilityCard_ID, const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
{
	UI::Init_UI(_fX, _fY, _fSizeX, _fSizeY);

	m_eAbilityCard_ID = _eAbilityCard_ID;

	switch (_iIdx_AbilityCard)
	{
	case 0:
		m_fLastDest_X = -196.5;
		break;
	case 1:
		m_fLastDest_X = 0.5f;
		break;
	case 2:
		m_fLastDest_X = 197.5f;
		break;
	}

	switch (m_eAbilityCard_ID)
	{
	case ACI_ATTACK:
		m_strProtoKey = L"Proto_Attack_Texture";
		m_strName_AbilityCard = L"+1 공격력";
		m_strExplain_AbilityCard = L"근거리 무기의 공격력을 증가시킵니다.";
		m_strCurStatTag = L"현재 근거리 무기 공격력 : ";
		
		break;
	case ACI_SPEED:
		m_strProtoKey = L"Proto_Speed_Texture";
		m_strName_AbilityCard = L"+1 이동 속도";
		m_strExplain_AbilityCard = L"이동 속도를 증가시킵니다.";
		m_strCurStatTag = L"현재 이동 속도 : ";
		break;
	case ACI_AGILITY:
		m_strProtoKey = L"Proto_Agility_Texture";
		m_strName_AbilityCard = L"+1 민첩성";
		m_strExplain_AbilityCard = L"원거리 무기의 공격력을 증가시킵니다.";
		m_strCurStatTag = L"현재 원거리 무기 공격력 : ";
		break;
	case ACI_DEFENSE:
		m_strProtoKey = L"Proto_Defense_Texture";
		m_strName_AbilityCard = L"+1 방어도";
		m_strExplain_AbilityCard = L"피격 시 받는 피해량을 감소시킵니다.";
		m_strCurStatTag = L"현재 방어도 : ";
		break;
	case ACI_HEALTH:
		m_strProtoKey = L"Proto_Health_Texture";
		m_strName_AbilityCard = L"+1 체력";
		m_strExplain_AbilityCard = L"최대 체력을 증가시킵니다.";
		m_strCurStatTag = L"현재 최대 체력 : ";
		break;
	case ACI_MAGIC:
		m_strProtoKey = L"Proto_Magic_Texture";
		m_strName_AbilityCard = L"+1 마법 강화";
		m_strExplain_AbilityCard = L"마법봉의 공격력을 증가시킵니다.";
		m_strCurStatTag = L"현재 마법봉 공격력 : ";
		break;
	}

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

_int UI_AbilityCard::Update_UI(const _float& fTimeDelta)
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에

	UI::Update_UI(fTimeDelta);
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
	return 0;
}

void UI_AbilityCard::LateUpdate_UI()
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::LateUpdate_UI();
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
}

void UI_AbilityCard::Render_UI()
{
	// 이 클래스가 아이콘으로 표현될때 업데이트 해줘야할게 있다면 여기에
}

void UI_AbilityCard::Update_Icon(const _float& fTimeDelta)
{
	if (!pPlayer)
		pPlayer = (*(Engine::Get_GameObject(L"GameObject", L"Player"))).front();

	switch (m_eAbilityCard_ID)
	{
	case ACI_ATTACK:
		m_iCurStat = dynamic_cast<Creature*>(pPlayer)->Get_CreatureAttack();
		break;
	case ACI_SPEED:
		m_iCurStat = dynamic_cast<Creature*>(pPlayer)->Get_CreatureMove_Speed();
		break;
	case ACI_AGILITY:
		m_iCurStat = dynamic_cast<Creature*>(pPlayer)->Get_CreatureRangedAttack();
		break;
	case ACI_DEFENSE:
		m_iCurStat = dynamic_cast<Creature*>(pPlayer)->Get_CreatureDefense();
		break;
	case ACI_HEALTH:
		m_iCurStat = dynamic_cast<Creature*>(pPlayer)->Get_CreatureMaxHp();
		break;
	case ACI_MAGIC:
		m_iCurStat = dynamic_cast<Creature*>(pPlayer)->Get_CreatureMagicAttack();
		break;
	}

	// Step_1
	if (m_bIs_X_FirstDest == false && m_bIs_X_LastDest == false)
	{
		if (m_matIcon._41 >= m_fFirstDest_X)
		{
			m_matIcon._41 -= m_fSpeed;
		}
	}

	// Step_2
	if (m_bIs_X_FirstDest == true && m_bIs_X_LastDest == false)
	{
		if (m_matIcon._41 <= m_fLastDest_X)
		{
			m_matIcon._41 += m_fSpeed;
		}
	}

	// Step_3
	if (m_bIs_X_FirstDest == true && m_bIs_X_LastDest == true)
	{
		if (m_bIsClicking == true)
		{
			if (m_matIcon._42 <= m_fDest_Y)
			{
				m_bShow_Explain_AbilityCard = true;
				m_matIcon._42 += m_fSpeed;
				
			}
		}
		else // (m_bIsClicking == false)
		{
			if (m_matIcon._42 >= m_fOrigin_Y)
			{
				m_bShow_Explain_AbilityCard = false;
				m_matIcon._42 -= m_fSpeed;
			}
		}
	}
}

void UI_AbilityCard::LateUpdate_Icon()
{
	// Step_1
	if (m_bIs_X_FirstDest == false && m_bIs_X_LastDest == false)
		if (m_matIcon._41 <= m_fFirstDest_X)
		{
			m_matIcon._41 = m_fFirstDest_X;
			m_bIs_X_FirstDest = true;
		}

	// Step_2
	if (m_bIs_X_FirstDest == true && m_bIs_X_LastDest == false)
	{
		if (m_matIcon._41 >= m_fLastDest_X)
		{
			m_matIcon._41 = m_fLastDest_X;
			m_bIs_X_LastDest = true;
		}
	}

	// Step_3
	if (m_bIs_X_FirstDest == true && m_bIs_X_LastDest == true)
	{
		if (m_bIsClicking == true)
		{
			if (m_matIcon._42 >= m_fDest_Y)
			{
				m_matIcon._42 = m_fDest_Y;
				
			}
		}
		else // (m_bIsClicking == false)
		{
			if (m_matIcon._42 <= m_fOrigin_Y)
			{
				m_matIcon._42 = m_fOrigin_Y;
			}
		}
	}

	m_bIsClicking = false;
}

void UI_AbilityCard::Render_Icon()
{
	UI::Render_Icon();

	_float fCur_fX = m_matIcon._41 - _float(m_fSizeX) * 0.5f + _float(WINCX >> 1);
	_float fCur_fY = -m_matIcon._42 - _float(m_fSizeY) * 0.5f + _float(WINCY >> 1);
	
	Engine::Render_Font(L"Font_RoundShape_Name_AbilityCard", m_strName_AbilityCard, &_vec4(fCur_fX, fCur_fY + 11, fCur_fX + m_fSizeX, fCur_fY + 35), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(0.176470593f, 0.0705882385f, 0.0705882385f, 1.f));
	Engine::Render_Font(L"Font_RoundShape_CurStat_AbilityCard", L"현재 능력치 : " + to_wstring(m_iCurStat), &_vec4(fCur_fX, fCur_fY + 37, fCur_fX + m_fSizeX, fCur_fY + 55), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(0.298039228f, 0.121568628f, 0.121568628f, 1.f));
	
	if (m_bShow_Explain_AbilityCard)
		Engine::Render_Font(L"Font_RoundShape_LevelUp_Text", m_strExplain_AbilityCard, &_vec4(0, 485, WINCX, 498), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

void UI_AbilityCard::OnIconClicking(_uint index)
{
	m_bIsClicking = true;

	if (Engine::Get_DIMouseState(DIM_LB) & 0x80) // 1) 왼쪽 마우스 클릭 상태!
	{
		if (m_bMouse_Lock == false)
		{
			m_bMouse_Lock = true;
		}
		else // ② 현재 마우스 슬롯이 차있다면!
		{
		}
	}

	else if (Engine::Get_DIMouseState(DIM_RB) & 0x80) // 2) 오른쪽 마우스 클릭 상태!
	{
		// 없는 기능입니다.
	}

	else // 3) 마우스 클릭이 없는 상태!
	{
		// 마우스 슬롯 상태 구분

		if (m_bMouse_Lock == true)
		{
			m_bMouse_Lock = false;
			_int iIncrease_Stat = 0;
			switch (m_eAbilityCard_ID)
			{
			case ACI_ATTACK:
				dynamic_cast<Creature*>(pPlayer)->Set_CreatureAttack(m_iCurStat + 1);
				break;
			case ACI_SPEED:
				dynamic_cast<Creature*>(pPlayer)->Set_CreatureMove_Speed(m_iCurStat + 1);
				break;
			case ACI_AGILITY: 
				dynamic_cast<Creature*>(pPlayer)->Set_CreatureRangedAttack(m_iCurStat + 1);
				break;
			case ACI_DEFENSE:
				dynamic_cast<Creature*>(pPlayer)->Set_CreatureDefense(m_iCurStat + 1);
				break;
			case ACI_HEALTH:
				dynamic_cast<Creature*>(pPlayer)->Set_CreatureMaxHp(m_iCurStat + 1);
				break;
			case ACI_MAGIC:
				dynamic_cast<Creature*>(pPlayer)->Set_CreatureMagicAttack(m_iCurStat + 1);
				break;
			}
			dynamic_cast<Player*>(pPlayer)->Set_Is_closed_LevelUpBG(true);
			UIManager::GetInstance()->Return_UI();
			UIManager::GetInstance()->SelectDelete_UI(L"UI_LevelUpBG");
			
		}

		else // ② 현재 마우스 슬롯이 차있다면!
		{
		}
	}
}

void UI_AbilityCard::OnMouseClicked(_uint index)
{
}

void UI_AbilityCard::OnIconClicked(_uint index)
{
	UI::OnMouseClicked(index);
}



HRESULT UI_AbilityCard::Add_Component()
{
	Component* pComponent = Clone_Proto(L"Proto_RcTex");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(pComponent);

	pComponent = Clone_Proto(m_strProtoKey);
	if (nullptr == pComponent) { return E_FAIL; }
	m_pTextureIcon = dynamic_cast<Texture*>(pComponent);

	return S_OK;
}

HRESULT UI_AbilityCard::Add_UI()
{
	return S_OK;
}

void UI_AbilityCard::Free()
{
	UI::Free();
}
