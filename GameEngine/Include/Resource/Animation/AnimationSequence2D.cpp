#include "AnimationSequence2D.h"
#include "../ResourceManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../../PathManager.h"

CAnimationSequence2D::CAnimationSequence2D() :
	m_Scene(nullptr),
	m_vecFrameData{}
{
}

CAnimationSequence2D::~CAnimationSequence2D()
{
}

bool CAnimationSequence2D::Init(CTexture* Texture)
{
	m_Texture = Texture;

	return true;
}

bool CAnimationSequence2D::Init(const std::string& Name,
                                const TCHAR*       FileName, const std::string& PathName)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
			return false;

		m_Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			return false;

		m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	return true;
}

void CAnimationSequence2D::AddFrame(const Vector2& StartPos, const Vector2& Size)
{
	AnimationFrameData Data = {};

	Data.Start = StartPos;
	Data.Size  = Size;

	m_vecFrameData.push_back(Data);
}

void CAnimationSequence2D::AddFrame(float StartX, float StartY, float Width, float Height)
{
	AnimationFrameData Data = {};

	Data.Start = Vector2(StartX, StartY);
	Data.Size  = Vector2(Width, Height);

	m_vecFrameData.push_back(Data);
}

void CAnimationSequence2D::SetFrame(int Index, float StartX, float StartY, float SizeX, float SizeY)
{
	m_vecFrameData[Index].Start = Vector2(StartX, StartY);
	m_vecFrameData[Index].Size  = Vector2(SizeX, SizeY);
}

void CAnimationSequence2D::SetFrame(int Index, const Vector2& StartPos, Vector2& EndPos)
{
	m_vecFrameData[Index].Start = StartPos;
	m_vecFrameData[Index].Size  = EndPos;
}

void CAnimationSequence2D::SaveFullPath(const char* FullPath)
{
	FILE* pFile;
	fopen_s(&pFile, FullPath, "wb");

	if (!pFile)
		return;

	int Length = static_cast<int>(m_Name.length());
	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), Length, pFile);

	bool TexEnable = false;
	if (m_Texture)
		TexEnable = true;

	fwrite(&TexEnable, sizeof(bool), 1, pFile);
	if (m_Texture)
		m_Texture->Save(pFile);

	int FrameCount = static_cast<int>(m_vecFrameData.size());
	fwrite(&FrameCount, sizeof(int), 1, pFile);
	if (FrameCount > 0)
	{
		fwrite(&m_vecFrameData[0], sizeof(AnimationFrameData), FrameCount, pFile);
	}
	fclose(pFile);
}

void CAnimationSequence2D::Save(FILE* pFile)
{
	int Length = (int)(m_Name.length());
	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), Length, pFile);

	bool TexEnable = false;
	if (m_Texture)
		TexEnable = true;
	fwrite(&TexEnable, sizeof(bool), 1, pFile);

	if (m_Texture)
		m_Texture->Save(pFile);

	// FrameCount 정보 세팅
	int FrameCount = (int)m_vecFrameData.size();
	fwrite(&FrameCount, sizeof(int), 1, pFile);
	if (FrameCount > 0)
	{
		fwrite(&m_vecFrameData[0], sizeof(AnimationFrameData), FrameCount, pFile);
	}
}


bool CAnimationSequence2D::Load(FILE* pFile)
{
	int Sequence2DNameLength = -1;
	fread(&Sequence2DNameLength, sizeof(int), 1, pFile);

	char Sequence2DName[MAX_PATH] = {};
	fread(Sequence2DName, sizeof(char), Sequence2DNameLength, pFile);
	m_Name = Sequence2DName;

	bool Sequence2DTexEnable = false;
	fread(&Sequence2DTexEnable, sizeof(bool), 1, pFile);

	// ----- Texture
	if (Sequence2DTexEnable)
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
					m_Scene->GetResource()->LoadTexture(TextureName, vecFileName[0].c_str(), PathName);
				}
				else
				{
					CResourceManager::GetInst()->LoadTexture(TextureName, vecFileName[0].c_str(), PathName);
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
			m_Texture = m_Scene->GetResource()->FindTexture(TextureName);
		}
		else
		{
			m_Texture = CResourceManager::GetInst()->FindTexture(TextureName);
		}

		m_Texture->SetImageType(ImageType);
	}

	// FrameCount
	int FrameCount = -1;
	fread(&FrameCount, sizeof(int), 1, pFile);
	if (FrameCount > 0)
	{
		m_vecFrameData.resize((const size_t)FrameCount) ;
		fread(&m_vecFrameData[0], sizeof(AnimationFrameData), FrameCount, pFile);
	}
	else
	{
		m_vecFrameData = {};
	}

	return true;
}


bool CAnimationSequence2D::LoadFullPath(const char* FullPath)
{
	FILE* pFile;

	fopen_s(&pFile, FullPath, "rb");

	if (!pFile)
		return false;

	int  Length = -1;
	char Name[MAX_PATH] = {};

	fread(&Length, sizeof(int), 1, pFile);
	fread(Name, sizeof(char), Length, pFile);
	m_Name = Name;

	bool TexEnable = false;
	fread(&TexEnable, sizeof(bool), 1, pFile);

	if (TexEnable)
	{
		int  TextureLength         = -1;
		fread(&TextureLength, sizeof(int), 1, pFile);
		
		char TextureName[MAX_PATH] = {};
		fread(TextureName, sizeof(char), TextureLength, pFile);

		Image_Type Type;
		fread(&Type, sizeof(Image_Type), 1, pFile);

		int InfoCount = -1;
		fread(&InfoCount, sizeof(int), 1, pFile);

		std::vector<std::wstring> vecFullPath;
		std::vector<std::wstring> vecFileName;
		std::string               PathName;

		for (int i = 0; i < InfoCount; i++)
		{
			int PathLength = -1;
			fread(&PathLength, sizeof(int), 1, pFile);

			TCHAR FullPath[MAX_PATH] = {};
			fread(FullPath, sizeof(TCHAR), PathLength, pFile);
			vecFullPath.push_back(FullPath);

			fread(&PathLength, sizeof(int), 1, pFile);
			char Path[MAX_PATH] = {};
			fread(Path, sizeof(char), PathLength, pFile);
			PathName = Path;

			fread(&PathLength, sizeof(int), 1, pFile);
			TCHAR FileName[MAX_PATH] = {};
			fread(FileName, sizeof(TCHAR), PathLength, pFile);
			vecFileName.push_back(FileName);
		}

		switch (Type)
		{
		case Image_Type::Atlas:
			if (vecFileName.size() == 1)
			{
				if (m_Scene)
					m_Scene->GetResource()->LoadTexture(TextureName, vecFileName[0].c_str(), PathName);
				else
					CResourceManager::GetInst()->LoadTexture(TextureName, vecFileName[0].c_str(), PathName);
			}
			break;
		case Image_Type::Frame:
			break;
		case Image_Type::Array:
			break;
		}

		if (m_Scene)
		{
			m_Texture = m_Scene->GetResource()->FindTexture(TextureName);
		}
		else
		{
			m_Texture = CResourceManager::GetInst()->FindTexture(TextureName);
		}
	}

	int FrameCount = 0;
	fread(&FrameCount, sizeof(int), 1, pFile);
	if (FrameCount >0)
	{
		m_vecFrameData.resize(static_cast<const size_t>(FrameCount));
		fread(&m_vecFrameData[0], sizeof(AnimationFrameData), FrameCount, pFile);
	}
	else
	{
		// 초기화는 진행해주어야 한다
		m_vecFrameData = {};
	}
	fclose(pFile);

	return true;
}

bool CAnimationSequence2D::Load(const char* FileName, const std::string& PathName)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	char FileFullPath[MAX_PATH] = {};

	if (Path)
		strcpy_s(FileFullPath, Path->PathMultibyte);
	strcat_s(FileFullPath, FileName);

	// 파일 존재 x
	if (_access(FileFullPath, 0) == -1)
		return false;

	return LoadFullPath(FileFullPath);
}
