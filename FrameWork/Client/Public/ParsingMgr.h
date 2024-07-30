#pragma once
#include "Base.h"
#include "Engine_Define.h"

namespace Engine
{
	class Scene;
}

class ParsingMgr : public Base
{
	DECLARE_SINGLETON(ParsingMgr)

private:
	explicit ParsingMgr();
	virtual ~ParsingMgr();

public:
	HRESULT			Setting_MapObjects(SCENETYPE eSceneType, Scene* pCurScene);
	
	_bool			Load_Prefab(SCENETYPE eScene);

private:
	_bool			Load_GameObject (const wstring& DatPath, SCENETYPE eSceneType, Scene* pCurScene);
	_bool			Load_MapCollider(const wstring& DatPath, SCENETYPE eSceneType, Scene* pCurScene);

	PREFABDAT*		Find_PrefabDat(const wstring& PrefabName, SCENETYPE eScene);

private:
	list<PREFABDAT> m_prefabList[SCENE_END][PREFAB_END]; // ť��/��Ʈ ������ ���� ����Ʈ

private:
	virtual void		Free();

};

