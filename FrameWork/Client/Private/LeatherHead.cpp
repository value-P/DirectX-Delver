#include "stdafx.h"
#include "LeatherHead.h"

LeatherHead::LeatherHead(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

LeatherHead::LeatherHead(const LeatherHead& rhs)
	: Super(rhs)
{
}

LeatherHead::~LeatherHead()
{
}

HRESULT LeatherHead::Init_GameObject()
{


	return S_OK;
}

_int LeatherHead::Update_GameObject(const _float& fTimeDelta)
{

	return 0;
}

void LeatherHead::LateUpdate_GameObject()
{
}

void LeatherHead::Render_GameObject()
{
}

HRESULT LeatherHead::Add_Component()
{
	return S_OK;
}

LeatherHead* LeatherHead::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

ItemBase* LeatherHead::Clone()
{
	return nullptr;
}

void LeatherHead::Free()
{
}
