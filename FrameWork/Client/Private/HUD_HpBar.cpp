#include "stdafx.h"
#include "HUD_HpBar.h"
#include "Creature.h"

HUD_HpBar::HUD_HpBar(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

HUD_HpBar::~HUD_HpBar()
{
}

HUD_HpBar* HUD_HpBar::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	HUD_HpBar* pInstance = new HUD_HpBar(_pGraphicDevice);

	if (FAILED(pInstance->Init_UI(_fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleChild::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT HUD_HpBar::Init_UI(const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
{
	UI::Init_UI(_fX, _fY, _fSizeX, _fSizeY);

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

_int HUD_HpBar::Update_UI(const _float& fTimeDelta)
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::Update_UI(fTimeDelta);
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
	return 0;
}

void HUD_HpBar::LateUpdate_UI()
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::LateUpdate_UI();
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
}

void HUD_HpBar::Render_UI()
{
	// �� Ŭ������ ���������� ǥ���ɶ� ������Ʈ ������Ұ� �ִٸ� ���⿡
}

void HUD_HpBar::Update_Icon(const _float& fTimeDelta)
{
	if (!pPlayer)
		pPlayer = (*(Engine::Get_GameObject(L"GameObject", L"Player"))).front();

	m_iPlayer_MaxHp = dynamic_cast<Creature*>(pPlayer)->Get_CreatureMaxHp();
	m_iPlayer_CurHp = dynamic_cast<Creature*>(pPlayer)->Get_CreatureHp();
	
	m_strCurHP_Status = to_wstring(m_iPlayer_CurHp) + L"/" + to_wstring(m_iPlayer_MaxHp);

	_float fCurHpBar = (_float(m_iPlayer_CurHp) / _float(m_iPlayer_MaxHp)) * 42;
	m_matIcon._11 = fCurHpBar;
	m_matIcon._41 = m_fX - float(WINCX >> 1) + fCurHpBar;
}

void HUD_HpBar::LateUpdate_Icon()
{
}

void HUD_HpBar::Render_Icon()
{
	UI::Render_Icon();
	Engine::Render_Font(L"Font_RoundShape_HP", m_strCurHP_Status, &_vec4(m_fX, m_fY, m_fX + m_fSizeX - 14, m_fY + m_fSizeY), DT_NOCLIP | DT_RIGHT | DT_SINGLELINE | DT_VCENTER, m_tColor);
}

void HUD_HpBar::OnMouseClicked(_uint index)
{
	UI::OnMouseClicked(index);
}

void HUD_HpBar::OnIconClicked(_uint index)
{
	// �� Ŭ������ ���������� ǥ���ɶ� Ŭ�������� �Ҹ��� �ִٸ� ���⿡
	//Engine::Activate_UI(this);	// �ڱ��ڽ��� �������μ� UIMgr�� m_CurUI�� �ڽ��� ��
}

void HUD_HpBar::OnIconClicking(_uint index)
{
}

HRESULT HUD_HpBar::Add_Component()
{
	Component* pComponent = Clone_Proto(L"Proto_RcTex");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(pComponent);

	pComponent = Clone_Proto(L"Proto_HpBar_Texture");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pTextureIcon = dynamic_cast<Texture*>(pComponent);

	return S_OK;
}

HRESULT HUD_HpBar::Add_UI()
{
	return S_OK;
}

void HUD_HpBar::Free()
{
	UI::Free();
}
