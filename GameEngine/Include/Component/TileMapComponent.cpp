#include "TileMapComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Shader/TileConstantBuffer.h"

CTileMapComponent::CTileMapComponent()
 {
	 SetTypeID<CTileMapComponent>();
	 m_Render = true;

	 m_CountX = 0;
	 m_CountY = 0;
	 m_RenderCount = 0;

	 m_LayerName = "Tile";
 }

 CTileMapComponent::CTileMapComponent(const CTileMapComponent& com)
 {
	 m_BackMesh = com.m_BackMesh;

	 if (com.m_BackMaterial)
		 m_BackMaterial = com.m_BackMaterial->Clone();

	 if (com.m_TileMaterial)
		 m_TileMaterial = com.m_TileMaterial->Clone();
 }

 CTileMapComponent::~CTileMapComponent()
{}

 void CTileMapComponent::SetBackMaterial(CMaterial* Material)
 {
	 m_BackMaterial = Material->Clone();
 }

void CTileMapComponent::SetTileMaterial(CMaterial* Material)
{
	m_TileMaterial = Material->Clone();
}

void CTileMapComponent::CreateTile(Tile_Shape Shape, int CountX, int CountY, const Vector3& Size)
 {
	m_CountX = CountX;
	m_CountY = CountY;
	m_TileShape = Shape;
	m_TileSize = Size;

	m_vecTile.resize(CountY * CountX);

 	for (int y = 0; y < CountY; y++) // За
	{
		for (int x = 0; x < CountX; x++) // ї­ 
		{
			CTile* Tile = new CTile;
			Tile->m_TileShape = m_TileShape;
			Tile->m_Owner = this;
			Tile->m_Size = m_TileSize;

			int Index = y * CountX + x;

			Tile->SetIndex(x, y, Index);
			m_vecTile[y * CountX + x] = Tile;
		}
	}

	switch (m_TileShape)
	{
 	case Tile_Shape::Rect :
 		{
 			for (int y = 0; y < m_CountY; y++)
 			{
 				for (int x = 0; x < m_CountX; x++)
 				{
					Vector3 Pos = Vector3((float)x * m_TileSize.x, y * m_TileSize.y, 1.f);
					m_vecTile[y * m_CountX + x]->m_Pos = Pos;
 				}
 			}
 		}
		break;
 	case Tile_Shape::Rhombus :
 		{
			Vector3 Pos;

			for (int y = 0; y < m_CountY; y++)
			{
				if (y % 2 == 0)
					Pos.x = 0.f;
				else
					Pos.x = m_TileSize.x * 0.5f;

				Pos.y = y * m_TileSize.y * 0.5f;

				for (int x = 0; x < m_CountX; x++)
				{
					if (x > 0)
						Pos.x += m_TileSize.x;

					m_vecTile[y * m_CountX + x]->SetPos(Pos);
				}
			}
 		}
		break;
	}

	m_CBuffer->SetTileSize(Vector2(m_TileSize.x, m_TileSize.y));
 }

void CTileMapComponent::SetBackBaseColor(const Vector4& Color)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->SetBaseColor(Color);
 }

 void CTileMapComponent::SetBackBaseColor(float r, float g, float b, float a)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->SetBaseColor(r,g,b,a);
 }

 void CTileMapComponent::SetBackRenderState(CRenderState* State)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->SetRenderState(State);

 }

 void CTileMapComponent::SetBackRenderState(const std::string& Name)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->SetRenderState(Name);

 }

 void CTileMapComponent::SetBackTransparency(bool Enable)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->SetTransparency(Enable);
 }

 void CTileMapComponent::SetBackOpacity(float Opacity)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->SetOpacity(Opacity);
 }

 void CTileMapComponent::AddBackOpacity(float Opacity)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->AddOpacity(Opacity);
 }

 void CTileMapComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->AddTexture(Register, ShaderType, Name, Texture);
 }

 void CTileMapComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
 {
	 if (m_BackMaterial)
		m_BackMaterial->AddTexture(Register, ShaderType, Name, FileName, PathName);
 }

 void CTileMapComponent::AddBackTextureFullPath(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->AddTexture(Register, ShaderType, Name, FullPath);
 }

 void CTileMapComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	 if (m_BackMaterial)
		 m_BackMaterial->AddTexture(Register, ShaderType, Name, vecFileName, PathName);
 }

 void CTileMapComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	CTexture* Texture)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, Texture);
 }

 void CTileMapComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, FileName, PathName);
 }

 void CTileMapComponent::SetBackTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, FullPath);
 }

 void CTileMapComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
 {
	 if (m_BackMaterial)
		 m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, vecFileName, PathName);
 }

 void CTileMapComponent::Start()
{
	CSceneComponent::Start();

}

 bool CTileMapComponent::Init()
{
	 if (!CSceneComponent::Init())
		 return false;

	 m_BackMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh("SpriteMesh");
	 m_BackMaterial = m_Scene->GetResource()->FindMaterial("BaseTexture");
	 SetMeshSize(1.f, 0.f, 0.f);
	 SetWorldScale((float)m_BackMaterial->GetTexture()->GetWidth(),
		 (float)m_BackMaterial->GetTexture()->GetHeight(), 1.f);

	 return true;
}

 void CTileMapComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

 void CTileMapComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

 void CTileMapComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

 void CTileMapComponent::Render()
{
	CSceneComponent::Render();

	if (!m_BackMaterial || !m_BackMesh)
		return;

	if (m_BackMaterial)
	{
		m_BackMaterial->Render();

		m_BackMesh->Render();

		m_BackMaterial->Reset();
	}
}

 void CTileMapComponent::PostRender()
{
	CSceneComponent::PostRender();
}

 CTileMapComponent* CTileMapComponent::Clone()
{
	 return new CTileMapComponent(*this);
}

 void CTileMapComponent::Save(FILE* File)
{
	 int MeshNameLength = (int)m_BackMesh->GetName().length();
	 fwrite(&MeshNameLength, sizeof(int), 1, File);
	 fwrite(m_BackMesh->GetName().c_str(), sizeof(char), MeshNameLength, File);

	 m_BackMaterial->Save(File);

	CSceneComponent::Save(File);
}

 void CTileMapComponent::Load(FILE* File)
{
	 int NameLength = 0;
	 fread(&NameLength, sizeof(int), 1, File);

	 char MeshName[MAX_PATH] = {};
	 fread(MeshName, sizeof(char), NameLength, File);
	 m_BackMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh(MeshName);

	 m_BackMaterial = m_Scene->GetResource()->CreateMaterialEmpty<CMaterial>();
	 m_BackMaterial->Load(File);

	CSceneComponent::Load(File);
}