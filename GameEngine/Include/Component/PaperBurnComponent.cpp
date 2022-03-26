
#include "PaperBurnComponent.h"
#include "../Resource/Material/Material.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"

CPaperBurnComponent::CPaperBurnComponent()
{
	SetTypeID<CPaperBurnComponent>();

	m_CBuffer = new CPaperBurnConstantBuffer;

	m_CBuffer->Init();

	m_FinishTime = 6.f;

	m_StartPaperBurn = false;

	m_Filter = 0.f;
}

CPaperBurnComponent::CPaperBurnComponent(const CPaperBurnComponent& com) :
	CObjectComponent(com)
{
	m_CBuffer = com.m_CBuffer->Clone();

	m_FinishTime = com.m_FinishTime;
	m_Material = com.m_Material;
	m_StartPaperBurn = false;
	m_Filter = 0.f;
}

CPaperBurnComponent::~CPaperBurnComponent()
{
	SAFE_DELETE(m_CBuffer);

	if (m_Material)
		m_Material->DeleteRenderCallback(this);
}

void CPaperBurnComponent::StartPaperBurn()
{
	if (!m_Material || m_StartPaperBurn)
		return;

	m_Material->SetPaperBurn(true);
	m_StartPaperBurn = true;
}

void CPaperBurnComponent::SetMaterial(CMaterial* Material)
{
	m_Material = Material;

	m_Material->AddRenderCallback(this, &CPaperBurnComponent::SetShader);
}

void CPaperBurnComponent::SetInverse(bool Enable)
{
	m_CBuffer->SetInverse(Enable);
}

void CPaperBurnComponent::SetBurnTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
		return;

	m_BurnTexture = m_Scene->GetResource()->FindTexture(Name);
}

void CPaperBurnComponent::SetOutLineColor(const Vector4& Color)
{
	m_CBuffer->SetOutLineColor(Color);
}

void CPaperBurnComponent::SetOutLineColor(float r, float g, float b, float a)
{
	m_CBuffer->SetOutLineColor(r, g, b, a);
}

void CPaperBurnComponent::SetInLineColor(const Vector4& Color)
{
	m_CBuffer->SetInLineColor(Color);
}

void CPaperBurnComponent::SetInLineColor(float r, float g, float b, float a)
{
	m_CBuffer->SetInLineColor(r, g, b, a);
}

void CPaperBurnComponent::SetCenterLineColor(const Vector4& Color)
{
	m_CBuffer->SetCenterLineColor(Color);
}

void CPaperBurnComponent::SetCenterLineColor(float r, float g, float b, float a)
{
	m_CBuffer->SetCenterLineColor(r, g, b, a);
}

void CPaperBurnComponent::SetInFilter(float Filter)
{
	m_CBuffer->SetInFilter(Filter);
}

void CPaperBurnComponent::SetOutFilter(float Filter)
{
	m_CBuffer->SetOutFilter(Filter);
}

void CPaperBurnComponent::SetCenterFilter(float Filter)
{
	m_CBuffer->SetCenterFilter(Filter);
}

void CPaperBurnComponent::Start()
{
	CObjectComponent::Start();
}

bool CPaperBurnComponent::Init()
{
	SetBurnTexture("DefaultBurnTexture", TEXT("DefaultPaperBurn.png"));

	return true;
}

void CPaperBurnComponent::Update(float DeltaTime)
{
	if (m_StartPaperBurn)
	{
		m_Filter += DeltaTime / m_FinishTime;

		if (m_Filter >= 1.f)
		{
			m_Filter = 1.f;

			if (m_FinishCallback)
				m_FinishCallback();
		}

		m_CBuffer->SetFilter(m_Filter);
	}
}

void CPaperBurnComponent::PostUpdate(float DeltaTime)
{
}

void CPaperBurnComponent::PrevRender()
{
}

void CPaperBurnComponent::Render()
{
}

void CPaperBurnComponent::PostRender()
{
}

CPaperBurnComponent* CPaperBurnComponent::Clone()
{
	return new CPaperBurnComponent(*this);
}

void CPaperBurnComponent::Save(FILE* pFile)
{
	CObjectComponent::Save(pFile);

	m_Material->Save(pFile);

	bool BurnTexEnable = false;
	if (m_BurnTexture)
		BurnTexEnable = true;

	fwrite(&BurnTexEnable, sizeof(bool), 1, pFile);

	if (m_BurnTexture)
		m_BurnTexture->Save(pFile);

}

void CPaperBurnComponent::Load(FILE* pFile)
{
	CObjectComponent::Load(pFile);

	// Material
	m_Material = m_Scene->GetResource()->CreateMaterialEmpty<CMaterial>();
	m_Material->Load(pFile);

	// Texture
	bool BurnTexEnable = false;

	fread(&BurnTexEnable, sizeof(bool), 1, pFile);

	if (BurnTexEnable)
	{
		int TextureNameLength = -1;
		char TextureName[MAX_PATH] = {};
		fread(&TextureNameLength, sizeof(int), 1, pFile);
		fread(TextureName, sizeof(char), TextureNameLength, pFile);

		Image_Type ImageType = Image_Type::Atlas;
		fread(&ImageType, sizeof(Image_Type), 1, pFile);

		int InfoCount = -1;
		fread(&InfoCount, sizeof(int), 1, pFile);

		std::vector<std::wstring> vecFullPath;
		std::vector<std::wstring> vecFileName;
		std::string PathName;

		for (int i = 0; i < InfoCount; i++)
		{
			int PathLength = -1;
			TCHAR FullPath[MAX_PATH] = {};
			fread(&PathLength, sizeof(int), 1, pFile);
			fread(FullPath, sizeof(TCHAR), PathLength, pFile);
			vecFullPath.push_back(FullPath);

			char Path[MAX_PATH] = {};
			fread(&PathLength, sizeof(int), 1, pFile);
			fread(Path, sizeof(char), PathLength, pFile);
			PathName = Path;

			TCHAR FileName[MAX_PATH] = {};
			fread(&PathLength, sizeof(int), 1, pFile);
			fread(FileName, sizeof(TCHAR), PathLength, pFile);
			vecFileName.push_back(FileName);
		}

		switch (ImageType)
		{
		case Image_Type::Atlas:
		{
			if (vecFileName.size() == 1)
			{
				if (m_Scene)
				{
					m_Scene->GetResource()->LoadTextureFullPath(TextureName, vecFullPath[0].c_str());
				}
				else
				{
					CResourceManager::GetInst()->LoadTextureFullPath(TextureName, vecFullPath[0].c_str());
				}
			}
		}
		break;
		case Image_Type::Frame:
			break;
		case Image_Type::Array:
			break;
		}
		if (m_Scene)
		{
			m_BurnTexture = m_Scene->GetResource()->FindTexture(TextureName);
		}
		else
		{
			m_BurnTexture = CResourceManager::GetInst()->FindTexture(TextureName);
		}

		if (!m_BurnTexture)
			return ;

		m_BurnTexture->SetImageType(ImageType);
	}

	// CBuffer
	SAFE_DELETE(m_CBuffer);

	m_CBuffer = new CPaperBurnConstantBuffer;

	m_CBuffer->Init();


}

void CPaperBurnComponent::SetShader()
{
	m_CBuffer->UpdateCBuffer();

	m_BurnTexture->SetShader(101, (int)Buffer_Shader_Type::Pixel, 0);
}
