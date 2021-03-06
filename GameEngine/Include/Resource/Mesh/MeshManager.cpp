#include "MeshManager.h"
#include "SpriteMesh.h"
#include "StaticMesh.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

bool CMeshManager::Init()
{
	CMesh* SpriteMesh = new CSpriteMesh;

	if (!SpriteMesh->Init())
	{
		SpriteMesh->Release();
		return false;
	}
	SpriteMesh->SetName("SpriteMesh");

	CMesh* FrameRectMesh = new CStaticMesh;

	Vector3 FrameRectPos[5] =
	{
		Vector3(0.f, 1.f, 0.f),
		Vector3(1.f, 1.f, 0.f),
		Vector3(1.f, 0.f, 0.f),
		Vector3(0.f, 0.f, 0.f),
		Vector3(0.f, 1.f, 0.f)
	};

	// 총 4개이 선을 그려 하나의 사각형을 만들 것이다.
	FrameRectMesh->SetName("FrameRect");
	FrameRectMesh->CreateMesh(FrameRectPos, sizeof(Vector3), 5,
	                          D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// Box2D Mesh
	CMesh* Box2DMesh = new CStaticMesh;

	// Center가 0.0 이 되도록 한다
	// 우리는 ColliderBox2D의 경우, Center를 중심으로 움직이기 때문이다. 
	Vector3 Box2DPos[5] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
		Vector3(-0.5f, 0.5f, 0.f)
	};

	Box2DMesh->SetName("Box2D");
	Box2DMesh->CreateMesh(Box2DPos, sizeof(Vector3), 5,
		D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// Circle
	CMesh* CircleMesh = new CStaticMesh;
	Vector3 CirclePos[31] = {};
	CirclePos[0] = Vector3(0.5f, 0.f, 0.f); // 반지름 0.5f

	for (int i = 1; i < 31; i++)
	{
		CirclePos[i].x = cosf(DegreeToRadian(12.f * i)) * 0.5f;
		CirclePos[i].y = sinf(DegreeToRadian(12.f * i)) * 0.5f;
	}

	CircleMesh->SetName("Circle");
	CircleMesh->CreateMesh(CirclePos, sizeof(Vector3), 31, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);


	// Line
	CMesh* LineMesh = new CStaticMesh;
	Vector3 LinePos[2] =
	{
		Vector3(0.f, 1.f, 0.f),
		Vector3(1.f, 0.f, 0.f)
	};

	LineMesh->SetName("Line");
	LineMesh->CreateMesh(LinePos, sizeof(Vector3), 2,
		D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);



	CMesh* WidgetMesh = new CStaticMesh;

	// Pos의 경우, y는 위가 1, 아래가 0
	// UV의 경우, y는 위가 0, 아래가 1
	VertexUV	WidgetVtx[4] =
	{
		VertexUV(Vector3(0.f, 1.f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(1.f, 1.f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(0.f, 0.f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(1.f, 0.f, 0.f), Vector2(1.f, 1.f))
	};

	int	WidgetIdx[6] = { 0, 1, 3, 0, 3, 2 };

	WidgetMesh->SetName("WidgetMesh");
	WidgetMesh->CreateMesh(WidgetVtx, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		WidgetIdx, 4, 6, D3D11_USAGE_IMMUTABLE,
		DXGI_FORMAT_R32_UINT);

	CMesh* ParticlePointMesh = new CStaticMesh;
	Vector3 Point;

	ParticlePointMesh->SetName("ParticlePointMesh");
	ParticlePointMesh->CreateMesh(&Point, sizeof(Vector3), 1, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// Sprite Mesh
	m_mapMesh.insert(std::make_pair("SpriteMesh", SpriteMesh));

	// FrameRect Mesh
	m_mapMesh.insert(std::make_pair("FrameRect", FrameRectMesh));

	// LineMesh
	m_mapMesh.insert(std::make_pair("Line", LineMesh));

	// Box2DMesh
	m_mapMesh.insert(std::make_pair("Box2D", Box2DMesh));

	// CircleMesh
	m_mapMesh.insert(std::make_pair("Circle", CircleMesh));

	// Widget Mesh
	m_mapMesh.insert(std::make_pair("WidgetMesh", WidgetMesh));

	// Particle Point Mesh
	m_mapMesh.insert(std::make_pair("ParticlePointMesh", ParticlePointMesh));

	return true;
}

CMesh* CMeshManager::FindMesh(const std::string& Name)
{
	auto iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
		return nullptr;

	return iter->second;
}

void CMeshManager::ReleaseMesh(const std::string& Name)
{
	auto iter = m_mapMesh.find(Name);

	if (iter != m_mapMesh.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapMesh.erase(iter);
	}
}
