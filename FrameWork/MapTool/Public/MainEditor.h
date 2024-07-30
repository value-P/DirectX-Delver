#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "Export_System.h"
#include "Export_Utility.h"

class DynamicCamera_Tool;
class MainUI;
class ColliderBox;
class Cube;

class MainEditor :
    public Base
{
private:
    explicit MainEditor();
    virtual ~MainEditor();

public:
    HRESULT Init_MainEditor();
    int     Update_MainEditor(const float& fTimeDelta);
    void    Key_Input();
    void    LateUpdate_MainEditor();
    void    Render_MainEditor();

    void    Mouse_Picking();

public:
    LPDIRECT3DDEVICE9   Get_GrahicDev()                     { return m_pGraphicDev; }
    DynamicCamera_Tool* Get_Camera()                        { return m_pCamera; }
    GameObject*         Get_PicekdObject()                  { return m_pPickedObj; }
    ColliderBox*        Get_PicekdColBox()                  { return m_pPickedColBox; }
    _vec3               Get_PickedPos()                     {return  m_fPickedPos;}
    
    SCENETYPE           Get_CurScene()                      {return m_eCurrentScene;}
    void                Set_CurScene(SCENETYPE eSceneType)  { m_eCurrentScene = eSceneType; }

    void                Add_GameObject(const wstring& layerTag, GameObject* pObject);
    void                Add_GameObject(SCENETYPE eSceneType, const wstring& layerTag, GameObject* pObject);
    
    void                Add_ColliderBox(ColliderBox* pColBox);
    void                Add_ColliderBox(SCENETYPE eSceneType, ColliderBox* pColBox);
    
    _bool               Erase_GameObject(GameObject* pObject);
    _bool               Save_GameObject();

    _bool               Erase_ColBox(ColliderBox* pColBox);
    _bool               Save_ColBox();

private:
    HRESULT SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
    
    _bool   Load_GameObject(const wstring& wsDataFilePath, SCENETYPE eSceneType);
    _bool   Load_ColliderBox(const wstring& wsDataFilePath, SCENETYPE eSceneType);

    void Init_ImGui();
    void Init_Object();

    void Update_ImGui(const float& fTimeDelta);
    void Update_Object(const float& fTimeDelta);

    void Render_ImGui();
    void Render_Object();

public:
    // 레이어의 역할을 하는 map을 담아놓는 일종의 Scene의 역할을 하는 배열
    map<const wstring, list<GameObject*>> m_mapScene[SCENE_END];

    // 씬마다의 맵 충돌박스를 가지고 있을 리스트 배열(기능X)
    list<ColliderBox*> m_colBoxList[SCENE_END];
    // 현재 충돌체 수정 모드인지
    _bool               m_bColliderEditMode = false;
    _bool               m_bGameObjectInvisible = false;

private:
    Engine::GraphicDev* m_pDeviceClass;
    LPDIRECT3DDEVICE9   m_pGraphicDev;

    DynamicCamera_Tool* m_pCamera = nullptr;
    MainUI*             m_pMainUI = nullptr;

    SCENETYPE           m_eCurrentScene = SCENE_LOGO;

    GameObject*         m_pPickedObj = nullptr;
    ColliderBox*        m_pPickedColBox = nullptr;
  
    _vec3              m_fPickedPos;

public:
    static MainEditor* Create();

private:
    virtual void Free();
};

