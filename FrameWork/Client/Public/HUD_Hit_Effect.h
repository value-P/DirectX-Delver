#pragma once
#include "UI.h"
class HUD_Hit_Effect : public UI
{
	explicit HUD_Hit_Effect(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~HUD_Hit_Effect();

public:
	static HUD_Hit_Effect* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY);

public:
	virtual HRESULT Init_UI(const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY);

public:
	virtual _int Update_UI(const _float& fTimeDelta);
	virtual void LateUpdate_UI();
	virtual void Render_UI();

protected:
	virtual void Update_Icon(const _float& fTimeDelta);
	virtual void LateUpdate_Icon();
	virtual void Render_Icon();

protected:
	virtual void OnMouseClicked(_uint index);
	virtual void OnIconClicked(_uint index);
	virtual void OnIconClicking(_uint index);

private:
	HRESULT Add_Component();
	HRESULT Add_UI();

private:
	virtual void Free();

};

