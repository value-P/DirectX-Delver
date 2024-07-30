#pragma once

#include "Management.h"

#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "ColliderFrame.h"
#include "PickCube.h"

#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "AnimationTexture.h"

#include "Camera.h"
#include "UIBase.h"

#include "ProtoMgr.h"
#include "Pipeline.h"

#include "Renderer.h"
#include "UIManager.h"
#include "CollisionMgr.h"
#include "RayPickingMgr.h"
#include "PSMgr.h"

#include "PSystem.h"
#include "PTrace.h"
#include "PSpread.h"
#include "PCharging.h"
#include "PLaserCharging.h"

#include "SingleCamera.h"

BEGIN(Engine)

//management

inline HRESULT		Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, Management** ppInstance);

inline Component*			Get_Component(const wstring pLayerTag, const wstring pObjTag, const wstring pComponentTag, COMPONENTID eID);
inline Layer*				Get_Layer(const wstring pLayerTag);
inline vector<GameObject*>* Get_GameObject(const wstring pLayerTag, const wstring pObjTag);

inline Scene*			Get_CurScene();
inline HRESULT			Set_Scene(Scene* pScene);
inline void				SetLoadingImg(GameObject* _pLoadingImg);

inline _int			Update_Scene(const _float& fTimeDelta, HWND hWnd);
inline void			LateUpdate_Scene();
inline void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

bool			RayCast(Ray* ray, GameObject** pPickedObj, _vec3* pPickedPos, _float maxDist = 100.f, const wstring& layerTag = LAYER_MAPCOL);

// ProtoMgr
inline HRESULT			Init_Proto(const wstring pProtoTag, Component* pComponent);
inline Component*		Clone_Proto(const wstring pProtoTag);

// Renderer
inline void		Add_RenderGroup(RENDERID eType, GameObject* pGameObject);
inline void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void		Clear_RenderGroup();

// UIMgr

inline HRESULT Init_UI(const wstring& _wstrUITag, UIBase* _pUIBase);
inline void Activate_UI(UIBase* _pUI);
inline void Return_UI();
inline void Clear_UI();
inline UIBase* GetParentUI();
inline UIBase* GetParentUI(const wstring& wstrKey);
inline UIBase* GetCurUI();
inline vector<UIBase*>& GetCurUIVec();

// CollisionMgr
inline void AddCollider(Collider* _pCollider);
inline void ComputeCollision();
inline void ClearColMgr();

// RayPickingMgr
inline Ray* RayAtViewSpace(LPDIRECT3DDEVICE9 pGraphicDev, int iScreenX, int iScreenY, _matrix* matProj);
inline Ray* RayAtWorldSpace(LPDIRECT3DDEVICE9 pGraphicDev, int iScreenX, int iScreenY, _matrix* matProj);
inline Ray* RayAtLocalSpace(LPDIRECT3DDEVICE9 pGraphicDev, int iScreenX, int iScreenY, _matrix* matProj, _matrix* matWorld);

inline _bool RayPicking(_vec3& v0, _vec3& v1, _vec3& v2, _vec3 vPickedPosition);

// ParticleMgr
inline void UpdatePSMgr(const float& _fTimeDelta);
inline void LateUpdatePSMgr();
inline void RenderPSMgr(LPDIRECT3DDEVICE9 _pGraphicDevice);
inline void AddParticles(PSystem* _pParticles);
inline void PSystem_Clear();

inline void	Release_Utility();


#include "Export_Utility.inl"

END