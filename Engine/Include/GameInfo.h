#pragma once

#define	DIRECTINPUT_VERSION	0x0800

#include <algorithm>
#include <cctype>
// Memory Leak
#include <crtdbg.h>
#include <stdlib.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <functional>
#include <iostream>
#include <list>
#include <math.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <Windows.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Matrix.h"
#include "SharedPtr.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#define	ROOT_PATH		   "Root"
#define	SHADER_PATH      "Shader"
#define	TEXTURE_PATH	   "Texture"
#define	FONT_PATH		   "Font"
#define	ANIMATION_PATH "Animation"
#define	SCENE_PATH        "SceneB"

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr; }

#define	DECLARE_SINGLE(Type)	\
private:\
	static Type*	m_Inst;\
public:\
	static Type* GetInst()\
	{\
		if(!m_Inst)\
			m_Inst = new Type;\
		return m_Inst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(m_Inst);\
	}\
private:\
	Type();\
	~Type();

#define	DEFINITION_SINGLE(Type)	Type* Type::m_Inst = nullptr;

struct Resolution
{
	unsigned int Width;
	unsigned int Height;
};

struct VertexColor
{
	Vector3 Pos;
	Vector4 Color;

	VertexColor()
	{
	}

	VertexColor(const Vector3& _Pos, const Vector4& _Color) :
		Pos(_Pos),
		Color(_Color)
	{
	}
};


struct VertexBuffer
{
	ID3D11Buffer* Buffer;
	int           Size;
	int           Count;

	VertexBuffer() :
		Buffer(nullptr),
		Size(0),
		Count(0)
	{
	}

	~VertexBuffer()
	{
		SAFE_RELEASE(Buffer);
	}
};

struct IndexBuffer
{
	ID3D11Buffer* Buffer;
	int           Size;
	int           Count;
	DXGI_FORMAT   Fmt;

	IndexBuffer() :
		Buffer(nullptr),
		Size(0),
		Count(0),
		Fmt(DXGI_FORMAT_UNKNOWN)
	{
	}

	~IndexBuffer()
	{
		SAFE_RELEASE(Buffer);
	}
};


struct MeshContainer
{
	VertexBuffer             VB;
	std::vector<IndexBuffer> vecIB;
	D3D11_PRIMITIVE_TOPOLOGY Primitive;
};

struct TransformCBuffer
{
	Matrix  matWorld;
	Matrix  matView;
	Matrix  matProj;
	Matrix  matWV;
	Matrix  matWVP;
	Matrix  matVP;
	Vector3 Pivot;
	float   Empty1;
	Vector3 MeshSize;
	float   Empty;
};

struct VertexUV
{
	Vector3 Pos;
	Vector2 UV;

	VertexUV()
	{
	}

	VertexUV(const Vector3& _Pos, const Vector2& _UV) :
		Pos(_Pos),
		UV(_UV)
	{
	}
};

struct MaterialCBuffer
{
	Vector4 BaseColor;
	float   Opacity;
	Vector3 Empty;
};

struct FindComponentName
{
	std::string Name;
	std::string ParentName;
};

struct AnimationFrameData
{
	Vector2 Start;
	Vector2 Size;
};

struct Animation2DCBuffer
{
	Vector2 Animation2DStartUV;
	Vector2 Animation2DEndUV;
	int     Animation2DType;
	Vector3 Animation2DEmpty;
};

struct Standard2DCBuffer
{
	int     AnimationEnable;
	Vector3 Empty;
};

// Functions
static bool CheckIfStringIsDigit(const std::string& String)
{
	for (int i = 0; i < String.length(); i++)
		if (!isdigit(String[i]))
			return false;
	return true;
}

static bool StringToBool(std::string String)
{
	bool result;
	std::transform(String.begin(), String.end(), String.begin(), tolower);
	std::istringstream is(String);
	is >> std::boolalpha >> result;
	return result;
}

struct Collision_Profile
{
	std::string Name;
	Collision_Channel Channel;
	bool CollisionEnable;
	std::vector<Collision_Interaction> vecInteraction;
};

struct Box2DInfo
{
	Vector2 Center;
	Vector2 Axis[2];
	Vector2 Length;
	Vector2 Min;
	Vector2 Max;
};

// 충돌한 결과를 반환하기 위한 구조체 
struct CollisionResult
{
	class CColliderComponent* Src;
	class CColliderComponent* Dest;
	Vector3 HitPoint; // 부딪힌 위치 
};

struct ColliderCBuffer
{
	Vector4 Color;
	Matrix matWVP;
};