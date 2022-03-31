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

// Candy Crush용
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
	unsigned char* Pixel; // 저장해야할 pixel 수가 몇개가 될 지 모르고, 이미지 가로 세로 크기도 몰라서 이렇게 세팅 
	unsigned int Width;
	unsigned int Height;
	PixelCollision_Type Type;
	unsigned char Color[4];// pixel은 하나의 이미지 형태 --> 0에서 255 사이의 r,g,b,a 로 구성되어 있다.
	Box2DInfo Box; // Pixel 충돌 판별시 먼저 Box 와의 충돌 여부를 파악하기 위함
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
	unsigned int	SpawnEnable;	// 현재 파티클 생성 여부
	Vector3	StartMin;		// 파티클이 생성될 영역의 Min
	Vector3	StartMax;		// 파티클이 생성될 영역의 Max
	unsigned int	SpawnCountMax;	// 생성될 파티클의 최대
	Vector3	ScaleMin;		// 생성될 파티클 크기의 Min
	float	LifeTimeMin;	// 생성될 파티클이 살아있을 최소시간
	Vector3	ScaleMax;		// 새성될 파티클 크기의 Max
	float	LifeTimeMax;	// 생성될 파티클이 살아있을 최대시간
	Vector4	ColorMin;		// 생성될 파티클의 색상 Min
	Vector4	ColorMax;		// 생성될 파티클의 색상 Max
	float	SpeedMin;		// 파티클의 최소 이동속도
	float	SpeedMax;		// 파티클의 최대 이동속도
	int		Move;			// 이동을 하는지 안하는지
	int		Gravity;		// 중력 적용을 받는지 안받는지
	Vector3	MoveDir;		// 이동을 한다면 기준이 될 이동 방향
	int		Is2D;			// 2D용 파티클인지
	Vector3	MoveAngle;	// 이동을 한다면 기준이 될 방향으로부터 x, y, z 에 저장된 각도만큼 틀어진 랜덤한 방향을 구한다.
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
	// 길이 찾아야 하는 위치를 vector 목록으로 인자 형태로 전달받을 것이다.
	std::function<void(const std::list<Vector3>&)> Callback;
	// 거쳐온 경로를 저장할 것이다.
	std::list<Vector3> vecPath;
};