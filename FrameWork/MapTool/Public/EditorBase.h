#pragma once
#include "Base.h"
#include "Engine_Define.h"

class MainEditor;

class EditorBase :
    public Base
{
protected:
    explicit EditorBase();
    virtual ~EditorBase();

public:
    virtual HRESULT Init_Editor(MainEditor* pMainEditor);
    virtual void  Update_Editor() {}

protected:
    MainEditor* m_pMainEditor = nullptr;

protected:
    virtual void Free();
};

