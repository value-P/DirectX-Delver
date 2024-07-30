#pragma once
#include "Engine_Define.h"

class DataMgr
{
	DECLARE_SINGLETON(DataMgr)

private:
	explicit DataMgr();
	virtual ~DataMgr();

public:
	HRESULT	Init_DataMgr();

public:
	map<const string, TEXDAT>* Get_TexMap() { return m_mapTexture; }

private:
	void Read_ImgFile(const wstring& wsPath, PREFABTYPE eType);

private:
	map<const string, TEXDAT> m_mapTexture[PREFAB_END];

private:
	virtual void		Free();
};

