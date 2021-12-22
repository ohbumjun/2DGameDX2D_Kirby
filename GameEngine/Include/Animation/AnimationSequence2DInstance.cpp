#include "AnimationSequence2DInstance.h"
#include "../Component/SpriteComponent.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Resource/Texture/Texture.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"

CAnimationSequence2DInstance::CAnimationSequence2DInstance() :
	m_PlayAnimation(false),
	m_Owner(nullptr),
	m_Scene(nullptr)
{
	SetTypeID<CAnimationSequence2DInstance>();
}

CAnimationSequence2DInstance::CAnimationSequence2DInstance(const CAnimationSequence2DInstance& Anim)
{
	auto iter    = Anim.m_mapAnimation.begin();
	auto iterEnd = Anim.m_mapAnimation.end();

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
}

void CAnimationSequence2DInstance::AddAnimation(const std::string& SequenceName,
                                                const std::string& AnimationName, bool Loop,
                                                float              PlayTime, float     PlayScale, bool Reverse)
{
	CAnimationSequence2DData* Anim = FindAnimation(AnimationName);

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
			m_Owner->SetTexture(0, 0, static_cast<int>(ConstantBuffer_Shader_Type::Pixel),
			                    Anim->m_Sequence->GetTexture()->GetName(),
			                    Anim->m_Sequence->GetTexture());
		}
	}

	m_mapAnimation.insert(std::make_pair(AnimationName, Anim));
}

void CAnimationSequence2DInstance::SetPlayTime(const std::string& Name, float PlayTime)
{
	CAnimationSequence2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_PlayTime = PlayTime;
}

void CAnimationSequence2DInstance::SetPlayScale(const std::string& Name, float PlayScale)
{
	CAnimationSequence2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_PlayScale = PlayScale;
}

void CAnimationSequence2DInstance::SetReverse(const std::string& Name, bool Reverse)
{
	CAnimationSequence2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_Reverse = Reverse;
}

void CAnimationSequence2DInstance::SetLoop(const std::string& Name, bool Loop)
{
	CAnimationSequence2DData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_Loop = Loop;
}

void CAnimationSequence2DInstance::SetCurrentAnimation(const std::string& Name)
{
	m_CurrentAnimation = FindAnimation(Name);

	if (!m_CurrentAnimation)
		return;

	m_CurrentAnimation->m_Frame = 0;
	m_CurrentAnimation->m_Time  = 0.f;

	size_t Size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurrentAnimation->m_vecNotify[i]->Call = false;
	}

	if (m_Owner)
	{
		m_Owner->SetTexture(0, 0, static_cast<int>(ConstantBuffer_Shader_Type::Pixel),
		                    m_CurrentAnimation->m_Sequence->GetTexture()->GetName(),
		                    m_CurrentAnimation->m_Sequence->GetTexture());
	}
}

void CAnimationSequence2DInstance::ChangeAnimation(const std::string& Name)
{
	if (m_CurrentAnimation->m_Name == Name)
		return;

	m_CurrentAnimation->m_Frame = 0;
	m_CurrentAnimation->m_Time  = 0.f;

	size_t Size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurrentAnimation->m_vecNotify[i]->Call = false;
	}

	m_CurrentAnimation          = FindAnimation(Name);
	m_CurrentAnimation->m_Frame = 0;
	m_CurrentAnimation->m_Time  = 0.f;

	if (m_Owner)
	{
		m_Owner->SetTexture(0, 0, static_cast<int>(ConstantBuffer_Shader_Type::Pixel),
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

bool CAnimationSequence2DInstance::Save(const char* FullPath)
{
	FILE* pFile;
	fopen_s(&pFile, FullPath, "wb");
	if (!pFile)
		return false;

	int mapSize = static_cast<int>(m_mapAnimation.size());
	fwrite(&mapSize, sizeof(int), 1, pFile);

	auto iter                       = m_mapAnimation.begin();
	auto iterEnd                    = m_mapAnimation.end();
	int  SequenceDataNameLength     = -1;
	char SequenceDataName[MAX_PATH] = {};
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
		// CurName이 저장된 Key이름을 찾는다
		auto iter = m_mapAnimation.begin();
		auto iterEnd = m_mapAnimation.end();

		for (; iter != iterEnd; ++iter)
		{
			if (iter->second->GetName() == m_CurrentAnimation->GetName())
			{
				int CurAnimNameKeyLength = (int)iter->first.length();
				char CurAnimNameKey[MAX_PATH] = {};
				fwrite(m_CurrentAnimation->GetName().c_str(), sizeof(char), CurAnimNameKeyLength, pFile);
				m_CurrentAnimation->Save(pFile);
				break;
			}
		}
	}

	// CBuffer
	bool CBufferEnable = false;
	if (m_CBuffer)
		CBufferEnable = true;
	if (m_CBuffer)
		m_CBuffer->Save(pFile);

	fclose(pFile);

	return true;
}

bool CAnimationSequence2DInstance::Load(const char* FullPath)
{
	FILE* pFile;
	fopen_s(&pFile, FullPath, "rb");
	if (!pFile)
		return false;

	int MapSize = -1;
	fread(&MapSize, sizeof(int), 1, pFile);

	// -1을 해주는 이유는 CurrentAnimation은 따로 Load하기 위함이다.
	int SequenceDataKeyNameLength = -1;
	char SequenceData2DNameKey[MAX_PATH] = {};
	char AnimName[MAX_PATH] = {};

	for (int i = 0; i < MapSize; i++)
	{
		// - CAnimationSequence2DData
		// Key Name 저장 
		fread(&SequenceDataKeyNameLength, sizeof(int), 1, pFile);
		fread(SequenceData2DNameKey, sizeof(char), SequenceDataKeyNameLength, pFile);

		// CAnimationSequence2DData 를 저장하기 ===============================
		CAnimationSequence2DData* Sequence2DData = new CAnimationSequence2DData;
		Sequence2DData->Load(pFile, FullPath);

		m_mapAnimation.insert(std::make_pair(SequenceData2DNameKey, Sequence2DData));
	}

	// Current Animation Info
	bool CurrentAnimEnable = false;
	fread(&CurrentAnimEnable, sizeof(bool), 1, pFile);

	if (CurrentAnimEnable)
	{
		int CurAnimNameKeyLength = -1;
		fread(&CurAnimNameKeyLength, sizeof(int), 1, pFile);
		char CurAnimNameKey[MAX_PATH] = {};
		fread(CurAnimNameKey, sizeof(char), CurAnimNameKeyLength, pFile);

		auto iter = m_mapAnimation.begin();
		auto iterEnd = m_mapAnimation.end();

		for (;iter != iterEnd; ++iter)
		{
			if (iter->first == CurAnimNameKey)
			{
				m_CurrentAnimation = iter->second;
				break;
			}
		}
	}

	bool CBufferEnable = false;
	fread(&CBufferEnable, sizeof(bool), 1, pFile);

	if (CBufferEnable)
	{
		CAnimation2DConstantBuffer* ConstantBuffer = new CAnimation2DConstantBuffer;
		ConstantBuffer->Init();
		ConstantBuffer->Load(pFile);
	}

	fclose(pFile);
}

void CAnimationSequence2DInstance::Start()
{
	if (m_Owner && m_CurrentAnimation)
	{
		m_Owner->SetTexture(0, 0, static_cast<int>(ConstantBuffer_Shader_Type::Pixel),
		                    m_CurrentAnimation->m_Sequence->GetTexture()->GetName(),
		                    m_CurrentAnimation->m_Sequence->GetTexture());
	}
}

bool CAnimationSequence2DInstance::Init()
{
	if (m_Scene)
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
	fwrite(&AnimCount, sizeof(int), AnimCount, pFile);

	auto iter      = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	int Length = -1;
	for (;iter != iterEnd; ++iter)
	{
		// Key 세팅
		Length = iter->first.length();
		fwrite(&Length, sizeof(int), Length, pFile);
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
		Length = (int)m_CurrentAnimation->GetName().length();
		fwrite(&Length, sizeof(int), 1, pFile);
		fwrite(m_CurrentAnimation->GetName().c_str(), sizeof(char), Length, pFile);
	}
}

void CAnimationSequence2DInstance::Load(FILE* pFile)
{
}

CAnimationSequence2DData* CAnimationSequence2DInstance::FindAnimation(const std::string& Name)
{
	auto iter = m_mapAnimation.find(Name);

	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}
