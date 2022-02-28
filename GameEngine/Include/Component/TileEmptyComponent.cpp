#include "TileEmptyComponent.h"
#include "TileEmpty.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/SceneManager.h"
#include "CameraComponent.h"
#include "../Scene/CameraManager.h"
#include "../Scene/NavigationManager.h"
#include "../Scene/SceneCollision.h"

class CStaticMesh;

CTileEmptyComponent::CTileEmptyComponent()
{
	SetTypeID<CTileEmptyComponent>();
	m_Render = true;

	m_CountX = 0;
	m_CountY = 0;
	m_RenderCount = 0;
	m_LayerName = "Back";

	m_DeltaTime = 0.f;

	for (int i = 0; i < (int)Tile_Type::End; ++i)
	{
		m_TileEmptyColor[i] = Vector4(1.f, 1.f, 1.f, 1.f);
	}

	m_TileEmptyColor[(int)Tile_Type::Wall] = Vector4(1.f, 0.f, 0.f, 1.f);

	m_EditMode = false;
}

CTileEmptyComponent::CTileEmptyComponent(const CTileEmptyComponent& Component)
{
	m_BackMesh = Component.m_BackMesh;

	if (Component.m_BackMaterial)
		m_BackMaterial = Component.m_BackMaterial->Clone();

	if (Component.m_TileInfoBuffer)
		m_TileInfoBuffer = Component.m_TileInfoBuffer->Clone();

	for (int i = 0; i < (int)Tile_Type::End; i++)
	{
		m_TileEmptyColor[i] = Component.m_TileEmptyColor[i];
	}

	m_EditMode = Component.m_EditMode;
}

CTileEmptyComponent::~CTileEmptyComponent()
{
	SAFE_DELETE(m_TileInfoBuffer);

	size_t Size = m_vecTileEmpty.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecTileEmpty[i]);
	}

	m_vecTileEmpty.clear();
}

void CTileEmptyComponent::SetBackMaterial(CMaterial* Material)
{
	m_BackMaterial = Material->Clone();

	m_BackMaterial->SetScene(m_Scene);

	if (!m_BackMaterial->EmptyTexture())
	{
		SetWorldScale((float)m_BackMaterial->GetTextureWidth(),
			(float)m_BackMaterial->GetTextureHeight(), 1.f);
	}
}

void CTileEmptyComponent::SetBackBaseColor(const Vector4& Color)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetBaseColor(Color);
}

void CTileEmptyComponent::SetBackBaseColor(float r, float g, float b, float a)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetBaseColor(r, g, b, a);
}

void CTileEmptyComponent::SetBackRenderState(CRenderState* State)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetRenderState(State);
}

void CTileEmptyComponent::SetBackRenderState(const std::string& Name)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetRenderState(Name);
}

void CTileEmptyComponent::SetBackTransparency(bool Enable)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTransparency(Enable);
}

void CTileEmptyComponent::SetBackOpacity(float Opacity)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetOpacity(Opacity);
}

void CTileEmptyComponent::AddBackOpacity(float Opacity)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddOpacity(Opacity);
}

void CTileEmptyComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddTexture(Register, ShaderType, Name, Texture);
}

void CTileEmptyComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddTexture(Register, ShaderType, Name, FileName, PathName);
}

void CTileEmptyComponent::AddBackTextureFullPath(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddTextureFullPath(Register, ShaderType, Name, FullPath);
}

void CTileEmptyComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddTexture(Register, ShaderType, Name, vecFileName, PathName);
}

void CTileEmptyComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	CTexture* Texture)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, Texture);
}

void CTileEmptyComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, FileName, PathName);
}

void CTileEmptyComponent::SetBackTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTextureFullPath(Index, Register, ShaderType, Name, FullPath);
}

void CTileEmptyComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, vecFileName, PathName);
}

void CTileEmptyComponent::CreateTileEmpty(int CountX, int CountY, const Vector3& Size)
{
	// 기존 Tile 들을 지워준다.
	for (int i = 0; i < m_Count; i++)
	{
		SAFE_DELETE(m_vecTileEmpty[i]);
	}

	m_vecTileEmpty.clear();

	m_CountX = CountX;
	m_CountY = CountY;
	m_TileEmptySize = Size;
	m_Count = m_CountX * m_CountY;

	m_vecTileEmpty.resize(m_Count);

	for (int row = 0; row < m_CountY; row++)
	{
		for (int col = 0; col < m_CountX; col++)
		{
			CTileEmpty* Tile = new CTileEmpty;

			Tile->m_Owner = this;

			int Index = row * m_CountX + col;

			Tile->SetIndex(col, row, Index);

			Tile->SetSize(m_TileEmptySize);

			m_vecTileEmpty[Index] = Tile;
		}
	}

	// Tile 위치 세팅
	Vector3 Pos = {};

	for (int row = 0; row < m_CountY; row++)
	{
		Pos.x = 0.f;
		Pos.y = row * m_TileEmptySize.y;

		for (int col = 0; col < m_CountX; col++)
		{
			Pos.x = col * m_TileEmptySize.x;

			int Index = row * m_CountY + col;

			m_vecTileEmpty[Index]->SetPos(Pos);
		}
	}

	// 그외 정보 세팅
	SetWorldInfo();

}

void CTileEmptyComponent::SetTileEmptyColor(Tile_Type Type, float r, float g, float b, float a)
{
	m_TileEmptyColor[(int)Type] = Vector4(r, g, b, a);
}

void CTileEmptyComponent::SetTileEmptyColor(Tile_Type Type, const Vector4& Color)
{
	m_TileEmptyColor[(int)Type] = Color;
}

int CTileEmptyComponent::GetTileEmptyIndexX(const Vector3& Pos)
{
	float	ConvertX = Pos.x - GetWorldPos().x;

	int	IndexX = (int)(ConvertX / m_TileEmptySize.x);

	if (IndexX < 0 || IndexX >= m_CountX)
		return -1;

	return IndexX;
}

int CTileEmptyComponent::GetTileEmptyIndexY(const Vector3& Pos)
{
	float ConvertY = Pos.y - GetWorldPos().y;

	int IndexY = (int)(ConvertY / m_TileEmptySize.y);

	if (IndexY < 0 || IndexY >= m_CountX)
		return -1;

	return IndexY;
}

int CTileEmptyComponent::GetTileEmptyIndex(const Vector3& Pos)
{
	int	IndexX = GetTileEmptyIndexX(Pos);
	int	IndexY = GetTileEmptyIndexY(Pos);

	if (IndexX == -1 || IndexY == -1)
		return -1;

	return IndexY * m_CountX + IndexX;
}

CTileEmpty* CTileEmptyComponent::GetTileEmpty(const Vector3& Pos)
{
	int Index = GetTileEmptyIndex(Pos);

	if (Index == -1)
		return nullptr;

	return m_vecTileEmpty[Index];
}

CTileEmpty* CTileEmptyComponent::GetTileEmpty(int x, int y)
{
	if (x < 0 || x >= m_CountX || y < 0 || y >= m_CountY)
		return nullptr;

	return m_vecTileEmpty[y * m_CountX + x];
}

CTileEmpty* CTileEmptyComponent::GetTileEmpty(int Index)
{
	if (Index < 0 || Index >= m_CountX * m_CountY)
		return nullptr;

	return m_vecTileEmpty[Index];
}

int CTileEmptyComponent::GetTileEmptyRenderIndexX(const Vector3& Pos)
{
	float ConvertX = Pos.x - GetWorldPos().x;

	int IndexX = (int)(ConvertX / m_TileEmptySize.x);

	if (IndexX < 0)
		return 0;
	if (IndexX >= m_CountX)
		return m_CountX - 1;

	return IndexX;
}

int CTileEmptyComponent::GetTileEmptyRenderIndexY(const Vector3& Pos)
{
	float ConvertY = Pos.y - GetWorldPos().y;

	int IndexY = (int)(ConvertY / m_TileEmptySize.y);

	if (IndexY < 0)
		return 0;

	if (IndexY >= m_CountX)
		return m_CountX - 1;

	return IndexY;
}

void CTileEmptyComponent::Start()
{
	CSceneComponent::Start();
}

bool CTileEmptyComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	// Back Mesh
	m_BackMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh("SpriteMesh");

	// Back Material
	m_Scene->GetResource()->CreateMaterial<CMaterial>("BackMaterial");

	m_BackMaterial = m_Scene->GetResource()->FindMaterial("BackMaterial");

	m_BackMaterial->SetShader("Mesh2DShader");

	// Tile
	m_TileMesh = m_Scene->GetResource()->FindMesh("Box2D");

	m_TileShader = CResourceManager::GetInst()->FindShader("TileMapEmptyShader");

	SetMeshSize(1.f, 1.f, 0.f);

	return true;
}

void CTileEmptyComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CTileEmptyComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	m_DeltaTime = DeltaTime;
}

void CTileEmptyComponent::PrevRender()
{
	CSceneComponent::PrevRender();

	CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	Resolution RS = Camera->GetResolution();

	Vector3 LB = Camera->GetWorldPos();

	Vector3 RT = LB + Vector3((float)RS.Width, (float)RS.Height, 0.f);

	int StartX, StartY, EndX, EndY;

	StartX = GetTileEmptyRenderIndexX(LB);
	StartY = GetTileEmptyRenderIndexY(LB);

	EndX = GetTileEmptyRenderIndexX(RT);
	EndY = GetTileEmptyRenderIndexY(RT);

	Matrix matView, matProj;

	matView = Camera->GetViewMatrix();

	matProj = Camera->GetProjMatrix();

	m_RenderCount = 0;

	if (m_vecTileEmpty.empty())
		return;

	for (int row = StartY; row <= EndY; row++)
	{
		for (int col = StartX; col <= EndX; col++)
		{
			int Index = row * m_CountX + col;

			// 해당 Tile의 Rendering 준비를 완료 시키고
			m_vecTileEmpty[Index]->Update(m_DeltaTime);

			if (m_EditMode)
			{
				m_vecTileEmptyInfo[m_RenderCount].TileColor = m_TileEmptyColor[(int)m_vecTileEmpty[Index]->GetTileType()];
			}

			m_vecTileEmptyInfo[m_RenderCount].matWVP = m_vecTileEmpty[Index]->GetWorldMatrix() * matView * matProj;
			m_vecTileEmptyInfo[m_RenderCount].matWVP.Transpose();

			++m_RenderCount;
		}
	}

	m_TileInfoBuffer->UpdateBuffer(&m_vecTileEmptyInfo[0], m_RenderCount);
}

void CTileEmptyComponent::Render()
{
	CSceneComponent::Render();

	if (m_BackMaterial)
	{
		m_BackMaterial->Render();

		m_BackMesh->Render();

		m_BackMaterial->Reset();
	}

	if (!m_vecTileEmpty.empty())
	{
		m_TileInfoBuffer->SetShader();

		m_TileShader->SetShader();

		m_TileMesh->RenderInstancing(m_RenderCount);

		m_TileInfoBuffer->ResetShader();
	}
}

void CTileEmptyComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CTileEmptyComponent* CTileEmptyComponent::Clone()
{
	return new CTileEmptyComponent(*this);
}

void CTileEmptyComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

	// Back Mesh
	std::string BackMeshName = m_BackMesh->GetName();

	int BackMeshNameLength = (int)BackMeshName.length();

	fwrite(&BackMeshNameLength, sizeof(int), 1, File);
	fwrite(BackMeshName.c_str(), sizeof(char), BackMeshNameLength, File);

	// Back Material
	bool MaterialEnable = false;

	if (m_BackMaterial)
		MaterialEnable = true;

	fwrite(&MaterialEnable, sizeof(bool), 1, File);

	if (MaterialEnable)
		m_BackMaterial->Save(File);

	// Tile Mesh, Shader
	std::string TileMeshName = m_TileMesh->GetName();

	int TileMeshNameLength = (int)TileMeshName.length();

	fwrite(&TileMeshNameLength, sizeof(int), 1, File);
	fwrite(TileMeshName.c_str(), sizeof(char), TileMeshNameLength, File);

	std::string TileShaderName = m_TileShader->GetName();

	int TileShaderNameLength = (int)TileShaderName.length();

	fwrite(&TileShaderName, sizeof(int), 1, File);
	fwrite(TileShaderName.c_str(), sizeof(char), TileShaderNameLength, File);

	// Tile Empty

	fwrite(&m_CountX, sizeof(int), 1, File);
	fwrite(&m_CountY, sizeof(int), 1, File);
	fwrite(&m_Count, sizeof(int), 1, File);
	fwrite(&m_TileEmptySize, sizeof(Vector3), 1, File);
	fwrite(m_TileEmptyColor, sizeof(Vector4), (int)Tile_Type::End, File);

	for (int i = 0; i < m_Count; i++)
	{
		m_vecTileEmpty[i]->Save(File);
	}

	CSceneComponent::Save(File);
}

void CTileEmptyComponent::Load(FILE* File)
{
	// Back Mesh
	char	BackMeshName[256] = {};

	int	Length = 0;

	fread(&Length, sizeof(int), 1, File);
	fread(BackMeshName, sizeof(char), Length, File);

	m_BackMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh(BackMeshName);

	// Back Material
	SetMeshSize(1.f, 1.f, 0.f);

	// BackMaterial
	bool MaterialEnable = false;

	fread(&MaterialEnable, sizeof(bool), 1, File);

	if (MaterialEnable)
	{
		m_BackMaterial = m_Scene->GetResource()->CreateMaterialEmpty<CMaterial>();
		m_BackMaterial->Load(File);
	}

	// Tile Mesh, Shader
	int TileMeshNameLength = -1;

	char	TileMeshName[256] = {};

	fread(&TileMeshNameLength, sizeof(int), 1, File);
	fread(TileMeshName, sizeof(char), TileMeshNameLength, File);

	m_TileMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh(TileMeshName);

	int TileShaderNameLength = -1;

	char	TileShaderName[256] = {};

	fread(&TileShaderNameLength, sizeof(int), 1, File);
	fread(TileShaderName, sizeof(char), TileShaderNameLength, File);

	m_TileShader = m_Scene->GetResource()->FindShader(TileShaderName);

	// Tile Empty
	fread(&m_CountX, sizeof(int), 1, File);
	fread(&m_CountY, sizeof(int), 1, File);
	fread(&m_Count, sizeof(int), 1, File);
	fread(&m_TileEmptySize, sizeof(Vector3), 1, File);
	fread(m_TileEmptyColor, sizeof(Vector4), (int)Tile_Type::End, File);

	size_t Size = m_vecTileEmpty.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecTileEmpty[i]);
	}

	// Tile 비워주고
	m_vecTileEmpty.clear();

	// 실제 Tile 들을 배치하기 위한 resize
	m_vecTileEmpty.resize(m_Count);

	for (int i = 0; i < m_Count; i++)
	{
		CTileEmpty* Tile = new CTileEmpty;

		Tile->SetOwner(this);

		Tile->Load(File);

		m_vecTileEmpty[i] = Tile;
	}

	SetWorldInfo();

	CSceneComponent::Load(File);
}

void CTileEmptyComponent::SetWorldInfo()
{
	SAFE_DELETE(m_TileInfoBuffer);

	m_TileInfoBuffer = new CStructuredBuffer;

	m_TileInfoBuffer->Init("TileEmptyInfo", 90, sizeof(TileEmptyInfo), true, m_Count, (int)Buffer_Shader_Type::Vertex);

	m_vecTileEmptyInfo.resize(m_Count);

	for (int i = 0; i < m_Count; i++)
	{
		m_vecTileEmptyInfo[i].TileColor = Vector4(1.f, 1.f, 1.f, 1.f);
	}
}
