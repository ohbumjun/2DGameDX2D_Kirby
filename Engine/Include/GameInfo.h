#pragma once

#define	DIRECTINPUT_VERSION	0x0800

#include <algorithm>
#include <cctype>
// Memory Leak
#include <crtdbg.h>
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <list>
#include <math.h>
#include <io.h>
#include <sstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include <stack>
#include <process.h>
#include <Windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <dwrite.h>
#include <dwrite_3.h>
#include <d2d1.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "SharedPtr.h"
#include "Matrix.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Resource/Texture/DirectXTex.h"
#include "fmod.hpp"

#ifdef _DEBUG

#pragma comment(lib, "DirectXTex_Debug.lib")

#else

#pragma comment(lib, "DirectXTex.lib")

#endif // _DEBUG

#pragma comment(lib, "fmod64_vc.lib")

// Path 
#define ENGINE_ROOT_PATH "EngineRoot"
#define	ENGINE_SHADER_PATH			"EngineShader"
#define	ENGINE_TEXTURE_PATH			"EngineTexture"
#define	ENGINE_FONT_PATH				"EngineFont"
#define	ENGINE_ANIMATION_PATH		"EngineAnimation"
#define	ENGINE_SCENE_PATH				"EngineScene"
#define	ENGINE_SOUND_PATH			"EngineSound"
#define	ENGINE_SEQUENCE_PATH			"EngineSequence"
#define	ENGINE_OBJECT_PATH			"EngineObject"

#define	ROOT_PATH				"Root"
#define	SHADER_PATH			"Shader"
#define	TEXTURE_PATH			"Texture"
#define	FONT_PATH				"Font"
#define	ANIMATION_PATH		"Animation"
#define	SCENE_PATH				"Scene"
#define	SOUND_PATH				"Sound"
#define	SEQUENCE_PATH		"Sequence"
#define	OBJECT_PATH		"Object"

// Excel Path
#define EXCEL_PATH "C:\\Users\\dhsys\\Desktop\\DX_Project\\CharacterStats.xlsx"

// Object Name
#define OBJECTNAME_SHOWOBJECT "SceneEditObject"

// Candy Crush??
#define CANDY_ANIMATION_PATH "CandyAnimation"
#define CANDY_SEQUENCE_PATH "CandySequence"

// Delete 
#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr; }

// SingleTon
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

// Definition
#define GRAVITY 9.8f

// Struct 
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

struct TileCBuffer
{
	Vector2 ImageSize;
	Vector2 Size;
	// UV
	Vector2 Start;
	Vector2 End;
	Matrix matWVP;
};

struct TileEmptyInfo {
	Matrix matWVP;
	Vector4 TileColor;
};

struct TileInfo
{
	Matrix matWVP;
	Vector2 TileStart;
	Vector2 TileEnd;
	Vector4 TileColor;
	float Opacity;
	Vector3 Empty;
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
	int      PaperBurnEnable;
	Vector2 Empty;
};

struct PaperBurnCBuffer {
	Vector4 InLineColor;
	Vector4 OutLineColor;
	Vector4 CenterLineColor;
	float Filter;
	int Inverse;
	float InFilter;
	float OutFilter;
	float CenterFilter;
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

// ?浹?? ?????? ??ȯ?ϱ? ???? ????ü 
struct CollisionResult
{
	class CColliderComponent* Src;
	class CColliderComponent* Dest;
	Vector3 HitPoint; // ?ε??? ??ġ 
};

struct ColliderCBuffer
{
	Vector4 Color;
	Matrix matWVP;
};

struct WidgetCBuffer {
	Vector4 Tint;
	Matrix matWP;
	int UseTexture;
	int WidgetAnimType;
	Vector2 Animation2DStartUV;
	Vector2 Animation2DEndUV;
	int WidgetAnimEnable;
	float Opacity;
	WidgetCBuffer() :
		Animation2DStartUV(0.f, 0.f),
		Animation2DEndUV(1.f, 1.f),
		UseTexture(0),
		WidgetAnimEnable(0),
		Opacity(1.f)
	{
	}
};

struct ProgressBarCBuffer {
	float Percent;
	int Dir;
	Vector2 Empty;
};


struct CircleInfo {
	Vector2 Center;
	float Radius;
	Vector2 Min;
	Vector2 Max;
};

struct PixelInfo {
	unsigned char* Pixel; // ?????ؾ??? pixel ???? ??? ?? ?? ?𸣰?, ?̹??? ???? ???? ũ?⵵ ?????? ?̷??? ???? 
	unsigned int Width;
	unsigned int Height;
	PixelCollision_Type Type;
	unsigned char Color[4];// pixel?? ?ϳ??? ?̹??? ???? --> 0???? 255 ?????? r,g,b,a ?? ?????Ǿ? ?ִ?.
	Box2DInfo Box; // Pixel ?浹 ?Ǻ??? ???? Box ???? ?浹 ???θ? ?ľ??ϱ? ????
	Vector2 Min;
	Vector2 Max;
	ID3D11ShaderResourceView* SRV;
	int RefCount;

	PixelInfo() :
		RefCount(1),
		Pixel(nullptr),
		SRV(nullptr),
		Width(0),
		Height(0),
		Box{},
		Color{},
		Type(PixelCollision_Type::Color_Confirm)
	{}
};


struct	ParticleCBuffer
{
	unsigned int	SpawnEnable;	// ???? ??ƼŬ ???? ????
	Vector3	StartMin;		// ??ƼŬ?? ?????? ?????? Min
	Vector3	StartMax;		// ??ƼŬ?? ?????? ?????? Max
	unsigned int	SpawnCountMax;	// ?????? ??ƼŬ?? ?ִ?
	Vector3	ScaleMin;		// ?????? ??ƼŬ ũ???? Min
	float	LifeTimeMin;	// ?????? ??ƼŬ?? ???????? ?ּҽð?
	Vector3	ScaleMax;		// ?????? ??ƼŬ ũ???? Max
	float	LifeTimeMax;	// ?????? ??ƼŬ?? ???????? ?ִ??ð?
	Vector4	ColorMin;		// ?????? ??ƼŬ?? ???? Min
	Vector4	ColorMax;		// ?????? ??ƼŬ?? ???? Max
	float	SpeedMin;		// ??ƼŬ?? ?ּ? ?̵??ӵ?
	float	SpeedMax;		// ??ƼŬ?? ?ִ? ?̵??ӵ?
	int		Move;			// ?̵??? ?ϴ??? ???ϴ???
	int		Gravity;		// ?߷? ?????? ?޴??? ?ȹ޴???
	Vector3	MoveDir;		// ?̵??? ?Ѵٸ? ?????? ?? ?̵? ????
	int		Is2D;			// 2D?? ??ƼŬ????
	Vector3	MoveAngle;	// ?̵??? ?Ѵٸ? ?????? ?? ???????κ??? x, y, z ?? ?????? ??????ŭ Ʋ???? ?????? ?????? ???Ѵ?.
	float	Empty;
};

struct GlobalCBuffer
{
	float g_DeltaTime;
	float g_AccTime;
	Vector2 g_Resolution;
	Vector2 g_NoiseResolution;
	Vector2 g_GlobalEmpty;
};

struct ParticleInfo
{
	Vector3 WorldPos;
	Vector3 Dir;
	float Speed;
	float LifeTime;
	float LifeTimeMax;
	int Alive;
	float FallTime;
	float FallStartY;
};

struct ParticleInfoShared
{
	int SpawnEnable;
	Vector3 ScaleMin;
	Vector3 ScaleMax;
	Vector4 ColorMin;
	Vector4 ColorMax;
	int GravityEnable;
};

struct NavWorkData {
	std::function<void(const std::list<Vector3>&)> Callback;
	CSharedPtr<class CSceneComponent> OwnerComponent;
	// Vector3 Start;
	Vector3 End;
};

struct NavResultData {
	// ???? ã?ƾ? ?ϴ? ??ġ?? vector ???????? ???? ???·? ???޹??? ???̴?.
	std::function<void(const std::list<Vector3>&)> Callback;
	// ???Ŀ? ???θ? ?????? ???̴?.
	std::list<Vector3> vecPath;
};
