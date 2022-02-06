#include "PathManager.h"

CPathManager* CPathManager::m_Inst = nullptr;

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
	auto iter    = m_mapPath.begin();
	auto iterEnd = m_mapPath.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CPathManager::Init()
{
	// RootPath�� ��θ� ���´�.
	// RootPath�� Bin���� ������ ��ü ��θ� ���Ѵ�.
	TCHAR Path[MAX_PATH] = {};

	// GetModuleFileName : ���� ���������� �ִ� ��� + ���������̸�.exe ��
	// ��ü��θ� ������ �ȴ�.
	// ���⿡�� �������� �̸������� ���� Bin���� ������ ���ͼ�
	// Root������ ����Ѵ�.
	GetModuleFileName(nullptr, Path, MAX_PATH);

	// ���� ���ڿ��� �ڿ������� ���ʷ� �����Ͽ� \�� / �� �ִ� ���ڸ� ã�´�.
	int Length = lstrlen(Path);

	// ROOT Path �����
	// C:\Users\dhsys\Desktop\DX_Project\GameEngine\Bin\~~
	for (int i = Length - 1; i > 0; --i)
	{
		if (Path[i] == '\\')
		{
			memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}

	PathInfo* Info = new PathInfo;

	lstrcpy(Info->Path, Path);

#ifdef UNICODE
	// �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, Path, -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_ACP, 0, Path, -1,
	                    Info->PathMultibyte, ConvertLength, nullptr, nullptr);
#else
    strcpy_s(Info->PathMultibyte, Path);
#endif // UNICODE

	m_mapPath.insert(std::make_pair(ROOT_PATH, Info));

	AddPath(SHADER_PATH, TEXT("Shader\\"));
	AddPath(TEXTURE_PATH, TEXT("Texture\\"));
	AddPath(FONT_PATH, TEXT("Font\\"));
	AddPath(ANIMATION_PATH, TEXT("Animation\\"));
	AddPath(SCENE_PATH, TEXT("Scene\\"));
	AddPath(SOUND_PATH, TEXT("Sound\\"));
	AddPath(SEQUENCE_PATH, TEXT("Sequence\\"));

	// Engine Path �����  ----------------------------------------------
	TCHAR EnginePath[MAX_PATH] = {};
	GetModuleFileName(nullptr, EnginePath, MAX_PATH);

	int EnginePathLength = lstrlen(EnginePath);

	const TCHAR* EngineRootFolder = TEXT("DX_Project");

	int RootFolderLength = (int)lstrlen(EngineRootFolder);

	for (int lth = EnginePathLength - 1; lth >= 0; lth--)
	{
		if (EnginePath[lth] == '\\')
		{
			bool Found = true;

			for (int st = 1; st <= 10; st++)
			{
				const TCHAR RootFolderChar = EngineRootFolder[RootFolderLength - st];
				const TCHAR PathChar = EnginePath[lth - st];

				if (RootFolderChar != PathChar)
				{
					Found = false;
					break;
				}
			}

			if (Found)
			{
				// C:\Users\dhsys\Desktop\DX_Project\GameEngine\Bin\~~ �� �����־�� �Ѵ�.
				// ���� ���� ���� --> Game
				const TCHAR* RestEnginePath = TEXT("GameEngine\\Bin\\");

				int RestLength = (int)lstrlen(RestEnginePath);

				memset(&EnginePath[lth + 1], 0, EnginePathLength - lth + 1);

				for (int i = 0; i <= RestLength; i++)
				{
					const TCHAR AddedChar = RestEnginePath[i];
					EnginePath[lth + 1 + i] = AddedChar;
				}

				break;
			}
		}
	}

	PathInfo* FinalEnginePath = new PathInfo;
	lstrcpy(FinalEnginePath->Path, EnginePath);

#ifdef UNICODE
	int EngineConvertLength = WideCharToMultiByte(CP_ACP, 0, EnginePath, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, EnginePath, -1, FinalEnginePath->PathMultibyte, EngineConvertLength, 0, 0);
#else
	strcpy_s(FinalEnginePath->PathMultibyte, EnginePath);
#endif

	m_mapPath.insert(std::make_pair(ENGINE_ROOT_PATH, FinalEnginePath));

	AddEnginePath(ENGINE_SHADER_PATH, TEXT("Shader\\"));
	AddEnginePath(ENGINE_TEXTURE_PATH, TEXT("Texture\\"));
	AddEnginePath(ENGINE_FONT_PATH, TEXT("Font\\"));
	AddEnginePath(ENGINE_ANIMATION_PATH, TEXT("Animation\\"));
	AddEnginePath(ENGINE_SCENE_PATH, TEXT("Scene\\"));
	AddEnginePath(ENGINE_SOUND_PATH, TEXT("Sound\\"));
	AddEnginePath(ENGINE_SEQUENCE_PATH, TEXT("Sequence\\"));

	return true;
}

bool CPathManager::AddPath(const std::string& Name, const TCHAR* Path,
                           const std::string& BaseName)
{
	if (FindPath(Name))
		return false;

	const PathInfo* BaseInfo = FindPath(BaseName);

	if (!BaseInfo)
		return false;

	PathInfo* Info = new PathInfo;

	lstrcpy(Info->Path, BaseInfo->Path);
	lstrcat(Info->Path, Path);

#ifdef UNICODE
	// �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
	                    Info->PathMultibyte, ConvertLength, nullptr, nullptr);
#else
    strcpy_s(Info->PathMultibyte, Info->Path);
#endif // UNICODE

	m_mapPath.insert(std::make_pair(Name, Info));

	return true;
}

bool CPathManager::AddEnginePath(const std::string& Name, const TCHAR* Path, const std::string& BaseName)
{
	if (FindPath(Name))
		return false;

	const PathInfo* BaseInfo = FindPath(BaseName);

	if (!BaseInfo)
		return false;

	PathInfo* Info = new PathInfo;

	lstrcpy(Info->Path, BaseInfo->Path);
	lstrcat(Info->Path, Path);

#ifdef UNICODE
	// �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
		Info->PathMultibyte, ConvertLength, nullptr, nullptr);
#else
	strcpy_s(Info->PathMultibyte, Info->Path);
#endif // UNICODE

	m_mapPath.insert(std::make_pair(Name, Info));

	return true;
}

const PathInfo* CPathManager::FindPath(const std::string& Name)
{
	std::unordered_map<std::string, PathInfo*>::iterator iter = m_mapPath.find(Name);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second;
}
