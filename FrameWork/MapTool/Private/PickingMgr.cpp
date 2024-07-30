#include "pch.h"
#include "PickingMgr.h"
#include "ColliderBox.h"

PickingMgr* PickingMgr::m_pInstance{ nullptr };

PickingMgr::PickingMgr()
{
}

PickingMgr::~PickingMgr()
{
	Free();
}

bool PickingMgr::Picking(LPDIRECT3DDEVICE9 _pGraphicDevice, map<const wstring, list<GameObject*>>* _pMap, GameObject** _ppGameObject, D3DXVECTOR3* _pVec3)
{
	if ((*_pMap).empty())
	{
		return false;
	}


	D3DVIEWPORT9 vp;
	_pGraphicDevice->GetViewport(&vp);

	// 마우스의 좌표를 얻어옴
	POINT ptMousePos{};
	GetCursorPos(&ptMousePos);
	ScreenToClient(g_hWnd, &ptMousePos);

	// 마우스의 좌표를 NDC 상의 좌표로 바꿈
	D3DXVECTOR3 vMouseNDCPos{ 2.0f * (float)ptMousePos.x / (float)vp.Width - 1.0f,-2.0f * (float)ptMousePos.y / (float)vp.Height + 1.0f,0.0f };

	// 투영 행렬과 뷰 행렬을 얻어옴
	D3DXMATRIX matProj, matView;
	_pGraphicDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	_pGraphicDevice->GetTransform(D3DTS_VIEW, &matView);

	// 투영 행렬의 역행렬을 곱하여 뷰 스페이스 상의 좌표로 바꿈
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouseNDCPos, &vMouseNDCPos, &matProj);

	// 뷰 스페이스 상의 Ray의 시작점과 뱡향
	D3DXVECTOR3 Origin{ 0.0f,0.0f,0.0f }, Dir{ vMouseNDCPos };

	// 뷰 행렬의 역행렬을 곱하여 월드 스페이스 상의 좌표로 바꿈
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3TransformCoord(&Origin, &Origin, &matView);
	D3DXVec3TransformNormal(&Dir, &Dir, &matView);

	D3DXVECTOR3 vPickPos{ 0.0f,0.0f,0.0f };
	float fNear{ 1000.f };
	float u, v, dist;
	GameObject* pPickedObject{ nullptr };
	bool Picked{ false };
	for (auto mapiter = (*_pMap).begin(); mapiter != (*_pMap).end(); mapiter++)
	{
		for (list<GameObject*>::iterator lstiter = (*mapiter).second.begin(); lstiter != (*mapiter).second.end(); lstiter++)
		{
			VIBuffer* pVIBuffer = (*lstiter)->Get_Buffer();
			// VIBuffer를 가지는 객체인지 조사함
			if (nullptr != pVIBuffer)
			{
				// 객체의 Transform을 가져와서 월드 행렬을 얻음
				Transform* pTransform = (*lstiter)->Get_Transform();
				D3DXMATRIX matWorld = *(pTransform->Get_WorldMatrix());

				// 월드 스페이스의 역행렬을 곱하여 그 객체의 로컬 스페이스 상의 좌표로 바꿈
				D3DXVECTOR3 LocalOrigin, LocalDir;
				D3DXMatrixInverse(&matWorld, 0, &matWorld);
				D3DXVec3TransformCoord(&LocalOrigin, &Origin, &matWorld);
				D3DXVec3TransformNormal(&LocalDir, &Dir, &matWorld);

				// 로컬 스페이스 상의 Ray와 충돌을 비교함
				D3DXVECTOR3* pPos = pVIBuffer->Get_VtxPos();
				vector<INDEX32> vecIndex = pVIBuffer->GetIndex();
				UINT iTriNum = vecIndex.size();
				for (UINT i = 0; i < iTriNum; i++)
				{
					if (D3DXIntersectTri(&pPos[vecIndex[i]._0], &pPos[vecIndex[i]._1], &pPos[vecIndex[i]._2], &LocalOrigin, &LocalDir, &u, &v, &dist))
					{
						Picked = true;
						if (fNear > dist)
						{
							// V1 + U(V2 - V1) + V(V3 - V1)
							vPickPos = pPos[vecIndex[i]._0] + u * (pPos[vecIndex[i]._1] - pPos[vecIndex[i]._0]) + v * (pPos[vecIndex[i]._2] - pPos[vecIndex[i]._0]);
							fNear = dist;
							pPickedObject = *lstiter;
						}
					}
				}
			}
		}
	}

	if (Picked)
	{
		D3DXVec3TransformCoord(&vPickPos, &vPickPos, pPickedObject->Get_Transform()->Get_WorldMatrix());
		if (nullptr != _pVec3)
		{
			*_pVec3 = vPickPos;
			m_pTransform->Set_Pos(vPickPos.x, vPickPos.y, vPickPos.z);
		}
		(*_ppGameObject) = pPickedObject;
	}


	return Picked;	
}

bool PickingMgr::Picking_ColBox(LPDIRECT3DDEVICE9 _pGraphicDevice, list<ColliderBox*>* _pList, ColliderBox** _ppColBox, D3DXVECTOR3* _pVec3)
{
	if ((*_pList).empty())
		return false;

	D3DVIEWPORT9 vp;
	_pGraphicDevice->GetViewport(&vp);

	// 마우스의 좌표를 얻어옴
	POINT ptMousePos{};
	GetCursorPos(&ptMousePos);
	ScreenToClient(g_hWnd, &ptMousePos);

	// 마우스의 좌표를 NDC 상의 좌표로 바꿈
	D3DXVECTOR3 vMouseNDCPos{ 2.0f * (float)ptMousePos.x / (float)vp.Width - 1.0f, 
							 -2.0f * (float)ptMousePos.y / (float)vp.Height + 1.0f,
							 0.0f };

	// 투영 행렬과 뷰 행렬을 얻어옴
	D3DXMATRIX matProj, matView;
	_pGraphicDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	_pGraphicDevice->GetTransform(D3DTS_VIEW, &matView);

	// 투영 행렬의 역행렬을 곱하여 뷰 스페이스 상의 좌표로 바꿈
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouseNDCPos, &vMouseNDCPos, &matProj);

	// 뷰 스페이스 상의 Ray의 시작점과 뱡향
	D3DXVECTOR3 Origin{ 0.0f,0.0f,0.0f }, Dir{ vMouseNDCPos };

	// 뷰 행렬의 역행렬을 곱하여 월드 스페이스 상의 좌표로 바꿈
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3TransformCoord(&Origin, &Origin, &matView);
	D3DXVec3TransformNormal(&Dir, &Dir, &matView);

	D3DXVECTOR3 vPickPos{ 0.0f,0.0f,0.0f };
	float fNear{ 1000.f };
	float u, v, dist;
	ColliderBox* pPickedObject{ nullptr };
	bool Picked{ false };
	for (auto mapiter = (*_pList).begin(); mapiter != (*_pList).end(); mapiter++)
	{
			VIBuffer* pVIBuffer = (*mapiter)->Get_Buffer();
			// VIBuffer를 가지는 객체인지 조사함
			if (nullptr != pVIBuffer)
			{
				// 객체의 Transform을 가져와서 월드 행렬을 얻음
				Transform* pTransform = (*mapiter)->Get_Transform();
				D3DXMATRIX matWorld = *(pTransform->Get_WorldMatrix());

				// 월드 스페이스의 역행렬을 곱하여 그 객체의 로컬 스페이스 상의 좌표로 바꿈
				D3DXVECTOR3 LocalOrigin, LocalDir;
				D3DXMatrixInverse(&matWorld, 0, &matWorld);
				D3DXVec3TransformCoord(&LocalOrigin, &Origin, &matWorld);
				D3DXVec3TransformNormal(&LocalDir, &Dir, &matWorld);

				// 로컬 스페이스 상의 Ray와 충돌을 비교함
				D3DXVECTOR3* pPos = pVIBuffer->Get_VtxPos();
				vector<INDEX32> vecIndex = pVIBuffer->GetIndex();
				UINT iTriNum = vecIndex.size();
				for (UINT i = 0; i < iTriNum; i++)
				{
					if (D3DXIntersectTri(&pPos[vecIndex[i]._0], &pPos[vecIndex[i]._1], &pPos[vecIndex[i]._2], &LocalOrigin, &LocalDir, &u, &v, &dist))
					{
						Picked = true;
						if (fNear > dist)
						{
							// V1 + U(V2 - V1) + V(V3 - V1)
							vPickPos = pPos[vecIndex[i]._0] + u * (pPos[vecIndex[i]._1] - pPos[vecIndex[i]._0]) + v * (pPos[vecIndex[i]._2] - pPos[vecIndex[i]._0]);
							fNear = dist;
							pPickedObject = *mapiter;
						}
					}
				}
			}
 	}

	if (Picked)
	{
		D3DXVec3TransformCoord(&vPickPos, &vPickPos, pPickedObject->Get_Transform()->Get_WorldMatrix());
		if (nullptr != _pVec3)
		{
			*_pVec3 = vPickPos;
			m_pTransform->Set_Pos(vPickPos.x, vPickPos.y, vPickPos.z);
		}
		(*_ppColBox) = pPickedObject;
	}


	return Picked;

}

void PickingMgr::RenderPickingMgr(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	if (nullptr == m_pCubeCom)
	{
		m_pCubeCom = PickCube::Create(_pGraphicDevice);
	}
	if (nullptr == m_pTransform)
	{
		m_pTransform = Engine::Transform::Create(_pGraphicDevice);
		m_pTransform->Set_Scale(0.2f, 0.2f, 0.2f);
	}

	_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTransform->SettingMatrix();
	_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());
	_pGraphicDevice->SetTexture(0, nullptr);
	m_pCubeCom->Render_Buffer();

	_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void PickingMgr::Free()
{
	Safe_Release(m_pCubeCom);
	Safe_Release(m_pTransform);
}

