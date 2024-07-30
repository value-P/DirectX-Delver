#include "Export_Utility.h"

HRESULT		Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, Management** ppInstance)
{
	Management*		pManagement = Management::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppInstance = pManagement;

	return S_OK;
}
Component*			Get_Component(const wstring pLayerTag, const wstring pObjTag, const wstring pComponentTag, COMPONENTID eID)
{
	return Management::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

Layer* Get_Layer(const wstring pLayerTag)
{
	return Management::GetInstance()->Get_Layer(pLayerTag);
}

vector<GameObject*>* Get_GameObject(const wstring pLayerTag, const wstring pObjTag)
{
	return Management::GetInstance()->Get_GameObject(pLayerTag, pObjTag);
}

Scene*				Get_CurScene()
{
	return Management::GetInstance()->Get_CurScene();
}

HRESULT			Set_Scene(Scene* pScene)
{
	return Management::GetInstance()->Set_Scene(pScene);
}

void SetLoadingImg(GameObject* _pLoadingImg)
{
	if (_pLoadingImg)
	{
		Management::GetInstance()->SetLoadingImg(_pLoadingImg);
	}
}

_int			Update_Scene(const _float& fTimeDelta, HWND hWnd)
{
	return Management::GetInstance()->Update_Scene(fTimeDelta, hWnd);
}
void			LateUpdate_Scene()
{
	Management::GetInstance()->LateUpdate_Scene();		
}
void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Management::GetInstance()->Render_Scene(pGraphicDev);
}

inline bool RayCast(Ray* ray, GameObject** pPickedObj, _vec3* pPickedPos, _float maxDist, const wstring& layerTag)
{
	return Management::GetInstance()->RayCast(ray,pPickedObj,pPickedPos,maxDist,layerTag);
}

HRESULT			Init_Proto(const wstring pProtoTag, Component* pComponent)
{
	return ProtoMgr::GetInstance()->Init_Proto(pProtoTag, pComponent);
}
Component*		Clone_Proto(const wstring pProtoTag)
{
	return ProtoMgr::GetInstance()->Clone_Proto(pProtoTag);
}

void		Add_RenderGroup(RENDERID eType, GameObject* pGameObject)
{
	Renderer::GetInstance()->Add_RenderGroup(eType, pGameObject);
}
void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Renderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void		Clear_RenderGroup()
{
	Renderer::GetInstance()->Clear_RenderGroup();
}

inline HRESULT Init_UI(const wstring& _wstrUITag, UIBase* _pUIBase)
{
	return UIManager::GetInstance()->Init_UI(_wstrUITag, _pUIBase);
}

inline void Activate_UI(UIBase* _pUI)
{
	UIManager::GetInstance()->Activate_UI(_pUI);
}

inline void Return_UI()
{
	UIManager::GetInstance()->Return_UI();
}

inline void Clear_UI()
{
	UIManager::GetInstance()->Clear_UI();
}

inline UIBase* GetParentUI()
{
	return UIManager::GetInstance()->GetParentUI();
}

inline UIBase* GetParentUI(const wstring& wstrKey)
{
	return UIManager::GetInstance()->GetParentUI(wstrKey);
}

inline UIBase* GetCurUI()
{
	return UIManager::GetInstance()->GetCurUI();
}

inline vector<UIBase*>& GetCurUIVec()
{
	return UIManager::GetInstance()->GetCurUIVec();
}

inline void AddCollider(Collider* _pCollider)
{
	CollisionMgr::GetInstance()->AddCollider(_pCollider);
}

inline void ComputeCollision()
{
	CollisionMgr::GetInstance()->ComputeCollision();
}

inline void ClearColMgr()
{
	CollisionMgr::GetInstance()->ClearColMgr();
}

inline Ray* RayAtViewSpace(LPDIRECT3DDEVICE9 pGraphicDev, int iScreenX, int iScreenY, _matrix* matProj)
{
	return RayPickingMgr::GetInstance()->RayAtViewSpace(pGraphicDev, iScreenX, iScreenY, matProj);
}

inline Ray* RayAtWorldSpace(LPDIRECT3DDEVICE9 pGraphicDev, int iScreenX, int iScreenY, _matrix* matProj)
{
	return RayPickingMgr::GetInstance()->RayAtWorldSpace(pGraphicDev, iScreenX, iScreenY, matProj);
}

inline Ray* RayAtLocalSpace(LPDIRECT3DDEVICE9 pGraphicDev, int iScreenX, int iScreenY, _matrix* matProj, _matrix* matWorld)
{
	return RayPickingMgr::GetInstance()->RayAtLocalSpace(pGraphicDev, iScreenX, iScreenY, matProj, matWorld);
}

inline _bool RayPicking(_vec3& v0, _vec3& v1, _vec3& v2, _vec3 vPickedPosition)
{
	return RayPickingMgr::GetInstance()->RayPicking(v0, v1, v2, vPickedPosition);
}

inline void UpdatePSMgr(const float& _fTimeDelta)
{
	PSMgr::GetInstance()->UpdatePSMgr(_fTimeDelta);
}

inline void LateUpdatePSMgr()
{
	PSMgr::GetInstance()->LateUpdatePSMgr();
}

inline void RenderPSMgr(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	PSMgr::GetInstance()->RenderPSMgr(_pGraphicDevice);
}

inline void AddParticles(PSystem* _pParticles)
{
	PSMgr::GetInstance()->AddParticles(_pParticles);
}

inline void PSystem_Clear()
{
	PSMgr::GetInstance()->PSystem_Clear();
}

void			Release_Utility()
{
	SingleCamera::GetInstance()->DestroyInstance();
	PSMgr::GetInstance()->DestroyInstance();
	RayPickingMgr::GetInstance()->DestroyInstance();
	CollisionMgr::GetInstance()->DestroyInstance();
	UIManager::GetInstance()->DestroyInstance();
	Renderer::GetInstance()->DestroyInstance();
	ProtoMgr::GetInstance()->DestroyInstance();
	Management::GetInstance()->DestroyInstance();
}