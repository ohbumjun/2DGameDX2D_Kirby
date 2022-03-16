#include "AnimationSequence2DInstance.h"
#include "../Component/SpriteComponent.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Resource/Texture/Texture.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"

CAnimationSequence2DInstance::CAnimationSequence2DInstance() :
	m_PlayAnimation(true),
	m_Owner(nullptr),
	m_Scene(nullptr)
{
	SetTypeID<CAnimationSequence2DInstance>();
}

CAnimationSequence2DInstance::CAnimationSequence2DInstance(const CAnimationSequence2DInstance& Anim)
{
	m_PlayAnimation = Anim.m_PlayAnimation;

	auto iter    = Anim.m_mapAnimation.begin();
	auto iterEnd = Anim.m_mapAnimation.end();

	// 깊은 복사 처리를 해줘야 한다.
	for (; iter != iterEnd; ++iter)
	{
		CAnimationSequence2DData* Data = new CAnimationSequence2DData;

		Data->m_Sequence  = iter->second->m_Sequence;
		Data->m_Name      = iter->second->m_Name;
		Data->m_Loop      = iter->second->m_Loop;
		Data->m_PlayTime  = iter->second->m_PlayTime;
		Data->m_PlayScale = iter->second->m_PlayScale;
		Data->m_Reverse   = iter->second->m_Reverse;

		if (Anim.m_CurrentAnimation->m_Name == Data->m_Name)
			m_CurrentAnimation = Data;

		m_mapAnimation.insert(std::make_pair(iter->first, Data));
	}
}

CAnimationSequence2DInstance::~CAnimationSequence2DInstance()
{
	auto iter    = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapAnimation.clear();
}

void CAnimationSequence2DInstance::GatherSequenceNames(std::vector<std::string>& vecNames)
{
	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter  != iterEnd; ++iter)
	{
		vecNames.push_back(iter->second->m_Sequence->GetName());
	}
}

void CAnimationSequence2DInstance::GatherAnimationNames(std::vector<std::string>& vecNames)
{
	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		vecNames.push_back(iter->first);
	}
}

void CAnimationSequence2DInstance::AddSequence2DToScene()
{
	CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		Resource->AddSequence2D(iter->second->GetAnimationSequence());
	}
}

void CAnimationSequence2DInstance::ClearAnimationSequence()
{
	// 현재 Animation은 null로
	m_CurrentAnimation = nullptr;

	// 나머지는 모두 지워주기
	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		// SceneResource 혹은 ResourceManager 에서 해당 AnimationSeq2DData의 Sequence2D를 지워줘야 한다
		if (m_Scene)
		{
			
		}
		else
		{
			
		}

		// SAFE_DELETE(iter->second);
		SAFE_DELETE(iter->second);
	}
	m_mapAnimation.clear();

}

bool CAnimationSequence2DInstance::DeleteAnimationSequence(const std::string& Name)
{
	bool IsDelete = false;

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->second->GetName() == Name)
		{
			SAFE_DELETE(iter->second);
			IsDelete = true;
			iter = m_mapAnimation.erase(iter);
			break;
		}
	}

	return IsDelete;
}

void CAnimationSequence2DInstance::EditSequence2DName(const std::string& PrevName, const std::string& NewName)
{
	auto iter = m_mapAnimation.find(PrevName);

	if (iter == m_mapAnimation.end())
		return;

	CAnimationSequence2DData* Animation = iter->second;

	// 새로운 내용 복사 생성
	CAnimationSequence2DData* NewAnimation = new CAnimationSequence2DData;

	NewAnimation->m_Sequence = Animation->m_Sequence;
	NewAnimation->m_Sequence->SetName(NewName);
	NewAnimation->m_Name = Animation->m_Name;
	NewAnimation->m_Loop = Animation->m_Loop;
	NewAnimation->m_PlayTime = Animation->m_PlayTime;
	NewAnimation->m_PlayScale = Animation->m_PlayScale;
	NewAnimation->m_Reverse = Animation->m_Reverse;
	NewAnimation->m_FrameTime = Animation->m_PlayTime / Animation->m_Sequence->GetFrameCount();

	// 기존 것 지우기
	m_mapAnimation.erase(iter);

	// 새로운 내용 추가
	m_mapAnimation.insert(std::make_pair(NewName, NewAnimation));
}

void CAnimationSequence2DInstance::AdjustSequence2DKeyName()
{
	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	std::vector<std::pair<std::string, std::string>> vecDiffName;

	for (; iter != iterEnd; ++iter)
	{
		if (iter->first != iter->second->m_Sequence->GetName())
		{
			vecDiffName.push_back(std::make_pair(iter->first, iter->second->m_Sequence->GetName()));
		}
	}

	size_t Size = vecDiffName.size();

	for (size_t i = 0; i < Size; i++)
	{
		EditSequence2DName(vecDiffName[i].first, vecDiffName[i].second);
	}
}

void CAnimationSequence2DInstance::SetFrameData(const Vector2& StartPos, const Vector2& Size, int Index)
{
	if (!m_CurrentAnimation)
		return;
	m_CurrentAnimation->SetFrameData(StartPos, Size, Index);
}

void CAnimationSequence2DInstance::AddAnimation(const std::string& AnimationName, CAnimationSequence2DData* Animation)
{
	CAnimationSequence2DData* Anim = FindAnimationSequence2DData(AnimationName);

	if (Anim)
		return;

	if (m_mapAnimation.empty())
	{
		m_CurrentAnimation = Animation;

		if (m_Owner)
		{
			m_Owner->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel,
				Animation->GetAnimationSequence()->GetTexture()->GetName(),
				Animation->GetAnimationSequence()->GetTexture());
		}
	}

	m_mapAnimation.insert(std::make_pair(AnimationName, Animation));
}

void CAnimationSequence2DInstance::AddAnimation(const std::string& AnimationName, 
	const std::string& SequenceName, CAnimationSequence2D* Sequence2D, bool Loop,
	float              PlayTime, float     PlayScale, bool Reverse)
{
	CAnimationSequence2DData* Animation = FindAnimationSequence2DData(AnimationName);

	if (Animation)
		return;

	Animation = new CAnimationSequence2DData;
	Animation->m_Sequence = Sequence2D;
	Animation->m_SequenceName = SequenceName;
	Animation->m_Loop = Loop;
	Animation->m_Reverse = Reverse;
	Animation->m_PlayTime = PlayTime;
	Animation->m_PlayScale = PlayScale;

	if (m_mapAnimation.empty())
	{
		m_CurrentAnimation = Animation;

		if (m_Owner)
		{
			m_Owner->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel,
				SequenceName, Sequence2D->GetTexture());
		}
	}

	m_mapAnimation.insert(std::make_pair(AnimationName, Animation));
}

void CAnimationSequence2DInstance::AddAnimation(const std::string& SequenceName,
												const std::string& AnimationName, bool Loop,
												float              PlayTime, float     PlayScale, bool Reverse)
{
	CAnimationSequence2DData* Anim = FindAnimationSequence2DData(AnimationName);

	if (Anim)
		return;

	CAnimationSequence2D* Sequence = nullptr;

	if (m_Scene)
	{
		Sequence = m_Scene->GetResource()->FindAnimationSequence2D(SequenceName);
	}

	else
	{
		Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);
	}

	if (!Sequence)
		return;

	Anim = new CAnimationSequence2DData;

	// 여기서 Seqeunce 에 대한 Ref Count가 1 증가한다.
	Anim->m_Sequence  = Sequence;
	Anim->m_Name      = AnimationName;
	Anim->m_Loop      = Loop;
	Anim->m_PlayTime  = PlayTime;
	Anim->m_PlayScale = PlayScale;
	Anim->m_Reverse   = Reverse;
	Anim->m_FrameTime = PlayTime / Sequence->GetFrameCount();

	if (m_mapAnimation.empty())
	{
		m_CurrentAnimation = Anim;

		if (m_Owner)
		{
			m_Owner->SetTexture(0, 0, (int)(Buffer_Shader_Type::Pixel),
			                    Anim->m_Sequence->GetTexture()->GetName(),
			                    Anim->m_Sequence->GetTexture());
		}
	}

	m_mapAnimation.insert(std::make_pair(AnimationName, Anim));
}

void CAnimationSequence2DInstance::AddAnimation(const TCHAR* FileName, const std::string& PathName,
	const std::string& AnimName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	CAnimationSequence2DData* Anim = FindAnimationSequence2DData(AnimName);

	if (Anim)
		return;

	char FileNameMultibyte[MAX_PATH] = {};
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FileName, -1, FileNameMultibyte, ConvertLength, 0, 0);

	CAnimationSequence2D* Sequence = nullptr;

	if (m_Scene)
	{
		std::string  SequenceName;
		m_Scene->GetResource()->LoadSequence2D(SequenceName, FileNameMultibyte, PathName);
		Sequence = m_Scene->GetResource()->FindAnimationSequence2D(SequenceName);
	}
	else
	{
		std::string  SequenceName;
		CResourceManager::GetInst()->LoadSequence2D(SequenceName, FileNameMultibyte, PathName);
		Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);
	}

	if (!Sequence)
		return;

	Anim = new CAnimationSequence2DData;
	Anim->m_Sequence = Sequence;
	Anim->m_Name      = AnimName;
	Anim->m_Loop        = Loop;
	Anim->m_Reverse   = Reverse;
	Anim->m_PlayScale = PlayScale;
	Anim->m_PlayTime  = PlayTime;
	Anim->m_FrameTime = PlayTime / Sequence->GetFrameCount();

	if (m_mapAnimation.empty())
	{
		m_CurrentAnimation = Anim;
		if (m_Owner)
		{
			m_Owner->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel,
				Anim->m_Sequence->GetTexture()->GetName(), Anim->m_Sequence->GetTexture());
		}
	}
	m_mapAnimation.insert(std::make_pair(AnimName, Anim));
}

void CAnimationSequence2DInstance::SetPlayTime(const std::string& Name, float PlayTime)
{
	CAnimationSequence2DData* Anim = FindAnimationSequence2DData(Name);

	if (!Anim)
		return;

	Anim->m_PlayTime = PlayTime;
}

void CAnimationSequence2DInstance::SetPlayScale(const std::string& Name, float PlayScale)
{
	CAnimationSequence2DData* Anim = FindAnimationSequence2DData(Name);

	if (!Anim)
		return;

	Anim->m_PlayScale = PlayScale;
}

void CAnimationSequence2DInstance::SetReverse(const std::string& Name, bool Reverse)
{
	CAnimationSequence2DData* Anim = FindAnimationSequence2DData(Name);

	if (!Anim)
		return;

	Anim->m_Reverse = Reverse;
}

void CAnimationSequence2DInstance::SetLoop(const std::string& Name, bool Loop)
{
	CAnimationSequence2DData* Anim = FindAnimationSequence2DData(Name);

	if (!Anim)
		return;

	Anim->m_Loop = Loop;
}

void CAnimationSequence2DInstance::SetCurrentAnimation(const std::string& Name)
{
	CAnimationSequence2DData* FoundAnimation = FindAnimationSequence2DData(Name);

	if (!FoundAnimation)
		return;

	m_CurrentAnimation = FoundAnimation;

	m_CurrentAnimation->m_Frame = 0;
	m_CurrentAnimation->m_Time  = 0.f;

	size_t Size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurrentAnimation->m_vecNotify[i]->Call = false;
	}

	if (m_Owner)
	{
		m_Owner->SetTexture(0, 0, static_cast<int>(Buffer_Shader_Type::Pixel),
		                    m_CurrentAnimation->m_Sequence->GetTexture()->GetName(),
		                    m_CurrentAnimation->m_Sequence->GetTexture());
	}
}

void CAnimationSequence2DInstance::ChangeAnimation(const std::string& Name)
{
	if (m_CurrentAnimation->m_Name == Name)
		return;

	if (!FindAnimationSequence2DData(Name))
		return;

	m_CurrentAnimation->m_Frame = 0;
	m_CurrentAnimation->m_Time  = 0.f;

	size_t Size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurrentAnimation->m_vecNotify[i]->Call = false;
	}

	m_CurrentAnimation          = FindAnimationSequence2DData(Name);
	m_CurrentAnimation->m_Frame = 0;
	m_CurrentAnimation->m_Time  = 0.f;

	if (m_Owner)
	{
		m_Owner->SetTexture(0, 0, static_cast<int>(Buffer_Shader_Type::Pixel),
		                    m_CurrentAnimation->m_Sequence->GetTexture()->GetName(),
		                    m_CurrentAnimation->m_Sequence->GetTexture());
	}
}

bool CAnimationSequence2DInstance::CheckCurrentAnimation(const std::string& Name)
{
	return m_CurrentAnimation->m_Name == Name;
}

CAnimationSequence2DData* CAnimationSequence2DInstance::GetCurrentAnimation() const
{
	if (m_CurrentAnimation)
		return m_CurrentAnimation;
	return nullptr;
}

void CAnimationSequence2DInstance::SetPlayScaleOfAllAnimation(float PlayScale)
{
	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter).second->SetPlayScale(PlayScale);
	}
}

int CAnimationSequence2DInstance::GetCurrentAnimationOrder()
{
	if (!m_CurrentAnimation)
		return -1;

	int Idx = 0;

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();
	for (; iter != iterEnd; ++iter)
	{
		if (iter->second == m_CurrentAnimation)
			break;
		Idx += 1;
	}

	return Idx;
}

bool CAnimationSequence2DInstance::SaveFullPath(const char* FullPath)
{
	FILE* pFile;
	fopen_s(&pFile, FullPath, "wb");
	if (!pFile)
		return false;

	/*
class CSpriteComponent*                                    m_Owner;
class CScene*                                              m_Scene;
std::unordered_map<std::string, CAnimationSequence2DData*> m_mapAnimation;
CAnimationSequence2DData*                                  m_CurrentAnimation;
class CAnimation2DConstantBuffer*                          m_CBuffer;
 */

	fwrite(&m_TypeID, sizeof(size_t), 1, pFile);
	fwrite(&m_PlayAnimation, sizeof(bool), 1, pFile);

	int Length = (int)m_Name.length();
	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), Length, pFile);

	size_t AnimationSize = m_mapAnimation.size();
	fwrite(&AnimationSize, sizeof(size_t), 1, pFile);

	auto iter                       = m_mapAnimation.begin();
	auto iterEnd                    = m_mapAnimation.end();
	size_t  SequenceDataNameLength     = -1;
	for (; iter != iterEnd; ++iter)
	{
		SequenceDataNameLength = strlen(iter->first.c_str());
		fwrite(&SequenceDataNameLength, sizeof(int), 1, pFile);
		fwrite(iter->first.c_str(), sizeof(char), SequenceDataNameLength, pFile);
		iter->second->Save(pFile);
	}

	// Current Animation
	bool CurrentAnimEnable = false;
	if (m_CurrentAnimation)
		CurrentAnimEnable = true;
	fwrite(&CurrentAnimEnable, sizeof(bool), 1, pFile);

	if (m_CurrentAnimation)
	{
		// Current Anim Length
		Length = (int)m_CurrentAnimation->m_Name.length();
		fwrite(&Length, sizeof(int), 1, pFile);
		fwrite(m_CurrentAnimation->m_Name.c_str(), sizeof(char), Length, pFile);
	}

	fclose(pFile);

	return true;
}

bool CAnimationSequence2DInstance::SaveAllSequencesFullPath(const char* FullPath)
{
	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		char SequenceFullPath[MAX_PATH] = {};
		strcpy_s(SequenceFullPath, FullPath);
		strcat_s(SequenceFullPath, iter->second->GetAnimationSequence()->GetName().c_str());
		strcat_s(SequenceFullPath, ".sqc");

		iter->second->GetAnimationSequence()->SaveFullPath(SequenceFullPath);
	}

	return true;
}

bool CAnimationSequence2DInstance::LoadFullPath(const char* FullPath)
{
	FILE* pFile;

	fopen_s(&pFile, FullPath, "rb");

	if (!pFile)
		return false;

	fread(&m_TypeID, sizeof(size_t), 1, pFile);
	fread(&m_PlayAnimation, sizeof(bool), 1, pFile);

	int Length = 0;
	fread(&Length, sizeof(int), 1, pFile);
	char Name[MAX_PATH] = {};
	fread(Name, sizeof(char), Length, pFile);
	m_Name = Name;

	size_t AnimationSize = -1;
	fread(&AnimationSize, sizeof(size_t), 1, pFile);


	// m_mapAnimation.clear();
	// m_CurrentAnimation = nullptr;

	for (int i = 0; i < AnimationSize; i++)
	{
		int SequenceDataKeyNameLength = 0;
		char SequenceData2DNameKey[MAX_PATH] = {};

		// - CAnimationSequence2DData
		// Key Name 저장 
		fread(&SequenceDataKeyNameLength, sizeof(int), 1, pFile);
		fread(SequenceData2DNameKey, sizeof(char), SequenceDataKeyNameLength, pFile);

		// CAnimationSequence2DData 를 저장하기 ===============================
		CAnimationSequence2DData* Sequence2DData = new CAnimationSequence2DData;

		if (!Sequence2DData->Load(pFile))
			return false;

		// Scene이 있냐 없냐에 따라
		// Scene Resource, ResourceManager에 해당 Sequence를 추가해주어야 한다
		/*
		if (m_Scene)
		{
			m_Scene->GetResource()->AddSequence2D(Sequence2DData->m_Sequence);
		}
		else
		{
			CResourceManager::GetInst()->AddSequence2D(Sequence2DData->m_Sequence);
		}
		*/

		// 중복 추가 방지
		if (FindAnimationSequence2DData(SequenceData2DNameKey))
		{
			SAFE_DELETE(Sequence2DData);
			continue;
		}

		if (m_mapAnimation.empty())
		{
			m_CurrentAnimation = Sequence2DData;
		}

		m_mapAnimation.insert(std::make_pair(SequenceData2DNameKey, Sequence2DData));

	}

	// Current Animation Info
	bool CurrentAnimEnable = false;
	fread(&CurrentAnimEnable, sizeof(bool), 1, pFile);

	if (CurrentAnimEnable)
	{
		int CurAnimNameLength = -1;
		fread(&CurAnimNameLength, sizeof(int), 1, pFile);
		char CurAnimName[MAX_PATH] = {};
		fread(CurAnimName, sizeof(char), CurAnimNameLength, pFile);

		CAnimationSequence2DData* FoundAnimation = FindAnimationSequence2DData(CurAnimName);

		// 이름 자체가 잘못 저장된 것일 수도 있어서 해당 내용이 없을 수도 있다.
		if (FoundAnimation)
		{
			m_CurrentAnimation = FoundAnimation;
		}
	}

	fclose(pFile);

	return true;
}

void CAnimationSequence2DInstance::Start()
{
	if (m_Owner && m_CurrentAnimation)
	{
		m_Owner->SetTexture(0, 0, static_cast<int>(Buffer_Shader_Type::Pixel),
		                    m_CurrentAnimation->m_Sequence->GetTexture()->GetName(),
		                    m_CurrentAnimation->m_Sequence->GetTexture());
	}
}

bool CAnimationSequence2DInstance::Init()
{
	if (m_Scene && !m_CBuffer)
		m_CBuffer = m_Scene->GetResource()->GetAnimation2DCBuffer();

	return true;
}

void CAnimationSequence2DInstance::Update(float DeltaTime)
{
	if (!m_PlayAnimation)
		return;
	if (!m_CurrentAnimation)
		return;
	if (m_CurrentAnimation->GetFrameCount() <= 0)
		return;

	if (m_CurrentAnimation->GetInitPauseTime() > 0.f)
	{
		m_CurrentAnimation->SetInitPauseTime(m_CurrentAnimation->GetInitPauseTime() - DeltaTime);

		return;
	}

	// FrameTime Update
	m_CurrentAnimation->m_FrameTime = m_CurrentAnimation->m_PlayTime / m_CurrentAnimation->GetFrameCount();

	m_CurrentAnimation->m_Time += DeltaTime * m_CurrentAnimation->m_PlayScale;

	bool AnimEnd = false;

	if (m_CurrentAnimation->m_Time >= m_CurrentAnimation->m_FrameTime)
	{
		m_CurrentAnimation->m_Time -= m_CurrentAnimation->m_FrameTime;

		if (m_CurrentAnimation->m_Reverse)
		{
			--m_CurrentAnimation->m_Frame;

			if (m_CurrentAnimation->m_Frame < 0)
				AnimEnd = true;
		}

		else
		{
			++m_CurrentAnimation->m_Frame;

			if (m_CurrentAnimation->m_Frame == m_CurrentAnimation->m_Sequence->GetFrameCount())
				AnimEnd = true;
		}
	}

	// 호출해야 하는 Notify가 있는지 판단한다.
	size_t Size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!m_CurrentAnimation->m_vecNotify[i]->Call &&
			m_CurrentAnimation->m_vecNotify[i]->Frame == m_CurrentAnimation->m_Frame)
		{
			m_CurrentAnimation->m_vecNotify[i]->Call = true;
			m_CurrentAnimation->m_vecNotify[i]->Function();
		}
	}

	if (AnimEnd)
	{
		if (m_CurrentAnimation->m_Loop)
		{
			if (m_CurrentAnimation->m_Reverse)
				m_CurrentAnimation->m_Frame = m_CurrentAnimation->m_Sequence->GetFrameCount() - 1;

			else
				m_CurrentAnimation->m_Frame = 0;
		}

		else
		{
			if (m_CurrentAnimation->m_Reverse)
				m_CurrentAnimation->m_Frame = 0;

			else
				m_CurrentAnimation->m_Frame = m_CurrentAnimation->m_Sequence->GetFrameCount() - 1;
		}

		if (m_CurrentAnimation->m_EndFunction)
			m_CurrentAnimation->m_EndFunction();

		if (m_CurrentAnimation->m_Loop)
		{
			size_t Size = m_CurrentAnimation->m_vecNotify.size();

			for (size_t i = 0; i < Size; ++i)
			{
				m_CurrentAnimation->m_vecNotify[i]->Call = false;
			}
		}
	}
}

void CAnimationSequence2DInstance::SetShader()
{
	if (!m_CurrentAnimation)
		return;

	if (m_CurrentAnimation->m_Sequence->GetFrameCount() <= 0)
		return;

	Vector2 StartUV, EndUV;

	Vector2 Start     = m_CurrentAnimation->m_Sequence->GetFrameData(m_CurrentAnimation->m_Frame).Start;
	Vector2 FrameSize = m_CurrentAnimation->m_Sequence->GetFrameData(m_CurrentAnimation->m_Frame).Size;

	StartUV = Start /
		Vector2(static_cast<float>(m_CurrentAnimation->m_Sequence->GetTexture()->GetWidth()),
		        static_cast<float>(m_CurrentAnimation->m_Sequence->GetTexture()->GetHeight()));

	EndUV = (Start + FrameSize) /
		Vector2(static_cast<float>(m_CurrentAnimation->m_Sequence->GetTexture()->GetWidth()),
		        static_cast<float>(m_CurrentAnimation->m_Sequence->GetTexture()->GetHeight()));

	m_CBuffer->SetAnimation2DType(m_CurrentAnimation->m_Sequence->GetTexture()->GetImageType());
	m_CBuffer->SetStartUV(StartUV);
	m_CBuffer->SetEndUV(EndUV);

	m_CBuffer->UpdateCBuffer();
}

void CAnimationSequence2DInstance::ResetShader()
{
}

CAnimationSequence2DInstance* CAnimationSequence2DInstance::Clone()
{
	return new CAnimationSequence2DInstance(*this);
}

void CAnimationSequence2DInstance::Save(FILE* pFile)
{
	/*
	class CSpriteComponent* m_Owner;
	class CScene* m_Scene; --> Load 하면서 세팅되기 때문에 이렇게 해놓은 것인가 ? 
	std::unordered_map<std::string, CAnimationSequence2DData*> m_mapAnimation;
	CAnimationSequence2DData* m_CurrentAnimation;
	class CAnimation2DConstantBuffer* m_CBuffer;
	*/

	fwrite(&m_PlayAnimation, sizeof(bool), 1, pFile);

	int AnimCount = (int)m_mapAnimation.size();
	fwrite(&AnimCount, sizeof(int), 1, pFile);

	auto iter      = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	
	for (;iter != iterEnd; ++iter)
	{
		// Key 세팅
		size_t Length = -1;
		Length = iter->first.length();
		fwrite(&Length, sizeof(int), 1, pFile);
		fwrite(iter->first.c_str(), sizeof(char), Length, pFile);

		iter->second->Save(pFile);
	}

	int CurrentAnimEnable = false;
	if (m_CurrentAnimation)
		CurrentAnimEnable = true;
	fwrite(&CurrentAnimEnable, sizeof(bool), 1, pFile);

	if (CurrentAnimEnable)
	{
		// CurrentAnimation의  경우, m_mapAnimation 중 하나,
		// 독립된 녀석이 아니므로, Name 만 저장해서 ResourceManager 로부터 가져오는 작업을 거칠 것이다
		size_t Length = -1;
		Length = (int)m_CurrentAnimation->GetName().length();
		fwrite(&Length, sizeof(int), 1, pFile);
		fwrite(m_CurrentAnimation->GetName().c_str(), sizeof(char), Length, pFile);
	}
}

void CAnimationSequence2DInstance::Load(FILE* pFile)
{
	fread(&m_PlayAnimation, sizeof(bool), 1, pFile);

	int AnimCount = -1;
	fread(&AnimCount, sizeof(int), 1, pFile);



	for (int i = 0; i < AnimCount; i++)
	{
		int Length = -1;
		char Name[MAX_PATH] = {};

		// Key 세팅
		fread(&Length, sizeof(int), 1, pFile);
		fread(Name, sizeof(char), Length, pFile);
		
		CAnimationSequence2DData* Data = new CAnimationSequence2DData;
		Data->Load(pFile);

		if (!Data->m_Sequence)
		{
			if (m_Scene)
			{
				Data->m_Sequence = m_Scene->GetResource()->FindAnimationSequence2D(Data->m_SequenceName);
			}
			else
			{
				Data->m_Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(Data->m_SequenceName);
			}
		}

		// 아래 2개 코드는 , Sprite 편집시 에러가 나지 않을까 ?
		if (!m_CurrentAnimation)
			m_CurrentAnimation = Data;

		m_mapAnimation.insert(std::make_pair(Name, Data));

	}

	int CurrentAnimEnable = false;
	fread(&CurrentAnimEnable, sizeof(bool), 1, pFile);

	if (CurrentAnimEnable)
	{
		int Length = -1;
		char Name[MAX_PATH] = {};

		// CurrentAnimation의  경우, m_mapAnimation 중 하나,
		// 독립된 녀석이 아니므로, Name 만 저장해서 ResourceManager 로부터 가져오는 작업을 거칠 것이다
		fread(&Length, sizeof(int), 1, pFile);
		fread(Name, sizeof(char), Length, pFile);

		m_CurrentAnimation = FindAnimationSequence2DData(Name);
	}

	if (m_Scene)
		m_CBuffer = m_Scene->GetResource()->GetAnimation2DCBuffer();
}

CAnimationSequence2DData* CAnimationSequence2DInstance::FindAnimationSequence2DData(const std::string& Name)
{
	auto iter = m_mapAnimation.find(Name);

	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}
