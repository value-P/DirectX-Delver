#pragma once
#include "EditorBase.h"
#include "MainEditor.h"

class SceneEditor :
    public EditorBase
{
private:
    explicit SceneEditor();
    virtual ~SceneEditor();

public:
    virtual HRESULT Init_Editor(MainEditor* pMainEditor) override;
    virtual void  Update_Editor() override;

private:


public:
    static SceneEditor* Create(MainEditor* pMainEditor);

private:
    virtual void Free();
};

