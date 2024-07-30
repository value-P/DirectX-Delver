#pragma once
#include "Base.h"
#include "Engine_Define.h"

class MainEditor;
class EditorBase;

class MainUI :
    public Base
{
private:
    explicit MainUI();
    virtual ~MainUI();

public:
    HRESULT Init_UI(MainEditor* pMainEditor);
    void Update_UI(const _float& fTimeDelta);

private:
    HRESULT Set_DefaultSetting();

private:
    ImGuiWindowFlags window_flags = 0;
    bool m_bOpen = true;
    
    MainEditor* m_pMainEditor = nullptr;

    list<EditorBase*> m_EditorList;

public:
    static MainUI* Create(MainEditor* pMainEditor);

private:
    virtual void Free();
};

