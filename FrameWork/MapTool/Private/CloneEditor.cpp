#include "pch.h"
#include "CloneEditor.h"
#include "MainEditor.h"
#include "Transform.h"
#include "PickingMgr.h"

CloneEditor::CloneEditor()
{
}

CloneEditor::~CloneEditor()
{
}

HRESULT CloneEditor::Init_Editor(MainEditor* pMainEditor)
{
    HRESULT result = __super::Init_Editor(pMainEditor);

    return result;
}

void CloneEditor::Update_Editor()
{
    if (ImGui::CollapsingHeader("Edit Clone"))
    {
        ImGui::Checkbox("Collider EditMode", &m_pMainEditor->m_bColliderEditMode);
        ImGui::Checkbox("GameObject InvisibleMode", &m_pMainEditor->m_bGameObjectInvisible);

        if (m_pMainEditor->m_bColliderEditMode)
        {
            m_pPickedColBox = m_pMainEditor->Get_PicekdColBox();

            // 박스 콜라이더 수정용
            if (m_pPickedColBox)
            {
                _vec3 PickingPos = PickingMgr::GetInstance()->Get_PickingPos();
                ImGui::BulletText("Picking Pos");
                ImGui::Text("X : %.2f Y : %.2f Z : %.2f", PickingPos.x, PickingPos.y, PickingPos.z);

                Transform* pTransform = m_pPickedColBox->Get_Transform();

                // 피킹한 오브첵트 포지션
                _vec3 ObjCurrentPos;
                pTransform->Get_Info(INFO_POS, &ObjCurrentPos);
                ImGui::BulletText("Current Pos");
                ImGui::Text("X : %.2f", ObjCurrentPos.x); ImGui::SameLine();
                ImGui::Text("Y : %.2f", ObjCurrentPos.y); ImGui::SameLine();
                ImGui::Text("Z : %.2f", ObjCurrentPos.z);

                // 피킹한 오브첵트 스케일
                ImGui::BulletText("Current Scale");
                ImGui::Text("X : %.2f", pTransform->m_vScale.x); ImGui::SameLine();
                ImGui::Text("Y : %.2f", pTransform->m_vScale.y); ImGui::SameLine();
                ImGui::Text("Z : %.2f", pTransform->m_vScale.z);

                ImGui::Text("");

                static _vec3 wantPos = { 0.f,0.f,0.f };
                ImGui::BulletText("Want Pos");
                ImGui::InputFloat3("want Pos", (float*)wantPos);

                static _vec3 wantScale = { 1.f,1.f,1.f };
                ImGui::BulletText("Want Scale");
                ImGui::InputFloat3("want Scale", (float*)wantScale);

                if (ImGui::Button("Apply"))
                {
                    pTransform->Set_Pos(wantPos.x, wantPos.y, wantPos.z);
                    pTransform->Set_Scale(wantScale.x, wantScale.y, wantScale.z);
                }

                if (ImGui::Button("Delete"))
                {
                    _bool result = m_pMainEditor->Erase_ColBox(m_pPickedColBox);
                    if (!result)
                        MSG_BOX("삭제 실패!!");
                }
            }
        }
        else
        {
            m_pPickedObj = m_pMainEditor->Get_PicekdObject();

            if (m_pPickedObj)
            {
                _vec3 PickingPos = PickingMgr::GetInstance()->Get_PickingPos();
                ImGui::BulletText("Picking Pos");
                ImGui::Text("X : %.1f Y : %.1f Z : %.1f", PickingPos.x, PickingPos.y, PickingPos.z);

                Transform* pTransform = m_pPickedObj->Get_Transform();

                _vec3 ObjCurrentPos;
                pTransform->Get_Info(INFO_POS, &ObjCurrentPos);
                ImGui::BulletText("Current Pos");
                ImGui::Text("X : %.1f", ObjCurrentPos.x); ImGui::SameLine();
                ImGui::Text("Y : %.1f", ObjCurrentPos.y); ImGui::SameLine();
                ImGui::Text("Z : %.1f", ObjCurrentPos.z);

                _vec3 ObjCurrentRot = pTransform->GetAngle();
                ImGui::BulletText("Current Rotation");
                ImGui::Text("X : %.1f", D3DXToDegree(ObjCurrentRot.x)); ImGui::SameLine();
                ImGui::Text("Y : %.1f", D3DXToDegree(ObjCurrentRot.y)); ImGui::SameLine();
                ImGui::Text("Z : %.1f", D3DXToDegree(ObjCurrentRot.z));

                _vec3 ObjCurrentScale = pTransform->Get_Scale();
                ImGui::BulletText("Current Scale");
                ImGui::Text("X : %.1f", ObjCurrentScale.x); ImGui::SameLine();
                ImGui::Text("Y : %.1f", ObjCurrentScale.y); ImGui::SameLine();
                ImGui::Text("Z : %.1f", ObjCurrentScale.z);

                ImGui::Text("");

                static _vec3 wantPos = { 0.f,0.f,0.f };
                ImGui::BulletText("Want Pos");
                ImGui::InputFloat3("want Pos", (float*)wantPos);

                static _vec3 wantRotation = { 0.f,0.f,0.f };
                ImGui::BulletText("Want Rotation");
                ImGui::InputFloat3("want Rot", (float*)wantRotation);

                static _vec3 wantScale = { 1.f,1.f,1.f };
                ImGui::BulletText("Want Scale");
                ImGui::InputFloat3("want Scale", (float*)wantScale);

                if (ImGui::Button("Apply"))
                {
                    pTransform->Set_Pos(wantPos.x, wantPos.y, wantPos.z);
                    _vec3 rotRad = { D3DXToRadian(wantRotation.x),D3DXToRadian(wantRotation.y) ,D3DXToRadian(wantRotation.z) };
                    pTransform->Set_Rotation(&rotRad);
                    pTransform->Set_Scale(wantScale.x, wantScale.y, wantScale.z);
                    m_pPickedObj->Get_Buffer()->Set_UVScale(wantScale.x, wantScale.y);
                }

                if (ImGui::Button("Delete"))
                {
                    _bool result = m_pMainEditor->Erase_GameObject(m_pPickedObj);
                    if (!result)
                        MSG_BOX("삭제 실패!!");
                }
            }

        }

    }
}

CloneEditor* CloneEditor::Create(MainEditor* pMainEditor)
{
    CloneEditor* pInstance = new CloneEditor;

    if (FAILED(pInstance->Init_Editor(pMainEditor)))
    {
        Safe_Release(pInstance);

        MSG_BOX("CloneEditor Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CloneEditor::Free()
{
}
