#include "Management.h"
#include "Export_System.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(Management)

Management::Management() : m_pScene(nullptr)
{
}

Management::~Management()
{
	Free();
}

_int Management::Update_Scene(const _float& fTimeDelta, HWND hWnd)
{
	int i{ -1 };
	if (m_pScene)
	{
		if (m_pScene->IsFinishLoading())
		{
			i = m_pScene->Update_Scene(fTimeDelta);
			Engine::UIManager::GetInstance()->Update_UI(fTimeDelta, hWnd);
			Engine::PSMgr::GetInstance()->UpdatePSMgr(fTimeDelta);
			Engine::CollisionMgr::GetInstance()->ComputeCollision();
		}
	}

	return i;
}

void Management::LateUpdate_Scene()
{
	if (m_pScene)
	{
		if (m_pScene->IsFinishLoading())
		{
			if (SingleCamera::GetInstance()->IsPlaying())
			{
				SingleCamera::GetInstance()->PlayCameraDirection();
			}
			else
			{
				SingleCamera::GetInstance()->PlayerViewpoint();
			}
			m_pScene->LateUpdate_Scene();
			Engine::UIManager::GetInstance()->LateUpdate_UI();
			Engine::PSMgr::GetInstance()->LateUpdatePSMgr();
		}
	}
}

void Management::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (m_pScene)
	{
		if (m_pScene->IsFinishLoading())
		{
			Engine::Render_GameObject(pGraphicDev);
			//Engine::CollisionMgr::GetInstance()->RenderColFrame(pGraphicDev);
			Engine::CollisionMgr::GetInstance()->ClearColMgr();
			m_pScene->Render_Scene();
		}
		else
		{
			m_pLoadingImg->Render_GameObject();
			m_pScene->CheckLoading();
		}
	}

}

Component* Engine::Management::Get_Component(const wstring pLayerTag, const wstring pObjTag, const wstring pComponentTag, COMPONENTID eID)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

Layer* Engine::Management::Get_Layer(const wstring pLayerTag)
{
	if (m_pScene == nullptr)
		return nullptr;

	return m_pScene->Get_Layer(pLayerTag);
}

vector<GameObject*>* Management::Get_GameObject(const wstring pLayerTag, const wstring pObjTag)
{
	if (m_pScene == nullptr)
		return nullptr;

	return m_pScene->Get_GameObject(pLayerTag, pObjTag);
}

HRESULT Management::Set_Scene(Scene * pScene)
{
	Safe_Release(m_pScene);

	Engine::Clear_RenderGroup();
	Engine::CollisionMgr::GetInstance()->ClearColMgr();
	Engine::PSMgr::GetInstance()->PSystem_Clear();
	Engine::SingleCamera::GetInstance()->StopCameraDirection();

	m_pScene = pScene;

	return S_OK;
}

void Management::SetLoadingImg(GameObject* _pLoadingImg)
{
	if (_pLoadingImg)
	{
		m_pLoadingImg = _pLoadingImg;
	}
}

// ray : 광선충돌의 주체가 될 객체의 주소
// pPicekdObj : 광선 충돌시 충돌한 객체를 넘겨주는 매개변수
// pPickedPos : 광선 충돌된 정확한 지점을 반환하는 매개변수
// maxDist : 광선이 충돌할 수 있는 최대 길이
bool Management::RayCast(Ray* ray, GameObject** pPickedObj, _vec3* pPickedPos, _float maxDist, const wstring& layerTag)
{
	if (!ray || !pPickedPos) return false;

	Layer* layer = m_pScene->Get_Layer(layerTag);
	
	if (!layer) return false;

	auto GameObjects = layer->Get_ObjectMap();

	VIBuffer* pBuffer = nullptr;
	Transform* pTransform = nullptr;
	const _matrix* pMatWorld = nullptr;
	_matrix	matInverse;
	float u, v, dist;
	bool Picked = false;
	float fNear = maxDist;

	for (auto& iter : *GameObjects)
	{
		pBuffer = iter.second->Get_Buffer();
		if (pBuffer)
		{
			// 월드행렬 받아오기
			pTransform = iter.second->Get_Transform();
			pMatWorld = pTransform->Get_WorldMatrix();

			// 객체의 월드 스페이스 역행렬로 ray를 로컬좌표로 내리기
			_vec3 vLocalOrigin, vLocalDir;
			D3DXMatrixInverse(&matInverse, 0, pMatWorld);
			D3DXVec3TransformCoord(&vLocalOrigin, &(ray->Get_Origin()),&matInverse);
			D3DXVec3TransformNormal(&vLocalDir, &(ray->Get_Dir()), &matInverse);

			// 로컬 스페이스의 Ray와 충돌 비교
			_vec3* pPos = pBuffer->Get_VtxPos();
			auto vecIndex = pBuffer->GetIndex();
			size_t iTriCnt = vecIndex.size();
			for (size_t i = 0; i < iTriCnt; ++i)
			{
				if (D3DXIntersectTri(&pPos[vecIndex[i]._0], &pPos[vecIndex[i]._1], &pPos[vecIndex[i]._2], &vLocalOrigin, &vLocalDir, &u, &v, &dist))
				{
					//if (dist > maxDist) continue;
					
					
					if (fNear > dist)
					{
						Picked = true;
						// V1 + U(V2 - V1) + V(V3 - V1)
						_vec3 pickedLocal = pPos[vecIndex[i]._0] + u * (pPos[vecIndex[i]._1] - pPos[vecIndex[i]._0]) + v * (pPos[vecIndex[i]._2] - pPos[vecIndex[i]._0]);
						fNear = dist;
						*pPickedObj = iter.second;
						D3DXVec3TransformCoord(pPickedPos, &pickedLocal, pMatWorld);
					}
				}
			}
		}
	}

	return Picked;

}

void Management::Free()
{
	Safe_Release(m_pLoadingImg);
	Safe_Release(m_pScene);
}
