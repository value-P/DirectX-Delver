#include "pch.h"
#include "PrefabMgr.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(PrefabMgr)

PrefabMgr::PrefabMgr()
{
}

PrefabMgr::~PrefabMgr()
{
    Free();
}

HRESULT PrefabMgr::Init_PrefabMgr()
{
    Load_RcPrefab();

    return S_OK;
}

void PrefabMgr::Add_Prefab(PREFABTYPE eType, PREFABDAT prefab, SCENETYPE eScene)
{

    auto iter = find_if(m_PrefabList[eScene][eType].begin(), m_PrefabList[eScene][eType].end(), [&prefab](PREFABDAT temp)->bool
        {
            wstring name(temp.szName);
            wstring input(prefab.szName);

            if (input == L"") return false;

            return name == input && temp.eType == prefab.eType;
        });

    if (iter != m_PrefabList[eScene][eType].end())
    {
        MSG_BOX("이미 존재하는 이름입니다");
        return;
    }

    m_PrefabList[eScene][eType].push_back(prefab);
    MSG_BOX("프리팹 생성 성공");
}

HRESULT PrefabMgr::Load_RcPrefab()
{
    // 사각형 프리팹 데이터
    Read_RcPrefabFile(L"../../Resource/PrefabDat/Villeage/RectPrefab.dat", PREFAB_RCTEX, SCENE_VILLEAGE);
    Read_RcPrefabFile(L"../../Resource/PrefabDat/Stage1/RectPrefab.dat", PREFAB_RCTEX, SCENE_STAGE1);
    Read_RcPrefabFile(L"../../Resource/PrefabDat/Stage2/RectPrefab.dat", PREFAB_RCTEX, SCENE_STAGE2);
    Read_RcPrefabFile(L"../../Resource/PrefabDat/Boss/RectPrefab.dat", PREFAB_RCTEX, SCENE_BOSS);
    // 큐브 프리팹 데이터
    Read_RcPrefabFile(L"../../Resource/PrefabDat/Villeage/CubePrefab.dat", PREFAB_CUBE, SCENE_VILLEAGE);
    Read_RcPrefabFile(L"../../Resource/PrefabDat/Stage1/CubePrefab.dat", PREFAB_CUBE, SCENE_STAGE1);
    Read_RcPrefabFile(L"../../Resource/PrefabDat/Stage2/CubePrefab.dat", PREFAB_CUBE, SCENE_STAGE2);
    Read_RcPrefabFile(L"../../Resource/PrefabDat/Boss/CubePrefab.dat", PREFAB_CUBE, SCENE_BOSS);
    // 빌보드 프리팹 데이터
    Read_RcPrefabFile(L"../../Resource/PrefabDat/Villeage/BillBoardPrefab.dat", PREFAB_BILLBOARD, SCENE_VILLEAGE);
    Read_RcPrefabFile(L"../../Resource/PrefabDat/Stage1/BillBoardPrefab.dat", PREFAB_BILLBOARD, SCENE_STAGE1);
    Read_RcPrefabFile(L"../../Resource/PrefabDat/Stage2/BillBoardPrefab.dat", PREFAB_BILLBOARD, SCENE_STAGE2);
    Read_RcPrefabFile(L"../../Resource/PrefabDat/Boss/BillBoardPrefab.dat", PREFAB_BILLBOARD, SCENE_BOSS);

    return S_OK;
}

void PrefabMgr::Save_Prefab(SCENETYPE eScene)
{
    switch (eScene)
    {
    case Engine::SCENE_VILLEAGE:
        if (!Write_PrefabFile(L"../../Resource/PrefabDat/Villeage/BillBoardPrefab.dat", PREFAB_BILLBOARD, SCENE_VILLEAGE)) { MSG_BOX("Save BillBoardPrefab Failed"); };
        if (!Write_PrefabFile(L"../../Resource/PrefabDat/Villeage/RectPrefab.dat", PREFAB_RCTEX, SCENE_VILLEAGE)) { MSG_BOX("Save RectPrefab Failed"); };
        if (!Write_PrefabFile(L"../../Resource/PrefabDat/Villeage/CubePrefab.dat", PREFAB_CUBE, SCENE_VILLEAGE)) { MSG_BOX("Save CubePrefab Failed"); };
        break;
    case Engine::SCENE_STAGE1:
        if (!Write_PrefabFile(L"../../Resource/PrefabDat/Stage1/BillBoardPrefab.dat", PREFAB_BILLBOARD, SCENE_STAGE1)) { MSG_BOX("Save BillBoardPrefab Failed"); };
        if (!Write_PrefabFile(L"../../Resource/PrefabDat/Stage1/RectPrefab.dat", PREFAB_RCTEX, SCENE_STAGE1)) { MSG_BOX("Save RectPrefab Failed"); };
        if (!Write_PrefabFile(L"../../Resource/PrefabDat/Stage1/CubePrefab.dat", PREFAB_CUBE, SCENE_STAGE1)) { MSG_BOX("Save CubePrefab Failed"); };
        break;
    case Engine::SCENE_STAGE2:
        if (!Write_PrefabFile(L"../../Resource/PrefabDat/Stage2/BillBoardPrefab.dat", PREFAB_BILLBOARD, SCENE_STAGE2)) { MSG_BOX("Save BillBoardPrefab Failed"); };
        if (!Write_PrefabFile(L"../../Resource/PrefabDat/Stage2/RectPrefab.dat", PREFAB_RCTEX, SCENE_STAGE2)) { MSG_BOX("Save RectPrefab Failed"); };
        if (!Write_PrefabFile(L"../../Resource/PrefabDat/Stage2/CubePrefab.dat", PREFAB_CUBE, SCENE_STAGE2))  { MSG_BOX("Save CubePrefab Failed"); };
        break;
    case Engine::SCENE_STAGE3:
        break;
    case Engine::SCENE_STAGE4:
        break;
    case Engine::SCENE_BOSS:
        if (!Write_PrefabFile(L"../../Resource/PrefabDat/Boss/BillBoardPrefab.dat", PREFAB_BILLBOARD, SCENE_BOSS)) { MSG_BOX("Save BillBoardPrefab Failed"); };
        if (!Write_PrefabFile(L"../../Resource/PrefabDat/Boss/RectPrefab.dat", PREFAB_RCTEX, SCENE_BOSS))     { MSG_BOX("Save RectPrefab Failed"); };
        if (!Write_PrefabFile(L"../../Resource/PrefabDat/Boss/CubePrefab.dat", PREFAB_CUBE, SCENE_BOSS))      { MSG_BOX("Save CubePrefab Failed"); };
        break;
    }

    MSG_BOX("Save Succeed");
}

const PREFABDAT* PrefabMgr::Find_OriginPrefab(const wstring& szPrefabName, SCENETYPE eScene)
{
    for (int i = 0; i < PREFAB_END; ++i)
    {
        for (auto& iter : m_PrefabList[eScene][i])
        {
            if (!lstrcmp(iter.szName, szPrefabName.c_str()))
            {
                return &iter;
            }
        }
    }

    return nullptr;
}

bool PrefabMgr::Write_PrefabFile(const wstring& szSavePath, PREFABTYPE eType, SCENETYPE eScene)
{
    std::ofstream fout;

    fout.open(szSavePath, std::ios::binary);

    if (fout.fail())
        return false;

    int iPrefabCnt = m_PrefabList[eScene][eType].size();
    fout.write(reinterpret_cast<char*>(&iPrefabCnt), sizeof(int)); // 벡터 사이즈

    auto iter = m_PrefabList[eScene][eType].begin();
    for (int i = 0; i < iPrefabCnt; ++i)
    {
        PREFABDAT Dat = *iter;
        fout.write(reinterpret_cast<char*>(&Dat), sizeof(PREFABDAT));

        ++iter;
    }

    fout.close();

    return true;
}

bool PrefabMgr::Read_RcPrefabFile(const wstring& szReadPath, PREFABTYPE eType, SCENETYPE eScene)
{
    std::ifstream fin;

    fin.open(szReadPath, std::ios::binary);

    if (fin.fail())
        return false;

    int iPrefabCnt;
    fin.read(reinterpret_cast<char*>(&iPrefabCnt), sizeof(int)); // 벡터 사이즈
    
    for (int i = 0; i < iPrefabCnt; ++i)
    {
        PREFABDAT Dat;
        fin.read(reinterpret_cast<char*>(&Dat), sizeof(PREFABDAT));
        m_PrefabList[eScene][eType].push_back(Dat);
    }

    fin.close();
    return true;
}

void PrefabMgr::Free()
{
    for (int i = 0; i < SCENE_END; ++i)
    {
        for (int j = 0; j < PREFAB_END; ++j)
        {
            m_PrefabList[i][j].clear();
        }
    }
}
