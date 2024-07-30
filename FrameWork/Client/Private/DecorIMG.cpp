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
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::Update_UI(fTimeDelta);
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
	return 0;
}

void DecorIMG::LateUpdate_UI()
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::LateUpdate_UI();
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
}

void DecorIMG::Render_UI()
{
	// 이 클래스가 아이콘으로 표현될때 업데이트 해줘야할게 있다면 여기에
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
	// 이 클래스가 아이콘으로 표현될때 클릭했을때 불릴게 있다면 여기에
	//Engine::Activate_UI(this);	// 자기자신을 넣음으로서 UIMgr의 m_CurUI가 자신이 됨
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
