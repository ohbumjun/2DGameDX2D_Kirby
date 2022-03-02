#include "BackGroundComponent.h"
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

CBackGroundComponent::CBackGroundComponent()
{
	SetTypeID<CBackGroundComponent>();
	m_Render = true;

	m_LayerName = "Back";

	m_DeltaTime = 0.f;

	m_EditMode = false;
}

CBackGroundComponent::CBackGroundComponent(const CBackGroundComponent& Component)
{
	m_ImageMesh = Component.m_ImageMesh;

	if (Component.m_TileImageMaterial)
		m_TileImageMaterial = Component.m_TileImageMaterial->Clone();

	if (Component.m_BackGroundMaterial)
		m_BackGroundMaterial = Component.m_BackGroundMaterial->Clone();

	if (Component.m_TileInfoBuffer)
		m_TileInfoBuffer = Component.m_TileInfoBuffer->Clone();

	for (int i = 0; i < (int)Tile_Type::End; i++)
	{
		m_TileEmptyColor[i] = Component.m_TileEmptyColor[i];
	}

	m_TileEmptyColor[(int)Tile_Type::Wall] = Vector4(1.f, 0.f, 0.f, 1.f);
	m_TileEmptyColor[(int)Tile_Type::Ceiling] = Vector4(0.f, 1.f, 0.f, 1.f);
	m_TileEmptyColor[(int)Tile_Type::Water] = Vector4(0.f, 0.f, 1.f, 1.f);


	m_EditMode = Component.m_EditMode;
}

CBackGroundComponent::~CBackGroundComponent()
{
	SAFE_DELETE(m_TileInfoBuffer);

	size_t Size = m_vecTileEmpty.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecTileEmpty[i]);
	}

	m_vecTileEmpty.clear();
}

void CBackGroundComponent::SetBackGroundMaterial(CMaterial* Material)
{
	m_BackGroundMaterial = Material->Clone();

	m_BackGroundMaterial->SetScene(m_Scene);

	/*
	if (!m_TileImageMaterial->EmptyTexture())
	{
		SetWorldScale((float)m_TileImageMaterial->GetTextureWidth(),
			(float)m_TileImageMaterial->GetTextureHeight(), 1.f);
	}
	*/
}

void CBackGroundComponent::SetBackGroundBaseColor(const Vector4& Color)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetBaseColor(Color);
}

void CBackGroundComponent::SetBackGroundBaseColor(float r, float g, float b, float a)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetBaseColor(r, g, b, a);
}

void CBackGroundComponent::SetBackGroundRenderState(CRenderState* State)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetRenderState(State);
}

void CBackGroundComponent::SetBackGroundRenderState(const std::string& Name)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetRenderState(Name);
}

void CBackGroundComponent::SetBackGroundTransparency(bool Enable)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetTransparency(Enable);
}

void CBackGroundComponent::SetBackGroundOpacity(float Opacity)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetOpacity(Opacity);
}

void CBackGroundComponent::AddBackGroundOpacity(float Opacity)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->AddOpacity(Opacity);
}

void CBackGroundComponent::AddBackGroundTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->AddTexture(Register, ShaderType, Name, Texture);
}

void CBackGroundComponent::AddBackGroundTexture(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->AddTexture(Register, ShaderType, Name, FileName, PathName);
}

void CBackGroundComponent::AddBackGroundTextureFullPath(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->AddTextureFullPath(Register, ShaderType, Name, FullPath);
}

void CBackGroundComponent::AddBackGroundTexture(int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->AddTexture(Register, ShaderType, Name, vecFileName, PathName);
}

void CBackGroundComponent::SetBackGroundTexture(int Index, int Register, int ShaderType, const std::string& Name,
	CTexture* Texture)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetTexture(Index, Register, ShaderType, Name, Texture);
}

void CBackGroundComponent::SetBackGroundTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetTexture(Index, Register, ShaderType, Name, FileName, PathName);
}

void CBackGroundComponent::SetBackGroundTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetTextureFullPath(Index, Register, ShaderType, Name, FullPath);
}

void CBackGroundComponent::SetBackGroundTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetTexture(Index, Register, ShaderType, Name, vecFileName, PathName);
}

void CBackGroundComponent::SetTileImageMaterial(CMaterial* Material)
{
	m_TileImageMaterial = Material->Clone();

	m_TileImageMaterial->SetScene(m_Scene);
	/*
	if (!m_TileImageMaterial->EmptyTexture())
	{
		SetWorldScale((float)m_TileImageMaterial->GetTextureWidth(),
			(float)m_TileImageMaterial->GetTextureHeight(), 1.f);
	}
	*/
}

void CBackGroundComponent::SetTileImageBaseColor(const Vector4& Color)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->SetBaseColor(Color);
}

void CBackGroundComponent::SetTileImageBaseColor(float r, float g, float b, float a)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->SetBaseColor(r, g, b, a);
}

void CBackGroundComponent::SetTileImageRenderState(CRenderState* State)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->SetRenderState(State);
}

void CBackGroundComponent::SetTileImageRenderState(const std::string& Name)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->SetRenderState(Name);
}

void CBackGroundComponent::SetTileImageTransparency(bool Enable)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->SetTransparency(Enable);
}

void CBackGroundComponent::SetTileImageOpacity(float Opacity)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->SetOpacity(Opacity);
}

void CBackGroundComponent::AddTileImageOpacity(float Opacity)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->AddOpacity(Opacity);
}

void CBackGroundComponent::AddTileImageTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->AddTexture(Register, ShaderType, Name, Texture);
}

void CBackGroundComponent::AddTileImageTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->AddTexture(Register, ShaderType, Name, FileName, PathName);
}

void CBackGroundComponent::AddTileImageTextureFullPath(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->AddTextureFullPath(Register, ShaderType, Name, FullPath);
}

void CBackGroundComponent::AddTileImageTexture(int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->AddTexture(Register, ShaderType, Name, vecFileName, PathName);
}

void CBackGroundComponent::SetTileImageTexture(int Index, int Register, int ShaderType, const std::string& Name,
	CTexture* Texture)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->SetTexture(Index, Register, ShaderType, Name, Texture);
}

void CBackGroundComponent::SetTileImageTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->SetTexture(Index, Register, ShaderType, Name, FileName, PathName);
}

void CBackGroundComponent::SetTileImageTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->SetTextureFullPath(Index, Register, ShaderType, Name, FullPath);
}

void CBackGroundComponent::SetTileImageTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_TileImageMaterial)
		return;

	m_TileImageMaterial->SetTexture(Index, Register, ShaderType, Name, vecFileName, PathName);
}

void CBackGroundComponent::CreateTileEmpty(int CountX, int CountY, const Vector3& Size)
{
	// ���� Tile ���� �����ش�.
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

			Tile->SetSize(Size);

			m_vecTileEmpty[Index] = Tile;
		}
	}

	// Tile ��ġ ����
	Vector3 Pos = Vector3(0.f, 0.f, 1.f);

	for (int row = 0; row < m_CountY; row++)
	{
		Pos.x = 0.f;
		Pos.y = row * m_TileEmptySize.y;

		for (int col = 0; col < m_CountX; col++)
		{
			Pos.x = col * m_TileEmptySize.x;

			int Index = row * m_CountX + col;

			m_vecTileEmpty[Index]->SetPos(Pos);
		}
	}

	// �׿� ���� ����
	SetWorldInfo();

}

void CBackGroundComponent::SetTileEmptyColor(Tile_Type Type, float r, float g, float b, float a)
{
	m_TileEmptyColor[(int)Type] = Vector4(r, g, b, a);
}

void CBackGroundComponent::SetTileEmptyColor(Tile_Type Type, const Vector4& Color)
{
	m_TileEmptyColor[(int)Type] = Color;
}

void CBackGroundComponent::SetTileDefaultSize(float x, float y)
{
	m_TileEmptySize = Vector3(x, y, 1.f);

	for (int i = 0; i < m_Count; i++)
	{
		m_vecTileEmpty[i]->m_Size = m_TileEmptySize;
	}

	Vector3 Pos = {};

	for (int row = 0; row < m_CountY; row++)
	{
		Pos.x = 0.f;
		Pos.y = row * m_TileEmptySize.y;

		for (int col = 0; col < m_CountX; col++)
		{
			Pos.x = col * m_TileEmptySize.x;

			int Index = row * m_CountX + col;

			m_vecTileEmpty[Index]->SetPos(Pos);
		}
	}
}

int CBackGroundComponent::GetTileEmptyIndexX(const Vector3& Pos)
{
	float	ConvertX = Pos.x - GetWorldPos().x;

	int	IndexX = (int)(ConvertX / m_TileEmptySize.x);

	if (IndexX < 0 || IndexX >= m_CountX)
		return -1;

	return IndexX;
}

int CBackGroundComponent::GetTileEmptyIndexX(float WorldPosX)
{
	float ConvertX = WorldPosX - GetWorldPos().x;

	int IndexX = (int)(ConvertX / m_TileEmptySize.x);

	if (IndexX < 0 || IndexX >= m_CountX)
		return -1;

	return IndexX;
}

int CBackGroundComponent::GetTileEmptyIndexY(float WorldPosY)
{
	float ConvertY = WorldPosY - GetWorldPos().y;

	int IndexY = (int)(ConvertY / m_TileEmptySize.y);

	if (IndexY < 0 || IndexY >= m_CountY)
		return -1;

	return IndexY;
}

int CBackGroundComponent::GetTileEmptyIndexY(const Vector3& Pos)
{
	float ConvertY = Pos.y - GetWorldPos().y;

	int IndexY = (int)(ConvertY / m_TileEmptySize.y);

	if (IndexY < 0 || IndexY >= m_CountY)
		return -1;

	return IndexY;
}

int CBackGroundComponent::GetTileEmptyIndex(const Vector3& Pos)
{
	int	IndexX = GetTileEmptyIndexX(Pos);
	int	IndexY = GetTileEmptyIndexY(Pos);

	if (IndexX == -1 || IndexY == -1)
		return -1;

	return IndexY * m_CountX + IndexX;
}

CTileEmpty* CBackGroundComponent::GetTileEmpty(const Vector3& Pos)
{
	int Index = GetTileEmptyIndex(Pos);

	if (Index == -1)
		return nullptr;

	return m_vecTileEmpty[Index];
}

CTileEmpty* CBackGroundComponent::GetTileEmpty(int x, int y)
{
	if (x < 0 || x >= m_CountX || y < 0 || y >= m_CountY)
		return nullptr;

	int ConvertIdx = y * m_CountX + x;

	return m_vecTileEmpty[y * m_CountX + x];
}

CTileEmpty* CBackGroundComponent::GetTileEmpty(int Index)
{
	if (Index < 0 || Index >= m_CountX * m_CountY)
		return nullptr;

	return m_vecTileEmpty[Index];
}

int CBackGroundComponent::GetTileEmptyRenderIndexX(const Vector3& Pos)
{
	float ConvertX = Pos.x - GetWorldPos().x;

	int IndexX = (int)(ConvertX / m_TileEmptySize.x);

	if (IndexX < 0)
		return 0;
	if (IndexX >= m_CountX)
		return m_CountX - 1;

	return IndexX;
}

int CBackGroundComponent::GetTileEmptyRenderIndexY(const Vector3& Pos)
{
	float ConvertY = Pos.y - GetWorldPos().y;

	int IndexY = (int)(ConvertY / m_TileEmptySize.y);

	if (IndexY < 0)
		return 0;

	if (IndexY >= m_CountX)
		return m_CountX - 1;

	return IndexY;
}

void CBackGroundComponent::Start()
{
	CSceneComponent::Start();
}

bool CBackGroundComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	// Back Mesh
	m_ImageMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh("SpriteMesh");

	SetMeshSize(1.f, 1.f, 0.f);

	// TileBase Material
	m_Scene->GetResource()->CreateMaterial<CMaterial>("TileBaseMaterial");

	m_TileImageMaterial = m_Scene->GetResource()->FindMaterial("TileBaseMaterial");

	m_TileImageMaterial->SetShader("Mesh2DShader");

	// Back Material
	m_Scene->GetResource()->CreateMaterial<CMaterial>("BackMaterial");

	m_BackGroundMaterial = m_Scene->GetResource()->FindMaterial("BackMaterial");

	m_BackGroundMaterial->SetShader("Mesh2DShader");

	// Tile
	m_TileMesh = m_Scene->GetResource()->FindMesh("Box2D");

	m_TileShader = CResourceManager::GetInst()->FindShader("TileMapEmptyShader");

	SetWorldScale(100.f, 100.f, 1.f);

	return true;
}

void CBackGroundComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CBackGroundComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	m_DeltaTime = DeltaTime;
}

void CBackGroundComponent::PrevRender()
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

	if (m_EditMode)
	{
		for (int row = StartY; row <= EndY; row++)
		{
			for (int col = StartX; col <= EndX; col++)
			{
				int Index = row * m_CountX + col;

				if (Index >= m_Count)
					continue;

				// �ش� Tile�� Rendering �غ� �Ϸ� ��Ű��
				m_vecTileEmpty[Index]->Update(m_DeltaTime);

				m_vecTileEmptyInfo[m_RenderCount].TileColor = m_TileEmptyColor[(int)m_vecTileEmpty[Index]->GetTileType()];

				m_vecTileEmptyInfo[m_RenderCount].matWVP = m_vecTileEmpty[Index]->GetWorldMatrix() * matView * matProj;
				m_vecTileEmptyInfo[m_RenderCount].matWVP.Transpose();

				++m_RenderCount;
			}
		}
	}

	m_TileInfoBuffer->UpdateBuffer(&m_vecTileEmptyInfo[0], m_RenderCount);
}

void CBackGroundComponent::Render()
{
	CSceneComponent::Render();

	// ��� ���� ����
	if (m_BackGroundMaterial)
	{
		m_BackGroundMaterial->Render();

		m_ImageMesh->Render();

		m_BackGroundMaterial->Reset();
	}

	// Tile Image �׸���
	if (m_TileImageMaterial)
	{
		m_TileImageMaterial->Render();

		m_ImageMesh->Render();

		m_TileImageMaterial->Reset();
	}

	if (!m_vecTileEmpty.empty())
	{
		m_TileInfoBuffer->SetShader();

		m_TileShader->SetShader();

		m_TileMesh->RenderInstancing(m_RenderCount);

		m_TileInfoBuffer->ResetShader();
	}
}

void CBackGroundComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CBackGroundComponent* CBackGroundComponent::Clone()
{
	return new CBackGroundComponent(*this);
}

void CBackGroundComponent::Save(FILE* File)
{

	// Back Mesh
	std::string BackMeshName = m_ImageMesh->GetName();

	int BackMeshNameLength = (int)BackMeshName.length();

	fwrite(&BackMeshNameLength, sizeof(int), 1, File);
	fwrite(BackMeshName.c_str(), sizeof(char), BackMeshNameLength, File);

	// Back Material
	bool BackMaterialEnable = false;

	if (m_BackGroundMaterial)
		BackMaterialEnable = true;

	fwrite(&BackMaterialEnable, sizeof(bool), 1, File);

	if (BackMaterialEnable)
		m_BackGroundMaterial->Save(File);

	// Tile Base Material
	bool TileBaseMaterialEnable = false;

	if (m_TileImageMaterial)
		TileBaseMaterialEnable = true;

	fwrite(&TileBaseMaterialEnable, sizeof(bool), 1, File);

	if (TileBaseMaterialEnable)
		m_TileImageMaterial->Save(File);

	// Tile Mesh, Shader
	std::string TileMeshName = m_TileMesh->GetName();

	int TileMeshNameLength = (int)TileMeshName.length();

	fwrite(&TileMeshNameLength, sizeof(int), 1, File);
	fwrite(m_TileMesh->GetName().c_str(), sizeof(char), TileMeshNameLength, File);

	std::string TileShaderName = m_TileShader->GetName();

	int TileShaderNameLength = (int)TileShaderName.length();

	fwrite(&TileShaderNameLength, sizeof(int), 1, File);
	fwrite(m_TileShader->GetName().c_str(), sizeof(char), TileShaderNameLength, File);

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

void CBackGroundComponent::Load(FILE* File)
{
	// Back Mesh
	char	BackMeshName[256] = {};

	int	Length = 0;

	fread(&Length, sizeof(int), 1, File);
	fread(BackMeshName, sizeof(char), Length, File);

	m_ImageMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh(BackMeshName);

	SetMeshSize(1.f, 1.f, 0.f);

	// Back Material
	bool BackMaterialEnable = false;

	fread(&BackMaterialEnable, sizeof(bool), 1, File);

	if (BackMaterialEnable)
	{
		m_BackGroundMaterial = m_Scene->GetResource()->CreateMaterialEmpty<CMaterial>();
		m_BackGroundMaterial->Load(File);
	}

	// Tile Base Material
	bool TileBaseMaterialEnable = false;

	fread(&TileBaseMaterialEnable, sizeof(bool), 1, File);

	if (TileBaseMaterialEnable)
	{
		m_TileImageMaterial = m_Scene->GetResource()->CreateMaterialEmpty<CMaterial>();
		m_TileImageMaterial->Load(File);
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

	// Tile ����ְ�
	m_vecTileEmpty.clear();

	// ���� Tile ���� ��ġ�ϱ� ���� resize
	m_vecTileEmpty.resize(m_Count);

	for (int i = 0; i < m_Count; i++)
	{
		CTileEmpty* Tile = new CTileEmpty;

		Tile->SetOwner(this);

		Tile->Load(File);

		m_vecTileEmpty[i] = Tile;
	}

	SetWorldInfo();

	m_EditMode = false;

	CSceneComponent::Load(File);
}

void CBackGroundComponent::SetWorldInfo()
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
