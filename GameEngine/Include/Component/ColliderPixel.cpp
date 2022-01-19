#include "ColliderPixel.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "ColliderBox2D.h"
#include "ColliderCircle.h"
#include "../PathManager.h"
#include "../Device.h"
#include "../Collision/Collision.h"
#include "../Component/CameraComponent.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "../Scene/CameraManager.h"

CColliderPixel::CColliderPixel() :
	m_Info{}
{
	SetTypeID<CColliderPixel>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;
	m_ColliderType = Collider_Type::Pixel;
}

CColliderPixel::CColliderPixel(const CColliderPixel& Pixel) : CColliderComponent(Pixel)
{
	m_Info = Pixel.m_Info;

	++m_Info.RefCount;

	m_PixelMesh = Pixel.m_PixelMesh;
	m_PixelShader = Pixel.m_PixelShader;
}

CColliderPixel::~CColliderPixel()
{
	--m_Info.RefCount;
	if (m_Info.RefCount <= 0)
	{
		SAFE_RELEASE(m_Info.SRV);
		SAFE_DELETE_ARRAY(m_Info.Pixel);
	}
}

void CColliderPixel::SetInfo(const TCHAR* FileName, const std::string& PathName)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	TCHAR FullPath[MAX_PATH] = {};

	if (Path)
		lstrcpy(FullPath, Path->Path);
	lstrcat(FullPath, FileName);

	return SetInfoFullPath(FullPath);
}

void CColliderPixel::SetInfoFullPath(const TCHAR* FullPath)
{
	TCHAR _FileExt[_MAX_EXT] = {};
	_wsplitpath_s(FullPath, nullptr, 0, nullptr, 0, nullptr, 0, _FileExt, _MAX_EXT);

	char Ext[_MAX_EXT] = {};

#ifdef UNICODE
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, _FileExt, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, _FileExt, -1, Ext, ConvertLength, 0, 0);
#else
	strcpy_s(Ext, _FileExt);
#endif

	_strupr_s(Ext);

	ScratchImage Image = {};

	if (strcmp(Ext, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(FullPath, DDS_FLAGS_NONE, nullptr, Image)))
		{
			return;
		}
	}

	else if (strcmp(Ext, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(FullPath, nullptr, Image)))
		{
			return;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(FullPath, WIC_FLAGS_NONE, nullptr, Image)))
		{
			return;
		}
	}

	m_Info.Width = (unsigned int)Image.GetImages()[0].width;
	m_Info.Height = (unsigned int)Image.GetImages()[0].height;

	m_Info.Pixel = new unsigned char[Image.GetPixelsSize()];
	memcpy(m_Info.Pixel, Image.GetPixels(), Image.GetPixelsSize());

	// ���� BMP ��� ����� ���� 
	if (strcmp(Ext, ".BMP") == 0)
	{
		unsigned int LineSize = m_Info.Width * 4;

		unsigned char* Line = new unsigned char[LineSize];

		int HalfHeight = m_Info.Height / 2;

		for (int i = 0; i < HalfHeight; i++)
		{
			memcpy(Line, &m_Info.Pixel[i * LineSize], LineSize);
			memcpy(&m_Info.Pixel[i * LineSize], &m_Info.Pixel[(m_Info.Height - 1 - i) * LineSize], LineSize);
			memcpy(&m_Info.Pixel[(m_Info.Height - 1 - i) * LineSize], Line, LineSize);
		}
		SAFE_DELETE_ARRAY(Line);
	}

	SetWorldScale((float)m_Info.Width, (float)m_Info.Height, 1.f);
	m_Info.Box.Length.x = m_Info.Width / 2.f;
	m_Info.Box.Length.y = m_Info.Height / 2.f;

	m_Info.Box.Min  = m_Info.Box.Center - m_Info.Box.Length;
	m_Info.Box.Max = m_Info.Box.Center + m_Info.Box.Length;

	m_Min.x = m_Info.Box.Min.x;
	m_Min.y = m_Info.Box.Min.y;

	m_Max.x = m_Info.Box.Max.x;
	m_Max.y = m_Info.Box.Max.y;

	if (FAILED(CreateShaderResourceView(CDevice::GetInst()->GetDevice(), 
		Image.GetImages(),Image.GetImageCount(), 
		Image.GetMetadata(), &m_Info.SRV)))
	{
		return;
	}
}

void CColliderPixel::SetInfoMultibyte(const char* FileName, const std::string& PathName)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	char FullPath[MAX_PATH] = {};

	if (Path)
		strcpy_s(FullPath, Path->PathMultibyte);

	strcat_s(FullPath, FileName);

	return SetInfoMultibyteFullPath(FullPath);
}

void CColliderPixel::SetInfoMultibyteFullPath(const char* FullPath)
{
	TCHAR ConvertFullPath[MAX_PATH] = {};

	int ConvertLength = MultiByteToWideChar(CP_ACP, 0, FullPath, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, FullPath, -1, ConvertFullPath, ConvertLength);

	return SetInfoFullPath(ConvertFullPath);
}

void CColliderPixel::SetInfoCollisionType(PixelCollision_Type Type)
{
	m_Info.Type = Type;
}

void CColliderPixel::SetPixelColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_Info.Color[0] = r;
	m_Info.Color[1] = g;
	m_Info.Color[2] = b;
}

void CColliderPixel::SetPixelAlpha(unsigned char a)
{
	m_Info.Color[3] = a;
}

void CColliderPixel::SetPixelCollisionType(PixelCollision_Type Type)
{
	m_Info.Type = Type;
}

void CColliderPixel::Start()
{
	CColliderComponent::Start();

	SetWorldScale((float)m_Info.Width, (float)m_Info.Height, 1.f);
}

bool CColliderPixel::Init()
{
	if (!CColliderComponent::Init())
		return false;

	m_Info.Box.Axis[0] = Vector2(1.f, 0.f);
	m_Info.Box.Axis[1] = Vector2(0.f, 1.f);

	m_Mesh = m_Scene->GetResource()->FindMesh("Box2D");
	m_PixelMesh = m_Scene->GetResource()->FindMesh("SpriteMesh");
	m_PixelShader = m_Scene->GetResource()->FindShader("ColliderPixelShader");

	SetWorldScale((float)m_Info.Width, (float)m_Info.Height, 1.f);

	return true;
}

void CColliderPixel::Update(float DeltaTime)
{
	CColliderComponent::Update(DeltaTime);
}

void CColliderPixel::PostUpdate(float DeltaTime)
{
	CColliderComponent::PostUpdate(DeltaTime);

	m_Info.Box.Center.x = GetWorldPos().x + m_Offset.x;
	m_Info.Box.Center.y = GetWorldPos().y + m_Offset.y;

	m_Min.x = m_Info.Box.Center.x - m_Info.Box.Length.x;
	m_Min.y = m_Info.Box.Center.y - m_Info.Box.Length.y;

	m_Max.x = m_Info.Box.Center.x + m_Info.Box.Length.x;
	m_Max.y = m_Info.Box.Center.y + m_Info.Box.Length.y;

	m_Info.Min.x = m_Min.x;
	m_Info.Min.y = m_Min.y;

	m_Info.Max.x = m_Max.x;
	m_Info.Max.y = m_Max.y;
}

void CColliderPixel::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderPixel::Render()
{
	CColliderComponent::Render();

	Matrix matView, matProj, matWorld, matWVP;

	CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	matView = Camera->GetViewMatrix();
	matProj  = Camera->GetProjMatrix();

	// PixelMesh�� ���, Rotation�� ó�������� ���� ���̴�.
	Matrix matScale, matTrans;
	matScale.Scaling((float)m_Info.Width, (float)m_Info.Height, 1.f);
	matTrans.Translation(m_Info.Box.Center);

	matWorld = matScale * matTrans;

	matWVP = matWorld * matView * matProj;

	matWVP.Transpose();

	m_CBuffer->SetWVP(matWVP);

	if (m_PrevCollisionList.empty())
		m_CBuffer->SetColliderColor(Vector4(0.f, 1.f, 0.f, 1.f));
	else
		m_CBuffer->SetColliderColor(Vector4(1.f, 0.f, 0.f, 1.f));

	if (m_MouseCollision)
		m_CBuffer->SetColliderColor(Vector4(1.f, 0.f, 0.f, 1.f));

	m_CBuffer->UpdateCBuffer();

	// Pixel
	CDevice::GetInst()->GetContext()->PSSetShaderResources(0, 1, &m_Info.SRV);

	m_PixelShader->SetShader();

	m_PixelMesh->Render();

	// Box 
	m_Shader->SetShader();

	m_Mesh->Render();


}

void CColliderPixel::PostRender()
{
	CColliderComponent::PostRender();
}

CColliderPixel* CColliderPixel::Clone()
{
	return new CColliderPixel(*this);
}

void CColliderPixel::Save(FILE* pFile)
{
	fwrite(&m_Info, sizeof(PixelInfo), 1, pFile);
	CColliderComponent::Save(pFile);
}

void CColliderPixel::Load(FILE* pFile)
{
	fread(&m_Info, sizeof(PixelInfo), 1, pFile);
	CColliderComponent::Load(pFile);
}

bool CColliderPixel::Collision(CColliderComponent* Dest)
{
	switch (Dest->GetColliderType())
	{
	case Collider_Type::Box_2D :
		return CCollision::CollisionBox2DToPixel((CColliderBox2D*)Dest, this);
	case Collider_Type::Circle :
		return CCollision::CollisionCircleToPixel((CColliderCircle*)Dest, this);
	}
	return false;
}

bool CColliderPixel::CollisionMouse(const Vector2& MousePos)
{
	CollisionResult result;
	m_MouseCollision = CCollision::CollisionPixelToPoint(m_MouseResult,  result, m_Info, MousePos);
	if (m_MouseCollision)
	{
		return true;
	}
	return m_MouseCollision;
}
