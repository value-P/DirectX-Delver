#include "pch.h"
#include "DataMgr.h"
#include "Texture.h"
#include "GraphicDev.h"

IMPLEMENT_SINGLETON(DataMgr)

DataMgr::DataMgr()
{
}

DataMgr::~DataMgr()
{
	Free();
}

HRESULT DataMgr::Init_DataMgr()
{
	Read_ImgFile(L"../../Resource/Texture/Rect", PREFAB_RCTEX);
	Read_ImgFile(L"../../Resource/Texture/Cube", PREFAB_CUBE);
	Read_ImgFile(L"../../Resource/Texture/BillBoard", PREFAB_BILLBOARD);

	return S_OK;
}

void DataMgr::Read_ImgFile(const wstring& wsPath, PREFABTYPE eType)
{
	filesystem::path path = wsPath;
	filesystem::directory_iterator iter(path);

	while (iter != filesystem::end(iter))
	{
		Texture* pTex;

		if ((*iter).is_regular_file())
		{
			wstring path = (*iter).path().wstring();
			string ObjName = (*iter).path().stem().string();

			if (eType == PREFAB_CUBE)
				pTex = Texture::Create(GraphicDev::GetInstance()->Get_GraphicDev(), TEX_CUBE, path);
			else
				pTex = Texture::Create(GraphicDev::GetInstance()->Get_GraphicDev(), TEX_NORMAL, path);

			TEXDAT texDat;

			texDat.texName = ObjName.c_str();
			texDat.texPath = path;
			texDat.pTex = pTex;

			m_mapTexture[eType].emplace(ObjName,texDat);
		}

		++iter;
	}
}

void DataMgr::Free()
{
	for (int i = 0; i < PREFAB_END; ++i)
	{
		for (auto& iter : m_mapTexture[i])
			Safe_Release(iter.second.pTex);

		m_mapTexture[i].clear();
	}
}
