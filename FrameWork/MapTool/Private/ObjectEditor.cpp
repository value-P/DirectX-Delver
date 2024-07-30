#include "pch.h"
#include "ObjectEditor.h"
#include "MainEditor.h"
#include "PrefabMgr.h"
#include "CloneRect.h"
#include "Cube.h"
#include "BillBoardObj.h"
#include "PickingMgr.h"
#include "Export_Utility.h"

ObjectEditor::ObjectEditor()
{
    ZeroMemory(current_PrefabSize, IM_ARRAYSIZE(current_PrefabSize));
}

ObjectEditor::~ObjectEditor()
{
}

HRESULT ObjectEditor::Init_Editor(MainEditor* pMainEditor)
{
    SCENETYPE curScene = pMainEditor->Get_CurScene();

    HRESULT result = __super::Init_Editor(pMainEditor);

    TexListSetting(curScene);

    return result;
}

void ObjectEditor::Update_Editor()
{
    SCENETYPE curScene = m_pMainEditor->Get_CurScene();

    if (ImGui::CollapsingHeader("Make Clone"))
    {
        ImGui::BulletText("PrefabType");
        const char* ObjTypes[] = { "RECT", "CUBE", "COLBOX", "BILLBOARD"};
        static int ObjType = 0;
        ImGui::Combo("clone", &ObjType, ObjTypes, IM_ARRAYSIZE(ObjTypes));
        
        for (int i = 0; i < IM_ARRAYSIZE(current_PrefabSize); ++i)
        {
            if (current_PrefabSize[i] != PrefabMgr::GetInstance()->m_PrefabList[curScene][i].size())
                TexListSetting(curScene);
        }

        switch (ObjType)
        {
        case 0:
            ObjectEditor::Rect_Prefab(curScene);
            break;
        case 1:
            ObjectEditor::Cube_Prefab(curScene);
            break;
        case 2:
            ObjectEditor::ColBox_Prefab(curScene);
            break;
        case 3:
            ObjectEditor::Billboard_Prefab(curScene);
        }
    }
}

void ObjectEditor::TexListSetting(SCENETYPE eScene)
{
    auto PrefabList = PrefabMgr::GetInstance()->m_PrefabList;

    for (int i = 0; i < PREFAB_END; ++i)
    {
        size_t iSize = PrefabList[eScene][i].size();
        current_PrefabSize[i] = iSize;
        m_vecTexlist[i].resize(iSize);

        int iCnt = 0;
        for (auto& iter : PrefabList[eScene][i])
        {
            m_vecTexlist[i][iCnt] = iter.sName;
            ++iCnt;
        }
    }
}

void ObjectEditor::Rect_Prefab(SCENETYPE eScene)
{
    ImGui::BulletText("Prefab List");
    ImGui::ListBox("Rect", &iRcPrefab, m_vecTexlist[PREFAB_RCTEX].data(), m_vecTexlist[PREFAB_RCTEX].size(), 8);

    ImGui::BulletText("Initial Transform");
    static _vec3 InitialPos = { 0.f,0.f,0.f };
    ImGui::InputFloat3("InitPos_Rect", (float*)InitialPos);   
   
    ImGui::BulletText("Initial Scale");
    static _vec2 InitialScale = { 1.f,1.f };
    ImGui::InputFloat2("InitSale_Rect", (float*)InitialScale);

    if (ImGui::Button("Create Clone"))
    {
        auto iter = PrefabMgr::GetInstance()->m_PrefabList[eScene][PREFAB_RCTEX].begin();

        for (int i = 0; i < iRcPrefab; ++i)
            ++iter;

        PREFABDAT dat = *iter;
        _vec2 scale = { InitialScale.x,InitialScale.y };

        _vec3 InitRot = { D3DXToRadian(90.f),0.f,0.f };
        _vec3 InitPos = InitialPos;

        GameObject* pObj = CloneRect::Create(m_pMainEditor->Get_GrahicDev(), InitPos, scale, dat.szTexPath, dat.szName);
        pObj->Get_Transform()->Set_Scale(InitialScale.x, InitialScale.y, 1.f);
        pObj->Get_Transform()->Set_Rotation(&InitRot);
        m_pMainEditor->Add_GameObject(LAYER_ENVI, pObj);
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete Prefab"))
    {
        Delete_Prefab(PREFAB_RCTEX,iRcPrefab, eScene);
        PrefabMgr::GetInstance()->Save_Prefab(eScene);
    }
}

void ObjectEditor::Cube_Prefab(SCENETYPE eScene)
{
    ImGui::BulletText("Prefab List");
    ImGui::ListBox("Cube", &iCubePrefab, m_vecTexlist[PREFAB_CUBE].data(), m_vecTexlist[PREFAB_CUBE].size(), 8);

    ImGui::BulletText("Initial Transform");
    static _vec3 InitialPos;
    ImGui::InputFloat3("InitPos_Cube", (float*)InitialPos);

    ImGui::BulletText("Initial Scale");
    static _vec3 InitialScale;
    ImGui::InputFloat3("InitScale_Scale", (float*)InitialScale);


    if (ImGui::Button("Create Clone"))
    {
        auto iter = PrefabMgr::GetInstance()->m_PrefabList[eScene][PREFAB_CUBE].begin();

        for (int i = 0; i < iCubePrefab; ++i)
            ++iter;

        PREFABDAT dat = *iter;
        GameObject* pObj = nullptr;
        _vec3 InitPos = InitialPos;

        pObj = Cube::Create(m_pMainEditor->Get_GrahicDev(), InitPos, dat.szTexPath, dat.szName);
        pObj->Get_Transform()->Set_Scale(InitialScale);
        m_pMainEditor->Add_GameObject(LAYER_ENVI, pObj);
    }

    if (ImGui::Button("Delete Prefab"))
    {
        Delete_Prefab(PREFAB_CUBE, iCubePrefab, eScene);
        PrefabMgr::GetInstance()->Save_Prefab(eScene);
    }


}

void ObjectEditor::ColBox_Prefab(SCENETYPE eScene)
{
    ImGui::BulletText("ColBox Scale");
    static _vec3 InitScale = { 1.f,1.f,1.f };
    ImGui::InputFloat3("ColBox Scale", (float*)InitScale, "%.2f");

    ImGui::BulletText("ColBox Transform");
    static _vec3 InitTransform;
    ImGui::InputFloat3("ColBox Transform", (float*)InitTransform, "%.2f");

    ImGui::BulletText("Create");
    if (ImGui::Button("Create"))
    {
        ColliderBox* pBox = ColliderBox::Create(m_pMainEditor->Get_GrahicDev(), InitTransform);
        pBox->Get_Transform()->Set_Scale(InitScale.x, InitScale.y, InitScale.z);
        m_pMainEditor->Add_ColliderBox(pBox);
    }

}

void ObjectEditor::Billboard_Prefab(SCENETYPE eScene)
{
    ImGui::BulletText("Prefab List");
    ImGui::ListBox("BillBoard", &iBillBoardPrefab, m_vecTexlist[PREFAB_BILLBOARD].data(), m_vecTexlist[PREFAB_BILLBOARD].size(), 8);

    ImGui::BulletText("Initial Transform");
    static _vec3 InitialPos = { 0.f,0.f,0.f };
    ImGui::InputFloat3("InitPos_Rect", (float*)InitialPos);

    ImGui::BulletText("Initial Scale");
    static _vec3 InitialScale = { 1.f,1.f,1.f };
    ImGui::InputFloat3("InitSale_Rect", (float*)InitialScale);

    ImGui::BulletText("Initial Scale");
    static _bool CreateMode = false;
    ImGui::Checkbox("Create Mode", &CreateMode);

    ImGui::BulletText("Floor Or Ceiling");
    ImGui::RadioButton("Floor", &isFloor, 0); ImGui::SameLine();
    ImGui::RadioButton("Ceiling", &isFloor, 1);

    if (Engine::Key_Down(VK_RBUTTON) && CreateMode)
    {
        m_pMainEditor->m_bColliderEditMode = true;
        _vec3 PickingPos = m_pMainEditor->Get_PickedPos();
        
        auto iter = PrefabMgr::GetInstance()->m_PrefabList[eScene][PREFAB_BILLBOARD].begin();

        for (int i = 0; i < iBillBoardPrefab; ++i)
            ++iter;

        PREFABDAT dat = *iter;
        _vec2 scale = { InitialScale.x,InitialScale.y };

        _vec3 InitRot = { 0.f,0.f,0.f };
        _vec3 InitPos = PickingPos;

        if (isFloor == 0) InitPos.y += 1.f * scale.y;
        else InitPos.y -= 1.f * scale.y;

        GameObject* pObj = BillBoardObj::Create(m_pMainEditor->Get_GrahicDev(), InitPos, scale, dat.szTexPath, dat.szName);
        pObj->Get_Transform()->Set_Scale(InitialScale.x, InitialScale.y, InitialScale.z);
        pObj->Get_Transform()->Set_Rotation(&InitRot);
        m_pMainEditor->Add_GameObject(LAYER_ENVI, pObj);
    }

    if (ImGui::Button("Delete Prefab"))
    {
        Delete_Prefab( PREFAB_BILLBOARD, iBillBoardPrefab, eScene);
        PrefabMgr::GetInstance()->Save_Prefab(eScene);
    }
}

void ObjectEditor::Delete_Prefab(PREFABTYPE eType, int index, SCENETYPE eScene)
{
    auto PrefabList = PrefabMgr::GetInstance()->m_PrefabList;

    auto iter = PrefabList[eScene][eType].begin();
    for (int i = 0; i < index; ++i)
        ++iter;

    PrefabList[eScene][eType].erase(iter);

    TexListSetting(eScene);
}

ObjectEditor* ObjectEditor::Create(MainEditor* pMainEditor) 
{
    ObjectEditor* pInstance = new ObjectEditor;

    if (FAILED(pInstance->Init_Editor(pMainEditor)))
    {
        Safe_Release(pInstance);

        MSG_BOX("ObjectEditor Create Failed");
        return nullptr;
    }

    return pInstance;
}

void ObjectEditor::Free()
{
}
