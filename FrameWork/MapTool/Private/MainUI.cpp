#include "pch.h"
#include "MainUI.h"
#include "MainEditor.h"

#include "SceneEditor.h"
#include "ObjectEditor.h"
#include "PrefabEditor.h"
#include "CloneEditor.h"

MainUI::MainUI()
{
}

MainUI::~MainUI()
{
}

HRESULT MainUI::Init_UI(MainEditor* pMainEditor)
{
    m_pMainEditor = pMainEditor;

    // 창 열것인가
    m_bOpen = true;

    // 창 옵션 세팅
    //window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    //window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    Set_DefaultSetting();

    return S_OK;
}

void MainUI::Update_UI(const _float& fTimeDelta)
{
    // UI Update
    ImGui::Begin("MainTool",&m_bOpen,window_flags);

    for (auto iter = m_EditorList.begin(); iter != m_EditorList.end(); ++iter)
    {
        (*iter)->Update_Editor();
    }

    ImGui::End();
}

HRESULT MainUI::Set_DefaultSetting()
{
    EditorBase* pEditor = nullptr;

    pEditor = SceneEditor::Create(m_pMainEditor);
    m_EditorList.push_back(pEditor);

    pEditor = CloneEditor::Create(m_pMainEditor);
    m_EditorList.push_back(pEditor);

    pEditor = ObjectEditor::Create(m_pMainEditor);
    m_EditorList.push_back(pEditor);

    pEditor = PrefabEditor::Create(m_pMainEditor);
    m_EditorList.push_back(pEditor);

    return S_OK;
}

MainUI* MainUI::Create(MainEditor* pMainEditor)
{
    MainUI* pInstance = new MainUI();

    if (FAILED(pInstance->Init_UI(pMainEditor)))
    {
        Safe_Release(pInstance);
        MSG_BOX("MainUI Create Failed");
        return nullptr;
    }

    return pInstance;
}

void MainUI::Free()
{
    for_each(m_EditorList.begin(), m_EditorList.end(), CDeleteObj());
    m_EditorList.clear();
}
