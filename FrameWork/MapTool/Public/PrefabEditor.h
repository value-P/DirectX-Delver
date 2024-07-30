#pragma once
#include "EditorBase.h"
class PrefabEditor :
    public EditorBase
{
private:
    explicit PrefabEditor();
    virtual ~PrefabEditor();

public:
    virtual HRESULT Init_Editor(MainEditor* pMainEditor) override;
    virtual void  Update_Editor() override;

private:
    void    TexListSetting();

    void Rect_Prefab();
    void Cube_Prefab();
    void Billboard_Prefab();

private:
    vector<const char*> m_vecTexlist[PREFAB_END];

    int iRcTexCurrent = 0;
    int iCubeTexCurrent = 0;

public:
    static PrefabEditor* Create(MainEditor* pMainEditor);

private:
    virtual void Free();
};

