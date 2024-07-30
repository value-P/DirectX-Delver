#include "pch.h"
#include "MainEditor.h"
#include "MainUI.h"
#include "DynamicCamera_Tool.h"
#include "PrefabMgr.h"
#include "DataMgr.h"
#include "PickingMgr.h"
#include "Export_System.h"
#include "CloneRect.h"
#include "Cube.h"
#include "ColliderBox.h"
#include "BillBoardObj.h"

MainEditor::MainEditor() : m_pDeviceClass(nullptr), m_pGraphicDev(nullptr)
{
}

MainEditor::~MainEditor()
{
}

HRESULT MainEditor::Init_MainEditor()
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);

	return S_OK;
}

int MainEditor::Update_MainEditor(const float& fTimeDelta)
{
	Engine::Update_InputDev();
		
	m_pCamera->Update_GameObject(fTimeDelta);

	Key_Input();

	Update_ImGui(fTimeDelta);
	Update_Object(fTimeDelta);

	return 0;
}

void MainEditor::Key_Input()
{
	if (Engine::Get_DIMouseState(DIM_RB) & 0x80)
	{
		Mouse_Picking();
	}


	// 모드에 따라 피킹한 오브젝트 화살표로 이동 / 돌리기(게임오브젝트만)
	if (m_bColliderEditMode)
	{
		if (m_pPickedColBox)
		{
			// X누르면서 상하(Y축이동) 좌우(X축이동) 2.f씩 이동
			if (Engine::Get_DIKeyState(DIK_X) & 0x80)
			{
				if (KeyMgr::GetInstance()->Key_Down(VK_RIGHT))
				{
					_vec3 Right = { 1.f,0.f,0.f };
					m_pPickedColBox->Get_Transform()->Move_Forward(&Right, 1.f, 1.f);
				}
				else if (KeyMgr::GetInstance()->Key_Down(VK_LEFT))
				{
					_vec3 Left = { -1.f,0.f,0.f };
					m_pPickedColBox->Get_Transform()->Move_Forward(&Left, 1.f, 1.f);
				}
				else if (KeyMgr::GetInstance()->Key_Down(VK_UP))
				{
					_vec3 Up = { 0.f,1.f,0.f };
					m_pPickedColBox->Get_Transform()->Move_Forward(&Up, 1.f, 1.f);
				}
				else if (KeyMgr::GetInstance()->Key_Down(VK_DOWN))
				{
					_vec3 Down = { 0.f,-1.f,0.f };
					m_pPickedColBox->Get_Transform()->Move_Forward(&Down, 1.f, 1.f);
				}
			}

			// Z 누르면서 위아래 방향키로 z축이동 가능
			if (Engine::Get_DIKeyState(DIK_Z) & 0x80)
			{
				if (KeyMgr::GetInstance()->Key_Down(VK_UP))
				{
					_vec3 Front = { 0.f,0.f,1.f };
					m_pPickedColBox->Get_Transform()->Move_Forward(&Front, 1.f, 1.f);
				}
				else if (KeyMgr::GetInstance()->Key_Down(VK_DOWN))
				{
					_vec3 Back = { 0.f,0.f,-1.f };
					m_pPickedColBox->Get_Transform()->Move_Forward(&Back, 1.f, 1.f);
				}

			}

			if (Engine::Key_Down(VK_F2))
			{
				Erase_ColBox(m_pPickedColBox);
				m_pPickedColBox = nullptr;
			}
		}
	}
	else
	{
		// 키보드로 오브젝트 이동
		if (m_pPickedObj)
		{
			// X누르면서 상하(Y축이동) 좌우(X축이동) 2.f씩 이동
			if (Engine::Get_DIKeyState(DIK_X) & 0x80)
			{
				if (KeyMgr::GetInstance()->Key_Down(VK_RIGHT))
				{
					_vec3 Right = { 1.f,0.f,0.f };
					m_pPickedObj->Get_Transform()->Move_Forward(&Right, 1.f, 1.f);
				}
				else if (KeyMgr::GetInstance()->Key_Down(VK_LEFT))
				{
					_vec3 Left = { -1.f,0.f,0.f };
					m_pPickedObj->Get_Transform()->Move_Forward(&Left, 1.f, 1.f);
				}
				else if (KeyMgr::GetInstance()->Key_Down(VK_UP))
				{
					_vec3 Up = { 0.f,1.f,0.f };
					m_pPickedObj->Get_Transform()->Move_Forward(&Up, 1.f, 1.f);
				}
				else if (KeyMgr::GetInstance()->Key_Down(VK_DOWN))
				{
					_vec3 Down = { 0.f,-1.f,0.f };
					m_pPickedObj->Get_Transform()->Move_Forward(&Down, 1.f, 1.f);
				}
			}

			// Z 누르면서 위아래 방향키로 z축이동 가능
			if (Engine::Get_DIKeyState(DIK_Z) & 0x80)
			{
				if (KeyMgr::GetInstance()->Key_Down(VK_UP))
				{
					_vec3 Front = { 0.f,0.f,1.f };
					m_pPickedObj->Get_Transform()->Move_Forward(&Front, 1.f, 1.f);
				}
				else if (KeyMgr::GetInstance()->Key_Down(VK_DOWN))
				{
					_vec3 Back = { 0.f,0.f,-1.f };
					m_pPickedObj->Get_Transform()->Move_Forward(&Back, 1.f, 1.f);
				}

			}

			// R 누르면서 위아래 방향키로 x축회전 / 좌우키로 y축회전
			if (Engine::Get_DIKeyState(DIK_R) & 0x80)
			{
				if (KeyMgr::GetInstance()->Key_Down(VK_RIGHT))
				{
					m_pPickedObj->Get_Transform()->Rotation(ROT_Y, D3DXToRadian(-90));
				}
				else if (KeyMgr::GetInstance()->Key_Down(VK_LEFT))
				{
					m_pPickedObj->Get_Transform()->Rotation(ROT_Y, D3DXToRadian(90));
				}
				else if (KeyMgr::GetInstance()->Key_Down(VK_UP))
				{
					m_pPickedObj->Get_Transform()->Rotation(ROT_X, D3DXToRadian(90));
				}
				else if (KeyMgr::GetInstance()->Key_Down(VK_DOWN))
				{
					m_pPickedObj->Get_Transform()->Rotation(ROT_X, D3DXToRadian(-90));
				}
			}

			// 삭제
			if (Engine::Key_Down(VK_F2))
			{
				Erase_GameObject(m_pPickedObj);
				m_pPickedColBox = nullptr;
			}
		}
	}

	// 카메라 속도 조절
	if (Engine::KeyMgr::GetInstance()->Key_Down(VK_PRIOR))
	{
		m_pCamera->m_fSpeed += 1.f;
	}
	else if (Engine::KeyMgr::GetInstance()->Key_Down(VK_NEXT))
	{
		if (m_pCamera->m_fSpeed >= 1.f)
			m_pCamera->m_fSpeed -= 1.f;
	}

	if (Engine::Key_Down(VK_F1))
	{
		Save_GameObject();
		Save_ColBox();
	}
}

void MainEditor::LateUpdate_MainEditor()
{
	m_pCamera->LateUpdate_GameObject();

	for (auto& iter : m_mapScene[m_eCurrentScene])
	{
		for (auto& iter2 : iter.second)
			iter2->LateUpdate_GameObject();
	}
}

void MainEditor::Render_MainEditor()
{
	Engine::Render_Begin(D3DXCOLOR(0.45f, 0.55f, 0.60f, 1.00f));

	Render_Object();
	PickingMgr::GetInstance()->RenderPickingMgr(m_pGraphicDev);
	Render_ImGui();

	Engine::Render_End();
}

void MainEditor::Mouse_Picking()
{
	_bool result;

	if (m_bColliderEditMode)
	{
		result = PickingMgr::GetInstance()->Picking_ColBox(m_pGraphicDev, &m_colBoxList[m_eCurrentScene], &m_pPickedColBox, &m_fPickedPos);
		if (!result) m_pPickedColBox = nullptr;
	}
	else
	{
		result = PickingMgr::GetInstance()->Picking(m_pGraphicDev, &m_mapScene[m_eCurrentScene], &m_pPickedObj, &m_fPickedPos);
		if (!result) m_pPickedObj = nullptr;
	}
}

void MainEditor::Add_GameObject(const wstring& layerTag, GameObject* pObject)
{
	auto iter = m_mapScene[m_eCurrentScene].find(layerTag);

	if (iter == m_mapScene[m_eCurrentScene].end())
	{
		list<GameObject*> newlist;
		newlist.push_back(pObject);
		m_mapScene[m_eCurrentScene].emplace(layerTag, newlist);
	}
	else
	{
		m_mapScene[m_eCurrentScene][layerTag].push_back(pObject);
	}

}

void MainEditor::Add_GameObject(SCENETYPE eSceneType, const wstring& layerTag, GameObject* pObject)
{
	auto iter = m_mapScene[eSceneType].find(layerTag);

	if (iter == m_mapScene[eSceneType].end())
	{
		list<GameObject*> newlist;
		newlist.push_back(pObject);
		m_mapScene[eSceneType].emplace(layerTag, newlist);
	}
	else
	{
		m_mapScene[eSceneType][layerTag].push_back(pObject);
	}

}

void MainEditor::Add_ColliderBox(ColliderBox* pColBox)
{
	m_colBoxList[m_eCurrentScene].push_back(pColBox);
}

void MainEditor::Add_ColliderBox(SCENETYPE eSceneType, ColliderBox* pColBox)
{
	m_colBoxList[eSceneType].push_back(pColBox);
}

_bool MainEditor::Erase_GameObject(GameObject* pObject)
{
	if (!pObject) return false;

	for (auto& iter : m_mapScene[m_eCurrentScene])
	{
		auto iter2 = find(iter.second.begin(), iter.second.end(), pObject);
		if (iter2 != iter.second.end())
		{
			if (pObject == m_pPickedObj)
				m_pPickedObj = nullptr;

			Safe_Release(*iter2);
			iter.second.erase(iter2);
			return true;
		}
	}	

	return false;
}

_bool MainEditor::Save_GameObject()
{
	std::ofstream fout;

	switch (m_eCurrentScene)
	{
	case Engine::SCENE_LOGO:
		break;
	case Engine::SCENE_VILLEAGE:
		fout.open(L"../../Resource/CloneDat/Villeage.dat", std::ios::binary);
		break;
	case Engine::SCENE_STAGE1:
		fout.open(L"../../Resource/CloneDat/Stage1.dat", std::ios::binary);
		break;
	case Engine::SCENE_STAGE2:
		fout.open(L"../../Resource/CloneDat/Stage2.dat", std::ios::binary);
		break;
	case Engine::SCENE_STAGE3:
		fout.open(L"../../Resource/CloneDat/Stage3.dat", std::ios::binary);
		break;
	case Engine::SCENE_STAGE4:
		fout.open(L"../../Resource/CloneDat/Stage4.dat", std::ios::binary);
		break;
	case Engine::SCENE_BOSS:
		fout.open(L"../../Resource/CloneDat/Boss.dat", std::ios::binary);
		break;
	}

	if (fout.fail())
	{
		return false;
	}

	int iPrefabCnt(0);
	for (auto iter : m_mapScene[m_eCurrentScene])
	{
		iPrefabCnt += iter.second.size();
	}

	fout.write(reinterpret_cast<char*>(&iPrefabCnt), sizeof(int)); // 벡터 사이즈
	CLONEDAT cloneDat;

	for (auto iter : m_mapScene[m_eCurrentScene])
	{
		for (auto& iter2 : iter.second)
		{
			lstrcpy(cloneDat.szLayerName, iter.first.c_str());

			lstrcpy(cloneDat.szOriginName, iter2->Get_OriginName().c_str());

			Transform* pTrans = iter2->Get_Transform();

			cloneDat.vScale = pTrans->m_vScale;
			cloneDat.vRotation = pTrans->m_vAngle;
			pTrans->Get_Info(INFO_POS, &cloneDat.vTransform);

			fout.write(reinterpret_cast<char*>(&cloneDat), sizeof(CLONEDAT));
		}
	}

	fout.close();

	MSG_BOX("Save Success");

	return true;
}

_bool MainEditor::Erase_ColBox(ColliderBox* pColBox)
{
	if (!pColBox) return false;

	auto iter = find(m_colBoxList[m_eCurrentScene].begin(), m_colBoxList[m_eCurrentScene].end(), pColBox);

	if (iter != m_colBoxList[m_eCurrentScene].end())
	{
		if (pColBox == m_pPickedColBox)
			m_pPickedColBox = nullptr;

		Safe_Release(*iter);
		m_colBoxList[m_eCurrentScene].erase(iter);
		return true;
	}

	return false;
}

_bool MainEditor::Save_ColBox()
{
	std::ofstream fout;

	switch (m_eCurrentScene)
	{
	case Engine::SCENE_VILLEAGE:
		fout.open(L"../../Resource/ColBoxDat/Villeage.dat", std::ios::binary);
		break;
	case Engine::SCENE_STAGE1:
		fout.open(L"../../Resource/ColBoxDat/Stage1.dat", std::ios::binary);
		break;
	case Engine::SCENE_STAGE2:
		fout.open(L"../../Resource/ColBoxDat/Stage2.dat", std::ios::binary);
		break;
	case Engine::SCENE_STAGE3:
		fout.open(L"../../Resource/ColBoxDat/Stage3.dat", std::ios::binary);
		break;
	case Engine::SCENE_STAGE4:
		fout.open(L"../../Resource/ColBoxDat/Stage4.dat", std::ios::binary);
		break;
	case Engine::SCENE_BOSS:
		fout.open(L"../../Resource/ColBoxDat/Boss.dat", std::ios::binary);
		break;
	}

	if (fout.fail())
	{
		return false;
	}

	int iPrefabCnt = m_colBoxList[m_eCurrentScene].size();

	fout.write(reinterpret_cast<char*>(&iPrefabCnt), sizeof(int)); // 벡터 사이즈
	
	// 수정
	COLBOXDAT ColBoxDat;

	for (auto& iter : m_colBoxList[m_eCurrentScene])
	{
		ColBoxDat.m_iColLayer = COL_WALL;
		ColBoxDat.vScale = iter->Get_Transform()->m_vScale;
		iter->Get_Transform()->Get_Info(INFO_POS, &ColBoxDat.vPos);

		fout.write(reinterpret_cast<char*>(&ColBoxDat), sizeof(COLBOXDAT));
	}

	fout.close();

	MSG_BOX("Save Success");

	return true;

}

HRESULT MainEditor::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	// GraphicDev Setting
	FAILED_CHECK_RETURN(Engine::Init_GraphicDev(g_hWnd, MODE_WIN, 1280, 800, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();
	*ppGraphicDev = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	// Texture Setting
	FAILED_CHECK_RETURN(DataMgr::GetInstance()->Init_DataMgr(), E_FAIL);

	// Prefab Setting
	FAILED_CHECK_RETURN(PrefabMgr::GetInstance()->Init_PrefabMgr(), E_FAIL);


	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);

	(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);		// Z버퍼에 깊이 값을 무조건 기록은 하지만 자동 정렬을 할지 말지 결정
	(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);  // Z버퍼에 깊이 값을 기록할 지 결정

	// Dinput
	FAILED_CHECK_RETURN(Engine::Init_InputDev(g_hInst, g_hWnd), E_FAIL);

	//(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// CamSetting
	_vec3 vEye = { -2.f,2.f,-2.f };
	_vec3 vAt = { 0.f,0.f,0.f };
	_vec3 vUp = { 0.f,1.f,0.f };
	m_pCamera = DynamicCamera_Tool::Create(m_pGraphicDev, &vEye, &vAt, &vUp, D3DXToRadian(60.f), 1280.0f / 800.0f, 0.1f, 1000.f);

	// ImGui Setting
	Init_ImGui();

	// Object Init
	Init_Object();

	return S_OK;
}

_bool MainEditor::Load_GameObject(const wstring& wsDataFilePath, SCENETYPE eSceneType)
{
	std::ifstream fin;

	fin.open(wsDataFilePath, std::ios::binary);

	if (fin.fail())
	{
		fin.close();
		return false;
	}

	int iPrefabCnt;
	fin.read(reinterpret_cast<char*>(&iPrefabCnt), sizeof(int)); // 벡터 사이즈

	auto prefab = PrefabMgr::GetInstance()->m_PrefabList;

	GameObject* pObj = nullptr;
		
	for (int i = 0; i < iPrefabCnt; ++i)
	{
		CLONEDAT Dat;
		fin.read(reinterpret_cast<char*>(&Dat), sizeof(CLONEDAT));
		
		// 데이터로 오브젝트 생성
		const PREFABDAT* Origin = PrefabMgr::GetInstance()->Find_OriginPrefab(Dat.szOriginName, eSceneType);
		
		if (!Origin) return false;
		_vec2 vec;
		switch (Origin->eType)
		{
		case PREFAB_RCTEX:
			vec = { Dat.vScale.x,Dat.vScale.y };
			pObj = CloneRect::Create(m_pGraphicDev, Dat.vTransform, vec, Origin->szTexPath, Dat.szOriginName);
			pObj->Get_Transform()->Set_Scale(Dat.vScale.x, Dat.vScale.y, Dat.vScale.z);
			pObj->Get_Transform()->Set_Rotation(&Dat.vRotation);
			Add_GameObject(eSceneType,Dat.szLayerName, pObj);
			break;
		case PREFAB_CUBE:
			pObj = Cube::Create(m_pGraphicDev, Dat.vTransform, Origin->szTexPath, Dat.szOriginName);
			pObj->Get_Transform()->Set_Scale(Dat.vScale.x, Dat.vScale.y, Dat.vScale.z);
			pObj->Get_Transform()->Set_Rotation(&Dat.vRotation);
			Add_GameObject(eSceneType, Dat.szLayerName, pObj);	
			break;
		case PREFAB_BILLBOARD:
			vec = { Dat.vScale.x,Dat.vScale.y };
			pObj = BillBoardObj::Create(m_pGraphicDev, Dat.vTransform, vec, Origin->szTexPath, Dat.szOriginName);
			pObj->Get_Transform()->Set_Scale(Dat.vScale.x, Dat.vScale.y, Dat.vScale.z);
			pObj->Get_Transform()->Set_Rotation(&Dat.vRotation);
			Add_GameObject(eSceneType, Dat.szLayerName, pObj);
			break;
		}

	}

	fin.close();

	return true;
}

_bool MainEditor::Load_ColliderBox(const wstring& wsDataFilePath, SCENETYPE eSceneType)
{
	std::ifstream fin;

	fin.open(wsDataFilePath, std::ios::binary);

	if (fin.fail())
	{
		fin.close();
		return false;
	}

	int iPrefabCnt;
	fin.read(reinterpret_cast<char*>(&iPrefabCnt), sizeof(int)); // 벡터 사이즈

	ColliderBox* pObj = nullptr;

	for (int i = 0; i < iPrefabCnt; ++i)
	{
		COLBOXDAT Dat;
		fin.read(reinterpret_cast<char*>(&Dat), sizeof(COLBOXDAT));

		pObj = ColliderBox::Create(m_pGraphicDev, Dat.vPos);
		pObj->Get_Transform()->Set_Scale(Dat.vScale.x, Dat.vScale.y, Dat.vScale.z);
		Add_ColliderBox(eSceneType, pObj);
	}

	fin.close();

	return true;
}

void MainEditor::Init_ImGui()
{
	// IMGUI Setting
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// Setup Dear ImGui style  
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphicDev);

	// Create UI
	m_pMainUI = MainUI::Create(this);
}

void MainEditor::Init_Object()
{
	// 게임 오브젝트 로드
	Load_GameObject(L"../../Resource/CloneDat/Villeage.dat", SCENE_VILLEAGE);
	Load_GameObject(L"../../Resource/CloneDat/Stage1.dat", SCENE_STAGE1);
	Load_GameObject(L"../../Resource/CloneDat/Stage2.dat", SCENE_STAGE2);
	Load_GameObject(L"../../Resource/CloneDat/Boss.dat", SCENE_BOSS);

	// 콜라이더 박스 로드
	Load_ColliderBox(L"../../Resource/ColBoxDat/Villeage.dat", SCENE_VILLEAGE);
	Load_ColliderBox(L"../../Resource/ColBoxDat/Stage1.dat", SCENE_STAGE1);
	Load_ColliderBox(L"../../Resource/ColBoxDat/Stage2.dat", SCENE_STAGE2);
	Load_ColliderBox(L"../../Resource/ColBoxDat/Boss.dat", SCENE_BOSS);
}

void MainEditor::Update_ImGui(const float& fTimeDelta)
{
	// Start Imgui Frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// UI Update
	m_pMainUI->Update_UI(fTimeDelta);             // Display some text (you can use a format strings too)
	
	// End Imgui Frame
	ImGui::EndFrame();
}

void MainEditor::Update_Object(const float& fTimeDelta)
{
	for(auto& iter : m_mapScene[m_eCurrentScene])
	{
		for (auto& iter2 : iter.second)
			iter2->Update_GameObject(fTimeDelta);
	}

	for (auto& ColBoxiter : m_colBoxList[m_eCurrentScene])
	{
		ColBoxiter->Update_GameObject(fTimeDelta);
	}
}

void MainEditor::Render_ImGui()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void MainEditor::Render_Object()
{
	if (!m_bGameObjectInvisible)
	{
		for (auto& iter : m_mapScene[m_eCurrentScene])
		{
			for (auto& iter2 : iter.second)
				iter2->Render_GameObject();
		}
	}

	for (auto& ColBoxiter : m_colBoxList[m_eCurrentScene])
	{
		ColBoxiter->Render_GameObject();
	}

}

MainEditor* MainEditor::Create()
{
	MainEditor* pInstance = new MainEditor;

	if (FAILED(pInstance->Init_MainEditor()))
	{
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

void MainEditor::Free()
{
	Safe_Release(m_pCamera);
	Safe_Release(m_pMainUI);

	for (int i = 0; i < SCENE_END; ++i)
	{
		for_each(m_mapScene[i].begin(), m_mapScene[i].end(), [](auto temp){
			for_each(temp.second.begin(), temp.second.end(), [](GameObject* pTemp) {
				if (pTemp != nullptr) Safe_Release(pTemp);
				});
			temp.second.clear();
			});
		m_mapScene[i].clear();
	}

	for (int i = 0; i < SCENE_END; ++i)
	{
		for (auto& iter : m_colBoxList[i])
			Safe_Release(iter);
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);

	DataMgr::GetInstance()->DestroyInstance();
	PrefabMgr::GetInstance()->DestroyInstance();
	PickingMgr::GetInstance()->DestroyInstance();
	Engine::Release_Utility();
	Engine::Release_System();
}
