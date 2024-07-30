#include "pch.h"
#include "SceneEditor.h"
#include "MainEditor.h"
#include "DynamicCamera_Tool.h"

SceneEditor::SceneEditor()
{
}

SceneEditor::~SceneEditor()
{
}

HRESULT SceneEditor::Init_Editor(MainEditor* pMainEditor)
{
    return __super::Init_Editor(pMainEditor);
}

void SceneEditor::Update_Editor()
{
    if (ImGui::CollapsingHeader("Scene/Camera"))
    {
        // 씬 세팅
        {
            const char* items[] = { "Logo", "Villeage", "Stage1", "Stage2", "Stage3", "Stage4", "Stage_Boss"};
            static int item_current = 0;

            ImGui::BulletText("Current Scene : %s", items[item_current]);
            ImGui::Combo("Scene", &item_current, items, IM_ARRAYSIZE(items));

            if (ImGui::Button("ChangeScene"))
            {
                m_pMainEditor->Set_CurScene((SCENETYPE)item_current);
            }
        }

        // 카메라 세팅
        {
            ImGui::BulletText("Camera Speed");
            ImGui::InputFloat("Cam Speed", &(m_pMainEditor->Get_Camera()->m_fSpeed));

            ImGui::BulletText("Camera Transform");
            _vec3 vEye = *(m_pMainEditor->Get_Camera()->Get_Eye());
            static float camPos[3] = { 0.f,0.f,0.f};
            if (ImGui::InputFloat3("trans", camPos))
            {
                _vec3 vCamPos = { camPos[0],-camPos[1],camPos[2] };
                m_pMainEditor->Get_Camera()->Set_Eye(&vCamPos);
            }
            else
            {
                camPos[0] = vEye.x;
                camPos[1] = -vEye.y;
                camPos[2] = vEye.z;
            }

            ImGui::BulletText("Camera Rotation");
            _vec3 vRot = *(m_pMainEditor->Get_Camera()->Get_Rotate());
            static float camRot[3] = { 0.f,0.f,0.f };
            if (ImGui::InputFloat3("rot", camRot))
            {
                _vec3 vCamRot = { camRot[0],-camRot[1],camRot[2] };
                m_pMainEditor->Get_Camera()->Set_Rotate(&vCamRot);
            }
            else
            {
                camRot[0] = vRot.x;
                camRot[1] = -vRot.y;
                camRot[2] = vRot.z;
            }
        }

        ImGui::Text("");

        ImGui::BulletText("Save Current Scene GameObjects");
        if(ImGui::Button("Save"))
        {
            m_pMainEditor->Save_GameObject();
            m_pMainEditor->Save_ColBox();
        }
    }
}

SceneEditor* SceneEditor::Create(MainEditor* pMainEditor)
{
    SceneEditor* pInstance = new SceneEditor;

    if (FAILED(pInstance->Init_Editor(pMainEditor)))
    {
        Safe_Release(pInstance);

        MSG_BOX("SceneEditor Create Failed");
        return nullptr;
    }

    return pInstance;
}

void SceneEditor::Free()
{
}
