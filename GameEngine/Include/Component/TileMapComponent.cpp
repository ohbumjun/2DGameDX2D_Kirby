#include "TileMapComponent.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Resource/Shader/TileConstantBuffer.h"
#include "../Scene/CameraManager.h"
#include "../Scene/Scene.h"

CTileMapComponent::CTileMapComponent()
{
	SetTypeID<CTileMapComponent>();
	m_RenderCount = 0;
	m_Count   = 0;
	m_CountX = 0;
	m_CountY = 0;
	m_TileShape = Tile_Shape::Rect;
	m_TileSize = Vector3(1.f, 1.f, 1.f);
	m_TileType = Tile_Type::Normal;
	m_EditMode = false;
	m_LayerName = "Back";
	m_TileInfoBuffer = nullptr;

	m_CBuffer = new CTileConstantBuffer;
	m_CBuffer->Init();

	for (int i = 0; i < (int)Tile_Type::End; i++)
	{
		m_TileColor[i] = Vector4(1.f, 1.f, 1.f, 1.f);
	}

	m_TileColor[(int)Tile_Type::Wall] = Vector4(1.f, 0.f, 0.f, 1.f);
}

CTileMapComponent::CTileMapComponent(const CTileMapComponent& com) :
CSceneComponent(com)
{
	m_BackMesh = com.m_BackMesh;

	if (com.m_BackMaterial)
		m_BackMaterial = com.m_BackMaterial->Clone();

	if (com.m_TileMaterial)
		m_TileMaterial = com.m_TileMaterial->Clone();

	if (com.m_CBuffer)
		m_CBuffer = com.m_CBuffer->Clone();

	if (com.m_TileInfoBuffer)
		m_TileInfoBuffer = com.m_TileInfoBuffer->Clone();

	for (int i = 0; i < (int)Tile_Type::End; i++)
	{
		m_TileColor[i] = com.m_TileColor[i];
	}

	m_EditMode = com.m_EditMode;
}

CTileMapComponent::~CTileMapComponent()
{
	SAFE_DELETE(m_TileInfoBuffer);
	SAFE_DELETE(m_CBuffer);

	size_t Size = m_vecTile.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecTile[i]);
	}

	m_vecTile.clear();
}

void CTileMapComponent::SetBackMaterial(CMaterial* Material)
{
	m_BackMaterial = Material->Clone();
	m_BackMaterial->SetScene(m_Scene);
}

void CTileMapComponent::SetTileMaterial(CMaterial* Material)
{
	m_TileMaterial = Material->Clone();
	m_TileMaterial->SetScene(m_Scene); ////
	m_CBuffer->SetImageSize(Vector2((float)m_TileMaterial->GetTextureWidth(), (float)m_TileMaterial->GetTextureHeight()));
}

void CTileMapComponent::SetBackBaseColor(const Vector4& Color)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->SetBaseColor(Color);
}

void CTileMapComponent::SetBackBaseColor(float r, float g, float b, float a)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->SetBaseColor(r,g,b,a);
}

void CTileMapComponent::SetBackRenderState(CRenderState* State)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->SetRenderState(State);
}

void CTileMapComponent::SetBackRenderStart(const std::string& Name)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->SetRenderState(Name);
}

void CTileMapComponent::SetBackTransparency(bool Enable)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->SetTransparency(Enable);
}

void CTileMapComponent::SetBackOpacity(float Opacity)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->SetOpacity(Opacity);
}

void CTileMapComponent::AddBackOpacity(float Opacity)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->AddOpacity(Opacity);
}

void CTileMapComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->AddTexture(Register, ShaderType, Name, Texture);
}

void CTileMapComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->AddTexture(Register, ShaderType, Name, FileName, PathName);
}

void CTileMapComponent::AddBackTextureFullPath(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->AddTexture(Register, ShaderType, Name, FullPath);
}

void CTileMapComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->AddTexture(Register, ShaderType, Name, vecFileName, PathName);
}

void CTileMapComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	CTexture* Texture)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, Texture);
}

void CTileMapComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, FileName, PathName);
}

void CTileMapComponent::SetBackTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, FullPath);
}

void CTileMapComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_BackMaterial)
		return;
	m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, vecFileName, PathName);
}

void CTileMapComponent::CreateTile(Tile_Shape Shape, int CountX, int CountY, const Vector3& Size)
{
	m_TileShape = Shape;
	m_CountX = CountX;
	m_CountY = CountY;
	m_Count = m_CountX * m_CountY;
	m_TileSize = Size;

	m_vecTile.resize(m_Count);

	for (int i = 0; i < m_CountY; i++)
	{
		for (int j = 0; j < m_CountX; j++)
		{
			CTile* Tile = new CTile;
			Tile->m_Owner = this;

			int Index = i * m_CountX + j;
			Tile->SetIndex(j, i, Index);
			Tile->SetSize(m_TileSize);
			Tile->SetShape(m_TileShape);

			m_vecTile[Index] = Tile;
		}
	}

	switch (m_TileShape)
	{
		case Tile_Shape::Rect :
		{
			Vector3 Pos;

			for (int i = 0; i < m_CountY; i++)
			{
				Pos.x = 0.f;
				Pos.y = i * m_TileSize.y;
				for (int j = 0; j < m_CountX; j++)
				{
					Pos.x = j * m_TileSize.x;

					int Index = i * m_CountX + j;
					m_vecTile[Index]->SetPos(Pos);
				}
			}
		}
			break;

		case Tile_Shape::Rhombus:
		{
			Vector3 Pos;

			for (int i = 0; i < m_CountY; i++)
			{
				if (i % 2 == 0)
					Pos.x = 0.f;
				else
					Pos.x = m_TileSize.x * 0.5f;

				Pos.y = i * m_TileSize.y * 0.5f;

				for (int j = 0; j < m_CountX; j++)
				{
					if (j > 0.f)
						Pos.x += m_TileSize.x;

					int Index = i * m_CountX + j;

					m_vecTile[Index]->SetPos(Pos);
				}
			}
		}
		break;
	}

	m_CBuffer->SetTileSize(Vector2(m_TileSize.x, m_TileSize.y));
	SetWorldInfo();
}

void CTileMapComponent::SetTileDefaultFrame(const Vector2& Start, const Vector2& End)
{
	for (int i = 0; i < m_Count; i++)
	{
		m_vecTile[i]->SetFrameStart(Start.x, Start.y);
		m_vecTile[i]->SetFrameEnd(End.x, End.y);
	}
}

void CTileMapComponent::SetTileDefaultFrame(float StartX, float StartY, float EndX, float EndY)
{
	for (int i = 0; i < m_Count; i++)
	{
		m_vecTile[i]->SetFrameStart(StartX, StartY);
		m_vecTile[i]->SetFrameEnd(EndX, EndY);
	}
}

void CTileMapComponent::SetTileFrame(int IndexX, int IndexY, const Vector2& Start, const Vector2& End)
{
	m_vecTile[IndexX * m_CountX + IndexY]->SetFrameStart(Start.x, Start.y);
	m_vecTile[IndexX * m_CountX + IndexY]->SetFrameEnd(End.x, End.y);
}

void CTileMapComponent::SetTileFrame(int Index, float StartX, float StartY, float EndX, float EndY)
{
	m_vecTile[Index]->SetFrameStart(StartX, StartY);
	m_vecTile[Index]->SetFrameEnd(EndX, EndY);
}

void CTileMapComponent::SetTileFrame(const Vector3& Pos, float StartX, float StartY, float EndX, float EndY)
{
}

void CTileMapComponent::SetTileOpacity(int IndexX, int IndexY, float Opacity)
{
	m_vecTile[IndexY * m_CountX + IndexX]->SetOpacity(Opacity);
}

void CTileMapComponent::SetTileOpacity(int Index, float Opacity)
{
	m_vecTile[Index]->SetOpacity(Opacity);
}

void CTileMapComponent::SetTileOpacity(const Vector3& Pos, float Opacity)
{}

void CTileMapComponent::SetTileColor(Tile_Type Type, float r, float g, float b, float a)
{
	m_TileColor[(int)Type] = Vector4(r, g, b, a);
}

void CTileMapComponent::SetTileColor(Tile_Type Type, const Vector4& Color)
{
	m_TileColor[(int)Type] = Color;
}

int CTileMapComponent::GetTileRenderIndexX(const Vector3& Pos)
{
	if (m_TileShape == Tile_Shape::Rect)
	{
		float ConvertX = Pos.x - GetWorldPos().x;

		int IndexX = (int)(ConvertX / m_TileSize.x);

		if (IndexX < 0)
			IndexX = 0;

		if (IndexX >= m_CountX)
			IndexX = m_CountX - 1;

		return IndexX;
	}
	else // 마름모
	{
		int IndexY = GetTileRenderIndexY(Pos);

		if (IndexY < 0)
			IndexY = 0;

		if (IndexY >= m_CountY)
			IndexY = m_CountY - 1;

		int IndexX = -1;

		float ConvertX = Pos.x - GetWorldPos().x;

		if (IndexY % 2 == 0)
			IndexX = (int)(ConvertX / m_TileSize.x);
		else
			IndexX = (int)((ConvertX - m_TileSize.x * 0.5f) / m_TileSize.x);

		if (IndexX < 0)
			IndexX = 0;

		if (IndexY >= m_CountX)
			IndexX = m_CountX - 1;

		return IndexX;
	}
}

int CTileMapComponent::GetTileRenderIndexY(const Vector3& Pos)
{
	if (m_TileShape == Tile_Shape::Rect)
	{
		float ConvertY = Pos.y - GetWorldPos().y;

		int IndexY = (int)(ConvertY / m_TileSize.y);

		if (IndexY < 0)
			IndexY = 0;
		if (IndexY >= m_CountY)
			IndexY = m_CountY - 1;

		return IndexY;
	}
	else
	{
		Vector3 ConvertPos = Pos - GetWorldPos();

		float RatioX = ConvertPos.x / m_TileSize.x;
		float RatioY = ConvertPos.y / m_TileSize.y;

		// 마름모도 먼저, 사각형을 기준으로 생각할 것이다.
		int IndexX = (int)RatioX;
		int IndexY = (int)RatioY;

		if (IndexX < 0)
			IndexX = 0;

		if (IndexX >= m_CountX)
			IndexX = m_CountX - 1;

		// 소수 부분만을 남긴다.
		RatioX -= (int)RatioX;
		RatioY -= (int)RatioY;

		// 사각형 아래쪽 부분일 경우
		if (RatioY < 0.5f)
		{
			// 좌하단 사각형 --> 그 중에서 마름모 밖
			if (RatioY < RatioX * -1.f + 0.5f)
			{
				// 제일 좌측의 경우, 모두 비어있는 공간
				if (IndexX == 0)
				{
					if (IndexY < 0)
						return 0;
					else if (IndexY >= m_CountY)
						return m_CountY - 1;
					else
						return IndexY * 2;
					// 원본 : return IndexY * 2 + 1;
				}
				// 가장 아래쪽의 경우 --> 비어있는 공간
				else if (IndexY == 0)
					return 0; // 그냥 0번째 Index Return 한다.
				// 2번째 줄의 경우
				else if (IndexY == 1)
					return 1;
				else
					return IndexY * 2 - 1;
			}
			// 우하단 사각형 --> 그중에서 마름모 밖
			else if (RatioY < RatioX - 0.5f)
			{
				if (IndexY == 0)
					return 0;
				else if (IndexY == 1)
					return 1;
				else
					return IndexY * 2 - 1;
			}
			// 하단 마름모 안
			else
				return IndexY * 2; 
		}
		// 사각형 위쪽 부분
		else if (RatioY > 0.5f)
		{
			// 좌상단 삼각형, 마름모 밖
			if (RatioY > RatioX + 0.5f)
			{
				if (IndexX == 0)
				{
					if (IndexY < 0)
						return 0;
					else if (IndexY >= m_CountY)
						return m_CountY - 1;
				}
				if (IndexY * 2 + 1 >= m_CountY)
					return m_CountY - 1;
				else
					return IndexY * 2 + 1;
			}
			// 우상단 삼각형, 마름모 밖
			else if (RatioY > RatioX * -1.f + 1.5f)
			{
				// 이 부분이 굳이 필요한 것인가 ?
				if (IndexX >= m_CountX)
				{
					if (IndexY < 0)
						return IndexY;
					else if (IndexY >= m_CountY)
						return m_CountY - 1;
				}
				if (IndexY * 2 + 1>= m_CountY)
					return m_CountY - 1;
				else
					return IndexY * 2 + 1;
			}
			// 마름모 안쪽
			return IndexY * 2;
		}
		// 사각형 정 가운데
		else
		{
			return IndexY * 2;
		}

		return -1;
	}

	return -1;
}

int CTileMapComponent::GetTileIndexX(const Vector3& Pos)
{
	// 실제 Tile의 Index 구하기
	if (m_TileShape == Tile_Shape::Rect)
	{
		Vector3 ConvertPos = Pos - GetWorldPos();

		int IndexX = (int)(ConvertPos.x / m_TileSize.x);

		if (IndexX < 0 || IndexX >= m_CountX)
			return -1;

		return IndexX;
	}
	else
	{
		Vector3 ConvertPos = Pos - GetWorldPos();

		int IndexY = GetTileIndexY(Pos);

		if (IndexY == -1)
			return -1;

		int IndexX = -1;

		if (IndexY % 2 == 0)
			IndexX = (int)(ConvertPos.x / m_TileSize.x);
		else
			IndexX = (int)((ConvertPos.x - m_TileSize.x * 0.5f) / m_TileSize.x);

		if (IndexX < 0 || IndexX >= m_CountX)
			return -1;
			
		return IndexX;
	}

	return -1;
}

int CTileMapComponent::GetTileIndexY(const Vector3& Pos)
{
	// 실제 Tile의 Index 구하기
	if (m_TileShape == Tile_Shape::Rect)
	{
		Vector3 ConvertPos = Pos - GetWorldPos();

		int IndexY = (int)(ConvertPos.y / m_TileSize.y);

		if (IndexY < 0 || IndexY >= m_CountY)
			return -1;

		return IndexY;
	}
	else
	{
		// 먼저 사각형 상의 Index를 구할 것이다.
		Vector3	ConvertPos = Pos - GetWorldPos();

		float	RatioX = ConvertPos.x / m_TileSize.x;
		float	RatioY = ConvertPos.y / m_TileSize.y;

		int	IndexX = (int)RatioX;
		int	IndexY = (int)RatioY;

		if (IndexX < 0 || IndexX >= m_CountX)
			return -1;
		if (IndexY < 0 || IndexY >= m_CountY)
			return -1;

		// 정수 부분을 제거하여 소수점 부분만을 남겨준다.
		RatioX -= (int)RatioX;
		RatioY -= (int)RatioY;

		if (RatioY < 0.5f)
		{
			// 왼쪽 하단
			if (RatioY < RatioX * -1.f + 0.5f)
			{
				// 가장 왼쪽 --> 비어있는 공간
				if (IndexX == 0)
					return -1;

				// 가장 아래 --> 비어있는 공간
				else if (IndexY == 0)
					return -1;

				else if (IndexY == 1)
					return 1;

				return IndexY * 2 - 1;
			}
			// 오른쪽 하단
			else if (RatioY < RatioX - 0.5f)
			{
				// 가장 아래 --> 비어있는 공간
				if (IndexY == 0)
					return -1;
				else if (IndexY == 1)
					return 1;
				return IndexY * 2 - 1;
			}
			// 마름모 안의 영역
			else
				return IndexY * 2;
		}
		else if (RatioY > 0.5f)
		{
			// 왼쪽 상단
			if (RatioY > RatioX - 0.5f)
			{
				// 가장 왼쪽 --> 비어있는 공간
				if (IndexX == 0)
					return -1;
				else if (IndexY * 2 + 1 >= m_CountY)
					return -1;
				return IndexY * 2 + 1;
			}
			// 오른쪽 상단
			else if (RatioY > RatioX * -1.f + 1.5f)
			{
				// 가장 오른쪽
				if (IndexX >= m_CountX - 1)
					return -1;
				if (IndexY * 2 + 1 >= m_CountY)
					return -1;
				return IndexY * 2 + 1;
			}
			// 마름모 안 영역 
			else
				return IndexY * 2;
		}
		// 정 가운데 
		else
			return IndexY * 2;
	}

	return -1;
}

int CTileMapComponent::GetTileIndex(const Vector3& Pos)
{
	int IndexX = GetTileIndexX(Pos);
	int IndexY = GetTileIndexY(Pos);

	if (IndexX == -1 || IndexY == -1)
		return -1;

	return IndexY * m_CountX + IndexX;
}

CTile* CTileMapComponent::GetTile(const Vector3& Pos)
{
	int Index = GetTileIndex(Pos);

	if (Index == -1)
		return nullptr;

	return m_vecTile[Index];
}

CTile* CTileMapComponent::GetTile(int IndexX, int IndexY)
{
	if (IndexX < 0 || IndexX >= m_CountX || IndexY < 0 || IndexX >= m_CountY)
		return nullptr;

	return m_vecTile[IndexX * m_CountX + IndexY];
}

CTile* CTileMapComponent::GetTile(int Index)
{
	if (Index < 0 || Index >= m_CountX * m_CountY)
		return nullptr;

	return m_vecTile[Index];
}

void CTileMapComponent::SetWorldInfo()
{
	SAFE_DELETE(m_TileInfoBuffer);

	m_TileInfoBuffer = new CStructuredBuffer;
	m_TileInfoBuffer->Init("TileInfo", 30, sizeof(TileInfo), true, m_Count, (int)Buffer_Shader_Type::Vertex);

	m_vecTileInfo.resize(m_Count);

	for (int i = 0; i < m_Count; i++)
	{
		m_vecTileInfo[i].TileColor = Vector4(1.f, 1.f, 1.f, 1.f);
		m_vecTileInfo[i].Opacity = 1.f;
	}
}

void CTileMapComponent::Start()
{
	CSceneComponent::Start();
}

bool CTileMapComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	// Mesh 세팅
	m_BackMesh = m_Scene->GetResource()->FindMesh("SpriteMesh");

	SetMeshSize(1.f, 1.f, 0.f);

	// 상수 버퍼 세팅
	m_CBuffer = new CTileConstantBuffer;
	m_CBuffer->Init();

	return true;
}

void CTileMapComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CTileMapComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	Resolution RS = Camera->GetResolution();

	Vector3 LB = Camera->GetWorldPos();
	Vector3 RT = LB + Vector3((float)RS.Width, (float)RS.Height, 0.f);

	int StartX, StartY, EndX, EndY;

	// 화면에 보여지는 부분만 선별할 것이다.
	StartX = GetTileRenderIndexX(LB);
	StartY = GetTileRenderIndexY(LB);

	EndX = GetTileRenderIndexX(RT);
	EndY = GetTileRenderIndexY(RT);

	// 마름모일 경우 영역을 확장한다
	if (m_TileShape == Tile_Shape::Rhombus)
	{
		StartX -= 1;
		StartY -= 1;

		if (StartX < 0)
			StartX = 0;
		if (StartY < 0)
			StartY = 0;

		EndX += 1;
		EndY += 1;

		if (EndX >= m_CountX)
			EndX = m_CountX - 1;
		if (EndY >= m_CountY)
			EndY = m_CountY - 1;
	}

	Matrix matView, matProj;
	matView = Camera->GetViewMatrix();
	matProj = Camera->GetProjMatrix();

	m_RenderCount = 0;

	for (int row = StartY; row <= EndY; row++)
	{
		for (int col = StartX; col <= EndX; col++)
		{
			int Index = row * m_CountX + col;

			m_vecTile[Index]->Update(DeltaTime);

			if (m_vecTile[Index]->GetRender())
			{
				if (m_EditMode)
				{
					// 색상 세팅하기
					m_vecTileInfo[m_RenderCount].TileColor = m_TileColor[(int)m_vecTile[Index]->GetTileType()];
				}

				m_vecTileInfo[m_RenderCount].TileStart = m_vecTile[Index]->GetFrameStart();
				m_vecTileInfo[m_RenderCount].TileEnd = m_vecTile[Index]->GetFrameEnd();
				m_vecTileInfo[m_RenderCount].Opacity = m_vecTile[Index]->GetOpacity();
				m_vecTileInfo[m_RenderCount].matWVP = m_vecTile[Index]->GetWorldMatrix();
				m_vecTileInfo[m_RenderCount].matWVP.Transpose();
				++m_RenderCount;

			}
		}
	}

	// 구조화 버퍼 내용 넘겨주기
	m_TileInfoBuffer->UpdateBuffer(&m_vecTileInfo[0], m_RenderCount);
}

void CTileMapComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CTileMapComponent::Render()
{
	CSceneComponent::Render();

	// 배경 먼저
	if (m_BackMaterial)
	{
		m_BackMaterial->Render();
		m_BackMesh->Render();
		m_BackMaterial->Reset();
	}

	// 그 다음 Tile 그리기
	if (m_TileMaterial)
	{
		// 구조화 버퍼 내용 셰이더 세팅해주고
		m_TileInfoBuffer->SetShader();

		// 상수 버퍼 내용 넘겨주고 --> 사실상 이미지 사이즈, TileSize만 넘겨준다.
		m_CBuffer->UpdateCBuffer();

		m_TileMaterial->Render();

		m_BackMesh->RenderInstancing(m_RenderCount);

		m_TileMaterial->Reset();

		// 구조화 버퍼 내용 초기화 해주고 
		m_TileInfoBuffer->ResetShader();
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
	std::string MeshName = m_BackMesh->GetName();

	int Length = (int)MeshName.length();

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(MeshName.c_str(), sizeof(char), Length, File);

	m_BackMaterial->Save(File);

	CSceneComponent::Save(File);
}

void CTileMapComponent::Load(FILE* File)
{
	int Length = 0;
	fread(&Length, sizeof(int), 1, File);

	char Name[MAX_PATH] = {};
	fread(Name, sizeof(char), Length, File);

	m_BackMesh = m_Scene->GetResource()->FindMesh(Name);

	m_BackMaterial = m_Scene->GetResource()->CreateMaterialEmpty<CMaterial>();

	m_BackMaterial->Load(File);

	CSceneComponent::Load(File);
}
