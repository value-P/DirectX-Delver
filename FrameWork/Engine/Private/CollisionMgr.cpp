#include "CollisionMgr.h"
#include "Collider.h"
#include "ColliderFrame.h"

CollisionMgr* CollisionMgr::m_pInstance{ nullptr };

Engine::CollisionMgr::CollisionMgr()
{
}

Engine::CollisionMgr::~CollisionMgr()
{
	Free();
}

void Engine::CollisionMgr::Init_ColMgr()
{
	ColSettingFunction();
}

void Engine::CollisionMgr::AddCollider(Collider* _pCollider)
{
	UINT Layer = _pCollider->GetColLayer();
	for (int i = 0; i < COL_END; i++)
	{
		if (Layer & 1 << i)
		{
			m_lstCollider[i].push_back(_pCollider);
			return;
		}
	}
}

void Engine::CollisionMgr::ComputeCollision()
{
	for (int i = 0; i < COL_END; i++)
	{
		for (int j = i; j < COL_END; j++)
		{
			if (m_ColLayer[i] & 1 << j)
			{
				if (i == j)
				{
					for (list<Collider*>::iterator iter = m_lstCollider[i].begin(); iter != m_lstCollider[i].end(); iter++)
					{
						for (list<Collider*>::reverse_iterator riter = m_lstCollider[j].rbegin(); riter != m_lstCollider[j].rend(); riter++)
						{
							if (*iter == *riter)
							{
								break;
							}
							D3DXVECTOR3 vNear{};
							if (ComputeColSphere(*iter, *riter, &vNear))
							{
								if (m_OBBOBBColLayer[i] & 1 << j)		// 둘다 OBB 충돌
								{
									m_lstOBBCollider.push_back({ *iter,*riter });
									(*iter)->AddColType(1 << j);
									(*iter)->AddColSphere(1 << j, (*riter)->GetCInfo(), vNear);
									(*riter)->AddColType(1 << i);
									(*riter)->AddColSphere(1 << i, (*iter)->GetCInfo(), vNear);
								}
								else if (m_OBBSPColLayer[i] & 1 << j)							// i는 OBB 충돌, j는 구 충돌
								{
									m_lstOSCollider.push_back({ *iter,*riter });
									(*iter)->AddColType(1 << j);
									(*iter)->AddColSphere(1 << j, (*riter)->GetCInfo(), vNear);
									(*riter)->AddColType(1 << i);
									(*riter)->AddColSphere(1 << i, (*iter)->GetCInfo(), vNear);
								}
								else if (m_SPOBBColLayer[i] & 1 << j)							// i는 구 충돌, j는 OBB 충돌
								{
									m_lstOSCollider.push_back({ *riter,*iter });
									(*iter)->AddColType(1 << j);
									(*iter)->AddColSphere(1 << j, (*riter)->GetCInfo(), vNear);
									(*riter)->AddColType(1 << i);
									(*riter)->AddColSphere(1 << i, (*iter)->GetCInfo(), vNear);
								}
								else															// 둘다 구 충돌
								{
									(*iter)->AddColType(1 << j);
									(*iter)->AddColSphere(1 << j, (*riter)->GetCInfo(), vNear);
									(*riter)->AddColType(1 << i);
									(*riter)->AddColSphere(1 << i, (*iter)->GetCInfo(), vNear);
								}
							}
						}
					}
				}
				else
				{
					for (list<Collider*>::iterator iter = m_lstCollider[i].begin(); iter != m_lstCollider[i].end(); iter++)
					{
						for (list<Collider*>::reverse_iterator riter = m_lstCollider[j].rbegin(); riter != m_lstCollider[j].rend(); riter++)
						{
							D3DXVECTOR3 vNear{};
							if (ComputeColSphere(*iter, *riter, &vNear))
							{
								if (m_OBBOBBColLayer[i] & 1 << j)		// 둘다 OBB 충돌
								{
									m_lstOBBCollider.push_back({ *iter,*riter });
									(*iter)->AddColType(1 << j);
									(*iter)->AddColSphere(1 << j, (*riter)->GetCInfo(), vNear);
									(*riter)->AddColType(1 << i);
									(*riter)->AddColSphere(1 << i, (*iter)->GetCInfo(), vNear);
								}
								else if (m_OBBSPColLayer[i] & 1 << j)							// i는 OBB 충돌, j는 구 충돌
								{
									m_lstOSCollider.push_back({ *iter,*riter });
									(*iter)->AddColType(1 << j);
									(*iter)->AddColSphere(1 << j, (*riter)->GetCInfo(), vNear);
									(*riter)->AddColType(1 << i);
									(*riter)->AddColSphere(1 << i, (*iter)->GetCInfo(), vNear);
								}
								else if (m_SPOBBColLayer[i] & 1 << j)							// i는 구 충돌, j는 OBB 충돌
								{
									m_lstOSCollider.push_back({ *riter,*iter });
									(*iter)->AddColType(1 << j);
									(*iter)->AddColSphere(1 << j, (*riter)->GetCInfo(), vNear);
									(*riter)->AddColType(1 << i);
									(*riter)->AddColSphere(1 << i, (*iter)->GetCInfo(), vNear);
								}
								else															// 둘다 구 충돌
								{
									(*iter)->AddColType(1 << j);
									(*iter)->AddColSphere(1 << j, (*riter)->GetCInfo(), vNear);
									(*riter)->AddColType(1 << i);
									(*riter)->AddColSphere(1 << i, (*iter)->GetCInfo(), vNear);
								}
							}
						}
					}
				}
			}
		}
	}

	for_each(m_lstOSCollider.begin(), m_lstOSCollider.end(),
		[&](pair<Collider*, Collider*> Temp)->void
		{
			D3DXVECTOR3 vNear{};
			if (ComputeColOBB_Sphere(Temp.first, Temp.second, &vNear))
			{
				Temp.first->AddColOBB(Temp.second->GetColLayer(), Temp.second->GetCInfo(), vNear);
				Temp.second->AddColOBB(Temp.first->GetColLayer(), Temp.first->GetCInfo(), vNear);
			}
		}
	);

	for_each(m_lstOBBCollider.begin(), m_lstOBBCollider.end(),
		[&](pair<Collider*, Collider*> Temp)->void
		{
			D3DXVECTOR3 vNear{};
			if (ComputeColOBB(Temp.first, Temp.second, &vNear))
			{
				Temp.first->AddColOBB(Temp.second->GetColLayer(), Temp.second->GetCInfo(), vNear);
				Temp.second->AddColOBB(Temp.first->GetColLayer(), Temp.first->GetCInfo(), vNear);
			}
		}
	);

}

void Engine::CollisionMgr::RenderColFrame(LPDIRECT3DDEVICE9 _pGraphicDevice)
{

	if (nullptr == m_pColBuffer)
	{
		m_pColBuffer = ColliderFrame::Create(_pGraphicDevice);
	}
	_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	_pGraphicDevice->SetTexture(0, nullptr);
	for (int i = 0; i < COL_END; i++)
	{
		if (!m_lstCollider[i].empty())
		{
			for (list<Collider*>::iterator iter = m_lstCollider[i].begin(); iter != m_lstCollider[i].end(); iter++)
			{
				_pGraphicDevice->SetTransform(D3DTS_WORLD, &(*iter)->GetWorld());
				m_pColBuffer->RenderBuffer((*iter)->GetColSphere(), (*iter)->GetColOBB());
			}
		}
	}
	_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void Engine::CollisionMgr::ClearColMgr()
{
	for (int i = 0; i < COL_END; i++)
	{
		m_lstCollider[i].clear();
	}
	m_lstOBBCollider.clear();
	m_lstOSCollider.clear();
}

bool Engine::CollisionMgr::ComputeColSphere(Collider* _pCollider1, Collider* _pCollider2, D3DXVECTOR3* _pNear)
{
	if ((_pCollider1->GetRadius() + _pCollider2->GetRadius()) * (_pCollider1->GetRadius() + _pCollider2->GetRadius()) > D3DXVec3LengthSq(&(_pCollider2->GetInfo(INFO_POS) - _pCollider1->GetInfo(INFO_POS))))
	{
		*_pNear = (_pCollider1->GetInfo(INFO_POS) + _pCollider2->GetInfo(INFO_POS)) * 0.5f;
		return true;
	}

	return false;
}

bool Engine::CollisionMgr::ComputeColOBB(Collider* _pCollider1, Collider* _pCollider2, D3DXVECTOR3* _vNear)
{
	D3DXVECTOR3 A[3], B[3], vA[3], vB[3], vDir;
	vA[0] = _pCollider1->GetInfo(INFO_RIGHT);
	vA[1] = _pCollider1->GetInfo(INFO_UP);
	vA[2] = _pCollider1->GetInfo(INFO_LOOK);
	vB[0] = _pCollider2->GetInfo(INFO_RIGHT);
	vB[1] = _pCollider2->GetInfo(INFO_UP);
	vB[2] = _pCollider2->GetInfo(INFO_LOOK);
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3Normalize(&A[i], &vA[i]);
		D3DXVec3Normalize(&B[i], &vB[i]);
	}
	vDir = _pCollider2->GetInfo(INFO_POS) - _pCollider1->GetInfo(INFO_POS);

	for (int i = 0; i < 3; i++)
	{
		if (abs(D3DXVec3Dot(&A[i], &vDir)) >
			(abs(D3DXVec3Dot(&A[i], &vA[0])) +
				abs(D3DXVec3Dot(&A[i], &vA[1])) +
					abs(D3DXVec3Dot(&A[i], &vA[2])) +
						abs(D3DXVec3Dot(&A[i], &vB[0])) +
							abs(D3DXVec3Dot(&A[i], &vB[1])) +
								abs(D3DXVec3Dot(&A[i], &vB[2]))
			))
		{
			return false;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (abs(D3DXVec3Dot(&B[i], &vDir)) >
			(abs(D3DXVec3Dot(&B[i], &vA[0])) +
				abs(D3DXVec3Dot(&B[i], &vA[1])) +
					abs(D3DXVec3Dot(&B[i], &vA[2])) +
						abs(D3DXVec3Dot(&B[i], &vB[0])) +
							abs(D3DXVec3Dot(&B[i], &vB[1])) +
								abs(D3DXVec3Dot(&B[i], &vB[2]))
								))
		{
			return false;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			D3DXVECTOR3 UCross;
			D3DXVec3Cross(&UCross, &A[i], &B[j]);
			D3DXVec3Normalize(&UCross, &UCross);
			if (abs(D3DXVec3Dot(&UCross, &vDir)) >
				(abs(D3DXVec3Dot(&UCross, &vA[0])) +
					abs(D3DXVec3Dot(&UCross, &vA[1])) +
						abs(D3DXVec3Dot(&UCross, &vA[2])) +
							abs(D3DXVec3Dot(&UCross, &vB[0])) +
								abs(D3DXVec3Dot(&UCross, &vB[1])) +
									abs(D3DXVec3Dot(&UCross, &vB[2]))
									))
			{
				return false;
			}
		}
	}

	*_vNear= (_pCollider1->GetInfo(INFO_POS) + _pCollider2->GetInfo(INFO_POS)) * 0.5f;
	return true;
}

bool Engine::CollisionMgr::ComputeColOBB_Sphere(Collider* _pOBB, Collider* _pSphere, D3DXVECTOR3* _vNear)
{
	D3DXVECTOR3 vOBB[3], vNearDot{ 0.0f,0.0f,0.0f };
	D3DXVECTOR3 vOBBPos = _pOBB->GetInfo(INFO_POS);
	D3DXVECTOR3 vSpherePos = _pSphere->GetInfo(INFO_POS);
	D3DXVECTOR3 vDir = vSpherePos - vOBBPos;
	float fRaidus = _pSphere->GetRadius();
	vOBB[0] = _pOBB->GetInfo(INFO_RIGHT);
	vOBB[1] = _pOBB->GetInfo(INFO_UP);
	vOBB[2] = _pOBB->GetInfo(INFO_LOOK);

	for (int i = 0; i < 3; i++)
	{
		if (0.0f > vOBB[i].x)
		{
			vOBB[i].x *= -1.0f;
		}
		if (0.0f > vOBB[i].y)
		{
			vOBB[i].y *= -1.0f;
		}
		if (0.0f > vOBB[i].z)
		{
			vOBB[i].z *= -1.0f;
		}
	}
	
	D3DXVECTOR3 vOBBTotal{ vOBB[0] + vOBB[1] + vOBB[2] };

	if (vOBBPos.x + vOBBTotal.x < vSpherePos.x)
	{
		vNearDot.x = vOBBPos.x + vOBBTotal.x;
	}
	else if (vOBBPos.x - vOBBTotal.x > vSpherePos.x)
	{
		vNearDot.x = vOBBPos.x - vOBBTotal.x;
	}
	else
	{
		vNearDot.x = vSpherePos.x;
	}

	if (vOBBPos.y + vOBBTotal.y < vSpherePos.y)
	{
		vNearDot.y = vOBBPos.y + vOBBTotal.y;
	}
	else if (vOBBPos.y - vOBBTotal.y > vSpherePos.y)
	{
		vNearDot.y = vOBBPos.y - vOBBTotal.y;
	}
	else
	{
		vNearDot.y = vSpherePos.y;
	}

	if (vOBBPos.z + vOBBTotal.z < vSpherePos.z)
	{
		vNearDot.z = vOBBPos.z + vOBBTotal.z;
	}
	else if (vOBBPos.z - vOBBTotal.z > vSpherePos.z)
	{
		vNearDot.z = vOBBPos.z - vOBBTotal.z;
	}
	else
	{
		vNearDot.z = vSpherePos.z;
	}

	if (D3DXVec3LengthSq(&(vSpherePos - vNearDot)) < fRaidus * fRaidus)
	{
		*_vNear = vNearDot;
		return true;
	}

	return false;
}

void Engine::CollisionMgr::ColSettingFunction()
{
	m_ColLayer[0] = COL_WALL | COL_DROPITEM | COL_MONSTERATTACK | COL_EXPLOSION | COL_ENVIRONMENT | COL_NPC | COL_MONSTER_PROJ | COL_INTERACTION | COL_DEBUFF;
	m_ColLayer[1] = COL_WALL | COL_PLAYERATTACK | COL_EXPLOSION | COL_ENVIRONMENT | COL_PLAYER_PROJ;
	m_ColLayer[2] = COL_DROPITEM | COL_ENVIRONMENT | COL_FRAGMENT | COL_PLAYER_PROJ | COL_MONSTER_PROJ;
	m_ColLayer[3] = COL_ENVIRONMENT;
	m_ColLayer[4] = COL_ENVIRONMENT;
	m_ColLayer[5] = COL_ENVIRONMENT;
	m_ColLayer[6] = COL_ENVIRONMENT;
	m_ColLayer[7] = COL_PLAYER_PROJ | COL_MONSTER_PROJ;
	m_ColLayer[8] = 0;
	m_ColLayer[9] = 0;
	m_ColLayer[10] = 0;
	m_ColLayer[11] = 0;
	m_ColLayer[12] = 0;
	m_ColLayer[13] = 0;

	m_OBBOBBColLayer[0] = COL_MONSTERATTACK | COL_MONSTER_PROJ | COL_DEBUFF;
	m_OBBOBBColLayer[1] = COL_PLAYERATTACK | COL_PLAYER_PROJ;
	m_OBBOBBColLayer[2] = 0;
	m_OBBOBBColLayer[3] = 0;
	m_OBBOBBColLayer[4] = 0;
	m_OBBOBBColLayer[5] = 0;
	m_OBBOBBColLayer[6] = 0;
	m_OBBOBBColLayer[7] = 0;
	m_OBBOBBColLayer[8] = 0;
	m_OBBOBBColLayer[9] = 0;
	m_OBBOBBColLayer[10] = 0;
	m_OBBOBBColLayer[11] = 0;
	m_OBBOBBColLayer[12] = 0;
	m_OBBOBBColLayer[13] = 0;

	m_OBBSPColLayer[0] = 0;
	m_OBBSPColLayer[1] = 0;
	m_OBBSPColLayer[2] = COL_DROPITEM | COL_ENVIRONMENT | COL_FRAGMENT | COL_PLAYER_PROJ | COL_MONSTER_PROJ;
	m_OBBSPColLayer[3] = 0;
	m_OBBSPColLayer[4] = 0;
	m_OBBSPColLayer[5] = 0;
	m_OBBSPColLayer[6] = 0;
	m_OBBSPColLayer[7] = 0;
	m_OBBSPColLayer[8] = 0;
	m_OBBSPColLayer[9] = 0;
	m_OBBSPColLayer[10] = 0;
	m_OBBSPColLayer[11] = 0;
	m_OBBSPColLayer[12] = 0;
	m_OBBSPColLayer[13] = 0;

	m_SPOBBColLayer[0] = COL_WALL;
	m_SPOBBColLayer[1] = COL_WALL;
	m_SPOBBColLayer[2] = 0;
	m_SPOBBColLayer[3] = 0;
	m_SPOBBColLayer[4] = 0;
	m_SPOBBColLayer[5] = 0;
	m_SPOBBColLayer[6] = 0;
	m_SPOBBColLayer[7] = 0;
	m_SPOBBColLayer[8] = 0;
	m_SPOBBColLayer[9] = 0;
	m_SPOBBColLayer[10] = 0;
	m_SPOBBColLayer[11] = 0;
	m_SPOBBColLayer[12] = 0;
	m_SPOBBColLayer[13] = 0;
}

void Engine::CollisionMgr::Free()
{
	for (int i = 0; i < COL_END; i++)
	{
		m_lstCollider[i].clear();
	}
	m_lstOBBCollider.clear();
	m_lstOSCollider.clear();
	Safe_Release(m_pColBuffer);
}

