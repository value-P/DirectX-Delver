#pragma once
#include "EditorBase.h"
class ObjectEditor :
    public EditorBase
{
private:
    explicit ObjectEditor();
    virtual ~ObjectEditor();

public:
    virtual HRESULT Init_Editor(MainEditor* pMainEditor) override;
    virtual void  Update_Editor() override;

private:
    void    TexListSetting(SCENETYPE eScene);

    void Rect_Prefab(SCENETYPE eScene);
    void Cube_Prefab(SCENETYPE eScene);
    void ColBox_Prefab(SCENETYPE eScene);
    void Billboard_Prefab(SCENETYPE eScene);

    void Delete_Prefab(PREFABTYPE eType, int index, SCENETYPE eScene);

private:
    vector<const char*> m_vecTexlist[PREFAB_END];

    int iRcPrefab = 0;
    int iCubePrefab = 0;
    int iBillBoardPrefab = 0;

    _int isFloor = 0;


    size_t current_PrefabSize[PREFAB_END];

public:
    static ObjectEditor* Create(MainEditor* pMainEditor);

private:
    virtual void Free();
};

