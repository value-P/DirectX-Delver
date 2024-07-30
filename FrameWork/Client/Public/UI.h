#pragma once

#include "UIBase.h"
#include "Export_System.h"
#include "Export_Utility.h"

class UI : public UIBase
{
protected:
	explicit UI(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~UI();

public:
	// UI�� ���� �Ҷ� UI�� ũ��� ��ġ�� �־������
	/* SizeX = 80.0f, SizeY = 30.0f �̸� ���� ũ�� 80.0f, ���� ũ�� 30.0f �� �̹����� �׷���
	X ( 0 ~ 800 ), Y ( 0 ~ 600 ), ex ) X = 100.0f, Y = 500.0f �̸� ( 100.0f, 500.0f ) ��ġ�������� ( SizeX, SizeY ) ũ��� �̹����� �׷���
	���� �ΰ��� ���ø� ��ġ�� ( 100.0f, 500.0f ) ��ġ�������� ( 80.0f, 30.0f ) ũ���� UI�� ������
	�̷� RECT�� �׷����ٰ� �����ϸ� �� left( 100.0f ), top( 500.0f ), right( 180.0f ), bottom( 530.0f ) */
	virtual HRESULT Init_UI(const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY);

public:
	virtual _int Update_UI(const _float& fTimeDelta);
	virtual void LateUpdate_UI();
	virtual void Render_UI();

public:	// �ڽ� UI�� Icon�� ���ؼ� ������Ʈ�� ��
	virtual void Update_Icon(const _float& fTimeDelta) abstract;
	virtual void LateUpdate_Icon() abstract;
	virtual void Render_Icon();

public:
	virtual void OnMouseClicked(_uint index);
	virtual void OnIconClicked(_uint index) abstract;
	virtual void OnIconClicking(_uint index) abstract;

public:		// Get Function
	virtual UIBase* GetParentUI() { return m_pParentUI; }
	vector<vector<UI*>*>& Get_ChildUI_Group() { return m_vecChildUI_Group; }
	vector<UI*>& GetChildUIVec() { return m_vecChildUI; }
	virtual bool GetState_Button() { return m_bActive_Button; }

	virtual VIBuffer* Get_VIBuffer() { return m_pBufferCom; }
	Texture* Get_TextureItem() { return m_pTextureItem; }

	float Get_PosX() { return m_fX; }
	float Get_PosY() { return m_fY; }

	_matrix* Get_MatIconWorld() { return &m_matIcon; }
	_matrix Get_MatBGWorld() { return m_matBG; }
	//UITYPE_ID Get_UiType() { return m_eUITYPE; }

public: // Set
	void Set_TextureItem(Texture* texture) { m_pTextureItem = texture; };

protected:		// Set Function
	void Add_ChildUI_Group(vector<UI*>* _vecChildUI);
	void Add_ChildUI(UI* _pUI);
	void SetState_Button(_int _iIndex, bool _bActive_Button);
	

protected:
	virtual _int Check_State_Button();

private:		// Class Function
	void SettingUI();

protected:
	RcTex* m_pBufferCom{ nullptr };

	Texture* m_pTextureIcon{ nullptr };

	Texture* m_pTextureBG{ nullptr };

	Texture* m_pTextureItem{ nullptr };
	
	UI* m_pParentUI{ nullptr };
	
	vector<vector<UI*>*> m_vecChildUI_Group;
	vector<UI*> m_vecChildUI;

	D3DXMATRIX m_matIcon, m_matBG;
	float m_fX{ 0.0f }, m_fY{ 0.0f }, m_fSizeX{ 1.0f }, m_fSizeY{ 1.0f };
	bool m_bActive_Button = false;
	bool m_bMouse_Lock = false;

	//enum UITYPE_ID m_eUITYPE = UT_END;

	enum UIRETURN_TYPE m_eUIReturn_Type = URT_END;

protected:
	virtual void Free();
};

