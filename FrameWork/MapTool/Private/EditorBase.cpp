#include "pch.h"
#include "EditorBase.h"

EditorBase::EditorBase()
{
}

EditorBase::~EditorBase()
{
}

HRESULT EditorBase::Init_Editor(MainEditor* pMainEditor)
{
    m_pMainEditor = pMainEditor;
    
    if (m_pMainEditor == nullptr)
        return E_FAIL;

    return S_OK;
}

void EditorBase::Free()
{
}
