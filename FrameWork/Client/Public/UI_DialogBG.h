#pragma once
#include "UI.h"

class NonPlayerCharacter;

class UI_DialogBG : public UI
{
protected:
	explicit UI_DialogBG(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~UI_DialogBG();

public:
	static UI_DialogBG* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, NonPlayerCharacter* _pNPC, vector<vector<wstring>> _pNPCDialogVec, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY);
public:
	virtual HRESULT Init_UI(vector<vector<wstring>> _pNPCDialogVec, NonPlayerCharacter* _pNPC, const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY);

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
	void Key_Input(const _float& fTimeDelta);
	void Next_Dialog();

private:
	virtual void Free();

private:
	_matrix* m_pMatContinue_Icon = nullptr;

	

	NonPlayerCharacter* m_pNPC;

	vector<vector<wstring>> m_pNPCDialogVec;
	vector<wstring> strCurNPCDialogVec;
	_int Idx_CurNPCDialog = 0;
	_int iCurNPCDialog_Line = 0;

	D3DXCOLOR m_tDialogColor{ 1.f, 1.f, 1.f, 1.f };

	_float m_fFrameContinue_Icon = 0.f;

	bool m_bIsDownIcon = false;
};

