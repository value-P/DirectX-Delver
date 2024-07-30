#pragma once
#include "Engine_Define.h"

namespace Engine
{
	class Texture;
}

struct TEXDAT
{
	std::string texName;				// 텍스처 대표이름
	std::wstring texPath;				// 샘플 텍스처 경로
	Engine::Texture* pTex = nullptr;	// 샘플 텍스처(보여주기용)
};

struct COLLIDERDAT
{
	Engine::_bool bHasCollider;
	Engine::COLLAYER eColLayer;
	Engine::COMPONENTID eComponentID;
};

