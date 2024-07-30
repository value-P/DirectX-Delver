#include "stdafx.h"
#include "ParsingMgr.h"
#include "MapObject.h"
#include "ParsingMgr.h"
#include "MapColObject.h"
#include "BillObj.h"
#include "Export_System.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(ParsingMgr)

ParsingMgr::ParsingMgr()
{
}

ParsingMgr::~ParsingMgr()
{
    Free();
}

HRESULT ParsingMgr::Setting_MapObjects(SCENETYPE eSceneType, Scene* pCurScene)
{
	switch (eSceneType)
	{
	case Engine::SCENE_LOGO:
		break;
	case Engine::SCENE_VILLEAGE:
		Load_GameObject(L"../../Resource/CloneDat/Villeage.dat", SCENE_VILLEAGE,pCurScene);
		Load_MapCollider(L"../../Resource/ColBoxDat/Villeage.dat", SCENE_VILLEAGE,pCurScene);
		break;
	case Engine::SCENE_STAGE1:
		Load_GameObject(L"../../Resource/CloneDat/Stage1.dat", SCENE_STAGE1,pCurScene);
		Load_MapCollider(L"../../Resource/ColBoxDat/Stage1.dat", SCENE_STAGE1,pCurScene);
		break;
	case Engine::SCENE_STAGE2:
		Load_GameObject(L"../../Resource/CloneDat/Stage2.dat", SCENE_STAGE2,pCurScene);
		Load_MapCollider(L"../../Resource/ColBoxDat/Stage2.dat", SCENE_STAGE2,pCurScene);
		break;
	case Engine::SCENE_STAGE3:
		break;
	case Engine::SCENE_STAGE4:
		break;
	case Engine::SCENE_BOSS:
		Load_GameObject(L"../../Resource/CloneDat/Boss.dat", SCENE_BOSS, pCurScene);
		Load_MapCollider(L"../../Resource/ColBoxDat/Boss.dat", SCENE_BOSS, pCurScene);
		break;
	}

    return S_OK;
}

_bool ParsingMgr::Load_Prefab(SCENETYPE eScene)
{
	wstring path;

	switch (eScene)
	{
	case Engine::SCENE_VILLEAGE:
		path = L"../../Resource/PrefabDat/Villeage/RectPrefab.dat";
		break;
	case Engine::SCENE_STAGE1:
		path = L"../../Resource/PrefabDat/Stage1/RectPrefab.dat";
		break;
	case Engine::SCENE_STAGE2:
		path = L"../../Resource/PrefabDat/Stage2/RectPrefab.dat";
		break;
	case Engine::SCENE_BOSS:
		path = L"../../Resource/PrefabDat/Boss/RectPrefab.dat";
		break;
	}

	// ∑∫∆Æ «¡∏Æ∆’ ¿˙¿Â
	{
		std::ifstream fin;
		fin.open(path, std::ios::binary);

		if (fin.fail())
		{
			fin.close();
			return false;
		}

		int iPrefabCnt;
		fin.read(reinterpret_cast<char*>(&iPrefabCnt), sizeof(int)); // ∫§≈Õ ªÁ¿Ã¡Ó

		GameObject* pObj = nullptr;

		for (int i = 0; i < iPrefabCnt; ++i)
		{
			Engine::PREFABDAT Dat;
			fin.read(reinterpret_cast<char*>(&Dat), sizeof(Engine::PREFABDAT));

			m_prefabList[eScene][PREFAB_RCTEX].push_back(Dat);
		}

		fin.close();
	}

	switch (eScene)
	{
	case Engine::SCENE_VILLEAGE:
		path = L"../../Resource/PrefabDat/Villeage/CubePrefab.dat";
		break;
	case Engine::SCENE_STAGE1:
		path = L"../../Resource/PrefabDat/Stage1/CubePrefab.dat";
		break;
	case Engine::SCENE_STAGE2:
		path = L"../../Resource/PrefabDat/Stage2/CubePrefab.dat";
		break;
	case Engine::SCENE_BOSS:
		path = L"../../Resource/PrefabDat/Boss/CubePrefab.dat";
		break;
	}

	// ≈•∫Í «¡∏Æ∆’ ¿˙¿Â
	{
		std::ifstream fin;
		fin.open(path, std::ios::binary);

		if (fin.fail())
		{
			fin.close();
			return false;
		}

		int iPrefabCnt;
		fin.read(reinterpret_cast<char*>(&iPrefabCnt), sizeof(int)); // ∫§≈Õ ªÁ¿Ã¡Ó

		GameObject* pObj = nullptr;

		for (int i = 0; i < iPrefabCnt; ++i)
		{
			Engine::PREFABDAT Dat;
			fin.read(reinterpret_cast<char*>(&Dat), sizeof(Engine::PREFABDAT));

			m_prefabList[eScene][PREFAB_CUBE].push_back(Dat);
		}

		fin.close();
	}

	switch (eScene)
	{
	case Engine::SCENE_VILLEAGE:
		path = L"../../Resource/PrefabDat/Villeage/BillBoardPrefab.dat";
		break;
	case Engine::SCENE_STAGE1:
		path = L"../../Resource/PrefabDat/Stage1/BillBoardPrefab.dat";
		break;
	case Engine::SCENE_STAGE2:
		path = L"../../Resource/PrefabDat/Stage2/BillBoardPrefab.dat";
		break;
	case Engine::SCENE_BOSS:
		path = L"../../Resource/PrefabDat/Boss/BillBoardPrefab.dat";
		break;
	}

	// ∫Ù∫∏µÂ «¡∏Æ∆’ ¿˙¿Â
	{
		std::ifstream fin;
		fin.open(path, std::ios::binary);

		if (fin.fail())
		{
			fin.close();
			return false;
		}

		int iPrefabCnt;
		fin.read(reinterpret_cast<char*>(&iPrefabCnt), sizeof(int)); // ∫§≈Õ ªÁ¿Ã¡Ó

		GameObject* pObj = nullptr;

		for (int i = 0; i < iPrefabCnt; ++i)
		{
			Engine::PREFABDAT Dat;
			fin.read(reinterpret_cast<char*>(&Dat), sizeof(Engine::PREFABDAT));

			m_prefabList[eScene][PREFAB_BILLBOARD].push_back(Dat);
		}

		fin.close();
	}

	return true;
}

_bool ParsingMgr::Load_GameObject(const wstring& DatPath, SCENETYPE eSceneType, Scene* pCurScene)
{
	std::ifstream fin;
	fin.open(DatPath, std::ios::binary);

	if (fin.fail())
	{
		return false;
	}

	int iPrefabCnt;
	fin.read(reinterpret_cast<char*>(&iPrefabCnt), sizeof(int)); // ∫§≈Õ ªÁ¿Ã¡Ó

	GameObject* pMapObject = nullptr;
	LPDIRECT3DDEVICE9 graphicDev = Engine::GraphicDev::GetInstance()->Get_GraphicDev();

	for (int i = 0; i < iPrefabCnt; ++i)
	{
		CLONEDAT Dat;
		fin.read(reinterpret_cast<char*>(&Dat), sizeof(CLONEDAT));

 		PREFABDAT PrefabDat = *(Find_PrefabDat(Dat.szOriginName, eSceneType));

		if (PrefabDat.eType == PREFAB_BILLBOARD)
		{
			_vec2 InitScale = { Dat.vScale.x,Dat.vScale.y };
			pMapObject = BillObj::Create(graphicDev, PrefabDat.szTexPath, InitScale);
			pMapObject->Get_Transform()->Set_Pos(Dat.vTransform);
			pMapObject->Get_Transform()->Set_Scale(Dat.vScale.x, Dat.vScale.y, Dat.vScale.z);
			pMapObject->Get_Transform()->Set_Rotation(&Dat.vRotation);
			if (FAILED(pCurScene->Get_Layer(LAYER_ENVI)->Add_GameObject(OBJ_MAP, pMapObject))) { MSG_BOX("Create Map Object Failed"); }
		}
		else
		{
			_vec2 InitScale = { Dat.vScale.x,Dat.vScale.y };
			pMapObject = MapObject::Create(graphicDev, PrefabDat.szTexPath, InitScale, PrefabDat.eType);
			pMapObject->Get_Transform()->Set_Pos(Dat.vTransform);
			pMapObject->Get_Transform()->Set_Scale(Dat.vScale.x, Dat.vScale.y, Dat.vScale.z);
			pMapObject->Get_Transform()->Set_Rotation(&Dat.vRotation);
			if (FAILED(pCurScene->Get_Layer(LAYER_ENVI)->Add_GameObject(OBJ_COLBOX, pMapObject))) { MSG_BOX("Create Map Object Failed"); };
		}
	}

	fin.close();

	return true;
}

_bool ParsingMgr::Load_MapCollider(const wstring& DatPath, SCENETYPE eSceneType, Scene* pCurScene)
{
	std::ifstream fin;
	fin.open(DatPath, std::ios::binary);

	if (fin.fail())
	{
		return false;
	}

	int iPrefabCnt;
	fin.read(reinterpret_cast<char*>(&iPrefabCnt), sizeof(int)); // ∫§≈Õ ªÁ¿Ã¡Ó

	MapColObject* pColBox = nullptr;
	LPDIRECT3DDEVICE9 graphicDev = Engine::GraphicDev::GetInstance()->Get_GraphicDev();

	for (int i = 0; i < iPrefabCnt; ++i)
	{
		COLBOXDAT Dat;
		fin.read(reinterpret_cast<char*>(&Dat), sizeof(COLBOXDAT));

		pColBox = MapColObject::Create(graphicDev, Dat.vPos, Dat.vScale);
		pCurScene->Get_Layer(LAYER_MAPCOL)->Add_GameObject(LAYER_MAPCOL, pColBox);
	}

	fin.close();

	return true;
}

PREFABDAT* ParsingMgr::Find_PrefabDat(const wstring& PrefabName, SCENETYPE eScene)
{
	for (int i = 0; i < PREFAB_END; ++i)
	{
		for (auto& iter : m_prefabList[eScene][i])
		{
			if (!lstrcmp(iter.szName, PrefabName.c_str()))
				return &iter;
		}
	}

	return nullptr;
}

void ParsingMgr::Free()
{
	for (int i = 0; i < SCENE_END; ++i)
	{
		for (int j = 0; j < PREFAB_END; ++j)
		{
			m_prefabList[i][j].clear();
		}
	}
}
