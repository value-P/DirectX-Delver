#include "stdafx.h"
#include "HUD_Hit_Effect.h"

HUD_Hit_Effect::HUD_Hit_Effect(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

HUD_Hit_Effect::~HUD_Hit_Effect()
{
}

HUD_Hit_Effect* HUD_Hit_Effect::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	HUD_Hit_Effect* pInstance = new HUD_Hit_Effect(_pGraphicDevice);

	if (FAILED(pInstance->Init_UI(_fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleChild::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT HUD_Hit_Effect::Init_UI(const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
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

_int HUD_Hit_Effect::Update_UI(const _float& fTimeDelta)
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::Update_UI(fTimeDelta);
	
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
	return 0;
}

void HUD_Hit_Effect::LateUpdate_UI()
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::LateUpdate_UI();
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
}

void HUD_Hit_Effect::Render_UI()
{
	// �� Ŭ������ ���������� ǥ���ɶ� ������Ʈ ������Ұ� �ִٸ� ���⿡
}

void HUD_Hit_Effect::Update_Icon(const _float& fTimeDelta)
{
	m_matIcon._11 = (float)WINCX * 0.5f;
	m_matIcon._22 = (float)WINCY * 0.5f;
	//m_matIcon._11 = 0;
	//m_matIcon._22 = 0;
}

void HUD_Hit_Effect::LateUpdate_Icon()
{
}

void HUD_Hit_Effect::Render_Icon()
{
	UI::Render_Icon();
}

void HUD_Hit_Effect::OnMouseClicked(_uint index)
{
	UI::OnMouseClicked(index);
}

void HUD_Hit_Effect::OnIconClicked(_uint index)
{
}

void HUD_Hit_Effect::OnIconClicking(_uint index)
{
}

HRESULT HUD_Hit_Effect::Add_Component()
{
	Component* pComponent = Clone_Proto(L"Proto_RcTex");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(pComponent);


	pComponent = m_pTextureIcon = dynamic_cast<Texture*>(Engine::Clone_Proto(L"Proto_PlayerHit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_DYNAMIC].insert({ L"Proto_HitEffect_Texture", pComponent });

	//pComponent = Clone_Proto(L"Proto_HitEffect_Texture");
	//if (nullptr == pComponent) { return E_FAIL; }
	//m_pTextureIcon = dynamic_cast<Texture*>(pComponent);
	return S_OK;
}

HRESULT HUD_Hit_Effect::Add_UI()
{
	return S_OK;
}

void HUD_Hit_Effect::Free()
{
	UI::Free();
}
