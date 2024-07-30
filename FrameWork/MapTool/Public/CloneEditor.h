#pragma once
#include "EditorBase.h"

namespace Engine
{
    class GameObject;
}

class ColliderBox;

class CloneEditor :
    public EditorBase
{
private:
    explicit CloneEditor();
    virtual ~CloneEditor();

public:
    virtual HRESULT Init_Editor(MainEditor* pMainEditor) override;
    virtual void  Update_Editor() override;

private:
    GameObject* m_pPickedObj = nullptr;
    ColliderBox* m_pPickedColBox = nullptr;

public:
    static CloneEditor* Create(MainEditor* pMainEditor);

private:
    virtual void Free();

};

