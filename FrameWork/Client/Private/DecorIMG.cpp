#include "stdafx.h"
#include "DecorIMG.h"

DecorIMG::DecorIMG(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

DecorIMG::~DecorIMG()
{
}

DecorIMG* DecorIMG::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, wstring _strProtoKey, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	DecorIMG* pInstance = new DecorIMG(_pGraphicDevice);

	if (FAILED(pInstance->Init_UI(_strProtoKey, _fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleChild::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT DecorIMG::Init_UI(wstring _strProtoKey, const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
{
	UI::Init_UI(_fX, _fY, _fSizeX, _fSizeY);

	m_strProtoKey = _strProtoKey;

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

_int DecorIMG::Update_UI(const _float& fTimeDelta)
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::Update_UI(fTimeDelta);
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
	return 0;
}

void DecorIMG::LateUpdate_UI()
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::LateUpdate_UI();
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
}

void DecorIMG::Render_UI()
{
	// �� Ŭ������ ���������� ǥ���ɶ� ������Ʈ ������Ұ� �ִٸ� ���⿡
}

void DecorIMG::Update_Icon(const _float& fTimeDelta)
{
}

void DecorIMG::LateUpdate_Icon()
{
}

void DecorIMG::Render_Icon()
{

	UI::Render_Icon();

}

void DecorIMG::OnMouseClicked(_uint index)
{
	UI::OnMouseClicked(index);
}

void DecorIMG::OnIconClicked(_uint index)
{
	// �� Ŭ������ ���������� ǥ���ɶ� Ŭ�������� �Ҹ��� �ִٸ� ���⿡
	//Engine::Activate_UI(this);	// �ڱ��ڽ��� �������μ� UIMgr�� m_CurUI�� �ڽ��� ��
}

void DecorIMG::OnIconClicking(_uint index)
{
}

HRESULT DecorIMG::Add_Component()
{
	Component* pComponent = Clone_Proto(L"Proto_RcTex");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(pComponent);
	
	pComponent = Clone_Proto(m_strProtoKey);
	if (nullptr == pComponent) { return E_FAIL; }
	m_pTextureIcon = dynamic_cast<Texture*>(pComponent);
	
	return S_OK;
}

HRESULT DecorIMG::Add_UI()
{
	return S_OK;
}

void DecorIMG::Free()
{
	UI::Free();
}
