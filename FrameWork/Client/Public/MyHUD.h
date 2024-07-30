#pragma once
#include "UI.h"

class MyHUD : public UI
{
protected:
	explicit MyHUD(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~MyHUD();

public:
	// static MyHUD* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY);
	static MyHUD* Create(LPDIRECT3DDEVICE9 _pGraphicDevice);

public:
	virtual HRESULT Init_UI();

public:
	virtual _int Update_UI(const _float& fTimeDelta);
	virtual void LateUpdate_UI();
	virtual void Render_UI();

public:
	vector<UI*>& Get_ArrQuickSlot() { return m_arrQuickSlot; }
	void Set_Close_CrossCursor(bool _bClose_CrossCursor) { m_bClose_CrossCursor = _bClose_CrossCursor; }

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

private:

	vector<UI*> m_arrQuickSlot;
	_matrix* m_matCrossCursor = nullptr;

	_float m_fSizeX_OriginCrossCursor = 0.f;
	_float m_fSizeY_OriginCrossCursor = 0.f;

	bool m_bClose_CrossCursor = false;
};

