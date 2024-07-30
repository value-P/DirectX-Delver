#include "pch.h"
#include "PrefabEditor.h"
#include "MainEditor.h"
#include "PrefabMgr.h"
#include "DataMgr.h"
#include "CloneRect.h"

PrefabEditor::PrefabEditor()
{
}

PrefabEditor::~PrefabEditor()
{
}

HRESULT PrefabEditor::Init_Editor(MainEditor* pMainEditor)
{
    HRESULT result = __super::Init_Editor(pMainEditor);

    TexListSetting();

    return result;
}

void PrefabEditor::Update_Editor()
{
    if (ImGui::CollapsingHeader("Make Prefab"))
    {
        ImGui::BulletText("PrefabType");
        const char* ObjTypes[] = { "RECT", "CUBE", "BILLBOARD"};
        static int ObjType = 0;
        ImGui::Combo("combo", &ObjType, ObjTypes, IM_ARRAYSIZE(ObjTypes));

        switch (ObjType)
        {
        case 0:
            PrefabEditor::Rect_Prefab();
            break;
        case 1:
            PrefabEditor::Cube_Prefab();
            break;
        case 2:
            PrefabEditor::Billboard_Prefab();
            break;
        }

    }
}

void PrefabEditor::TexListSetting()
{
    auto texDataMap = DataMgr::GetInstance()->Get_TexMap();

    for (int i = 0; i < PREFAB_END; ++i)
    {
        size_t iSize = texDataMap[i].size();
        m_vecTexlist[i].resize(iSize);

        int iCnt = 0;
        for (auto& iter : texDataMap[i])
        {
            m_vecTexlist[i][iCnt] = iter.second.texName.c_str();
            ++iCnt;
        }
    }
}

void PrefabEditor::Rect_Prefab()
{
    ImGui::BulletText("Prefab Name");
    static char prefabName[128];
    ImGui::InputText(" ", prefabName, IM_ARRAYSIZE(prefabName));

    // 텍스처 리스트박스
    ImGui::BulletText("Texture");
    ImGui::ListBox("texture", &iRcTexCurrent, m_vecTexlist[PREFAB_RCTEX].data(), m_vecTexlist[PREFAB_RCTEX].size(), 8);

    // 미리보기 이미지
    auto texMap = DataMgr::GetInstance()->Get_TexMap()[PREFAB_RCTEX];

    // 미리보기
    LPDIRECT3DTEXTURE9 pTex = nullptr;
    D3DSURFACE_DESC my_texture_desc;
    string key;
	if ((UINT)iRcTexCurrent >= texMap.size())
    {
        pTex = nullptr;
        ImGui::Text("%.0fx%.0f", 0.f, 0.f);
    }
    else
    {
        key = m_vecTexlist[PREFAB_RCTEX][iRcTexCurrent];
        pTex = static_cast<LPDIRECT3DTEXTURE9>(texMap[key].pTex->Get_Texture());
        pTex->GetLevelDesc(0, &my_texture_desc);
        float my_tex_w = float(my_texture_desc.Width);
        float my_tex_h = float(my_texture_desc.Height);
        ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
    }
    // 미리보기 띄우기
    {
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
        ImVec4 tint_col = ImGui::GetStyleColorVec4(ImGuiCol_Text);   // No tint
        ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
        ImGui::Image(pTex, ImVec2(64.f, 64.f), uv_min, uv_max, tint_col, border_col);
    }
    

    if (ImGui::Button("Create Prefab"))
    {
        PREFABDAT Dat;
        string sName(prefabName);
        wstring wsName;
        wsName.assign(sName.begin(),sName.end());
        
        strcpy_s(Dat.sName, sName.c_str());
        lstrcpy(Dat.szName, wsName.c_str());
        lstrcpy(Dat.szTexPath, texMap[key].texPath.c_str());
        Dat.eType = PREFAB_RCTEX;
        
        PrefabMgr::GetInstance()->Add_Prefab(PREFAB_RCTEX, Dat, m_pMainEditor->Get_CurScene());
        PrefabMgr::GetInstance()->Save_Prefab(m_pMainEditor->Get_CurScene());
    }

}

void PrefabEditor::Cube_Prefab()
{
    ImGui::BulletText("Prefab Name");
    static char prefabName[128];
    ImGui::InputText(" ", prefabName, IM_ARRAYSIZE(prefabName));

    // 텍스처 리스트박스
    ImGui::BulletText("Texture");
    ImGui::ListBox("texture", &iCubeTexCurrent, m_vecTexlist[PREFAB_CUBE].data(), m_vecTexlist[PREFAB_CUBE].size(), 8);

    auto texMap = DataMgr::GetInstance()->Get_TexMap()[PREFAB_CUBE];
    string key = m_vecTexlist[PREFAB_CUBE][iCubeTexCurrent];

    if (ImGui::Button("Create Prefab"))
    {
        PREFABDAT Dat;
        string sName(prefabName);
        wstring wsName;
        wsName.assign(sName.begin(), sName.end());

        strcpy_s(Dat.sName, sName.c_str());
        lstrcpy(Dat.szName, wsName.c_str());
        lstrcpy(Dat.szTexPath, texMap[key].texPath.c_str());
        Dat.eType = PREFAB_CUBE;

        PrefabMgr::GetInstance()->Add_Prefab(PREFAB_CUBE, Dat, m_pMainEditor->Get_CurScene());
        PrefabMgr::GetInstance()->Save_Prefab(m_pMainEditor->Get_CurScene());
    }

}

void PrefabEditor::Billboard_Prefab()
{
    ImGui::BulletText("Prefab Name");
    static char prefabName[128];
    ImGui::InputText(" ", prefabName, IM_ARRAYSIZE(prefabName));

    // 텍스처 리스트박스
    ImGui::BulletText("Texture");
    ImGui::ListBox("texture", &iRcTexCurrent, m_vecTexlist[PREFAB_BILLBOARD].data(), m_vecTexlist[PREFAB_BILLBOARD].size(), 8);

    // 미리보기 이미지
    auto texMap = DataMgr::GetInstance()->Get_TexMap()[PREFAB_BILLBOARD];

    // 미리보기
    LPDIRECT3DTEXTURE9 pTex = nullptr;
    D3DSURFACE_DESC my_texture_desc;
    string key;
    if (UINT(iRcTexCurrent) >= texMap.size())
    {
        pTex = nullptr;
        ImGui::Text("%.0fx%.0f", 0.f, 0.f);
    }
    else
    {
        key = m_vecTexlist[PREFAB_BILLBOARD][iRcTexCurrent];
        pTex = static_cast<LPDIRECT3DTEXTURE9>(texMap[key].pTex->Get_Texture());
        pTex->GetLevelDesc(0, &my_texture_desc);
        float my_tex_w = float(my_texture_desc.Width);
        float my_tex_h = float(my_texture_desc.Height);
        ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
    }
    // 미리보기 띄우기
    {
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
        ImVec4 tint_col = ImGui::GetStyleColorVec4(ImGuiCol_Text);   // No tint
        ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
        ImGui::Image(pTex, ImVec2(64.f, 64.f), uv_min, uv_max, tint_col, border_col);
    }

    if (ImGui::Button("Create Prefab"))
    {
        PREFABDAT Dat;
        string sName(prefabName);
        wstring wsName;
        wsName.assign(sName.begin(), sName.end());

        strcpy_s(Dat.sName, sName.c_str());
        lstrcpy(Dat.szName, wsName.c_str());
        lstrcpy(Dat.szTexPath, texMap[key].texPath.c_str());
        Dat.eType = PREFAB_BILLBOARD;

        PrefabMgr::GetInstance()->Add_Prefab(PREFAB_BILLBOARD, Dat, m_pMainEditor->Get_CurScene());
        PrefabMgr::GetInstance()->Save_Prefab(m_pMainEditor->Get_CurScene());
    }
}

PrefabEditor* PrefabEditor::Create(MainEditor* pMainEditor)
{
    PrefabEditor* pInstance = new PrefabEditor;

    if (FAILED(pInstance->Init_Editor(pMainEditor)))
    {
        Safe_Release(pInstance);

        MSG_BOX("PrefabEditor Create Failed");
        return nullptr;
    }

    return pInstance;
}

void PrefabEditor::Free()
{
    for (int i = 0; i < PREFAB_END; ++i)
    {
        m_vecTexlist[i].clear();
        m_vecTexlist[i].shrink_to_fit();
    }
}
