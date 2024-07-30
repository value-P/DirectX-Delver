#pragma once
#include "Engine_Define.h"

class PrefabMgr
{
	DECLARE_SINGLETON(PrefabMgr)

private:
	explicit PrefabMgr();
	virtual ~PrefabMgr();

public:
	HRESULT	Init_PrefabMgr();

	void	Add_Prefab(PREFABTYPE eType, PREFABDAT prefab, SCENETYPE eScene);
	void	Save_Prefab(SCENETYPE eScene);

	const PREFABDAT*	Find_OriginPrefab(const wstring& szPrefabName, SCENETYPE eScene);

public:
	list<PREFABDAT> m_PrefabList[SCENE_END][PREFAB_END];


private:
	HRESULT Load_RcPrefab();
	bool Write_PrefabFile(const wstring& szSavePath, PREFABTYPE eType, SCENETYPE eScene);
	bool Read_RcPrefabFile(const wstring& szReadPath, PREFABTYPE eType, SCENETYPE eScene);

private:
	virtual void Free();

};

