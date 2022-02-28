#include "TileEmptyComponent.h"
#include "TileEmpty.h"
#include "../Resource/Shader/StructuredBuffer.h"

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

}

void CTileEmptyComponent::SetTileEmptyOpacity(int IndexX, int IndexY, float Opacity)
{}

void CTileEmptyComponent::SetTileEmptyOpacity(int Index, float Opacity)
{}

void CTileEmptyComponent::SetTileEmptyOpacity(const Vector3& Pos, float Opacity)
{}

void CTileEmptyComponent::SetTileEmptyColor(Tile_Type Type, float r, float g, float b, float a)
{}

void CTileEmptyComponent::SetTileEmptyColor(Tile_Type Type, const Vector4& Color)
{}

int CTileEmptyComponent::GetTileEmptyIndexX(const Vector3& Pos)
{}

int CTileEmptyComponent::GetTileEmptyIndexY(const Vector3& Pos)
{}

int CTileEmptyComponent::GetTileEmptyIndex(const Vector3& Pos)
{}

CTileEmpty* CTileEmptyComponent::GetTileEmpty(const Vector3& Pos)
{}

CTileEmpty* CTileEmptyComponent::GetTileEmpty(int x, int y)
{}

CTileEmpty* CTileEmptyComponent::GetTileEmpty(int Index)
{}

int CTileEmptyComponent::GetTileEmptyRenderIndexX(const Vector3& Pos)
{}

int CTileEmptyComponent::GetTileEmptyRenderIndexY(const Vector3& Pos)
{}

void CTileEmptyComponent::Start()
{
	CSceneComponent::Start();
}

bool CTileEmptyComponent::Init()
{
	return CSceneComponent::Init();
}

void CTileEmptyComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CTileEmptyComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CTileEmptyComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CTileEmptyComponent::Render()
{
	CSceneComponent::Render();
}

void CTileEmptyComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CTileEmptyComponent* CTileEmptyComponent::Clone()
{
	return CSceneComponent::Clone();
}

void CTileEmptyComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);
}

void CTileEmptyComponent::Load(FILE* File)
{
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
		m_vecTileEmptyInfo[i].Opacity = 1.f;
	}
}
