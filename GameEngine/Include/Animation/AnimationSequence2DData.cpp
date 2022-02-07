#include "AnimationSequence2DData.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"

CAnimationSequence2DData::CAnimationSequence2DData() :
	m_Frame(0),
	m_Time(0.f),
	m_FrameTime(0.f),
	m_PlayTime(0.f),
	m_PlayScale(1.f),
	m_Loop(false),
	m_Reverse(false),
	m_FrameReverse(false)
{
}

CAnimationSequence2DData::~CAnimationSequence2DData()
{
	auto iter    = m_vecNotify.begin();
	auto iterEnd = m_vecNotify.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
}

void CAnimationSequence2DData::SetSequence2D(CAnimationSequence2D* Sequence2D)
{
	m_Sequence = Sequence2D;
}

bool CAnimationSequence2DData::Save(FILE* pFile)
{
	int NameLength = static_cast<int>(m_Name.length());
	fwrite(&NameLength, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), NameLength, pFile);

	bool SequenceEnable = false;
	if (m_Sequence)
		SequenceEnable = true;
	fwrite(&SequenceEnable, sizeof(bool), 1, pFile);

	if (m_Sequence)
	{
		int Frame = 0;
		fwrite(&Frame, sizeof(int), 1, pFile);
		float Time = 0; // 애니메이션 동작 시간
		fwrite(&Time, sizeof(float), 1, pFile);
		float FrameTime = 0.f; // 1프레임당 시간
		fwrite(&FrameTime, sizeof(float), 1, pFile);
		fwrite(&m_PlayTime, sizeof(float), 1, pFile);
		fwrite(&m_PlayScale, sizeof(float), 1, pFile);
		fwrite(&m_Loop, sizeof(bool), 1, pFile);
		fwrite(&m_Reverse, sizeof(bool), 1, pFile);

		std::string SequenceName = m_Sequence->GetName();

		NameLength = (int)SequenceName.length();
		fwrite(&NameLength, sizeof(int), 1, pFile);
		fwrite(SequenceName.c_str(), sizeof(char), NameLength, pFile);

		m_Sequence->Save(pFile);
	}

	return true;
}

bool CAnimationSequence2DData::Load(FILE* pFile)
{
	int NameLength = -1;
	fread(&NameLength, sizeof(int), 1, pFile);

	char Name[MAX_PATH] = {};
	fread(Name, sizeof(char), NameLength, pFile);
	m_Name = Name;

	bool SequenceEnable = false;
	if (m_Sequence)
		SequenceEnable = true;
	fread(&SequenceEnable, sizeof(bool), 1, pFile);

	if (SequenceEnable)
	{
		int Frame = 0;
		fread(&Frame, sizeof(int), 1, pFile);
		m_Frame = Frame;

		float Time = 0; // 애니메이션 동작 시간
		fread(&Time, sizeof(float), 1, pFile);
		m_Time = Time;

		float FrameTime = 0.f; // 1프레임당 시간
		fread(&FrameTime, sizeof(float), 1, pFile);
		m_FrameTime = FrameTime;

		fread(&m_PlayTime, sizeof(float), 1, pFile);
		fread(&m_PlayScale, sizeof(float), 1, pFile);
		fread(&m_Loop, sizeof(bool), 1, pFile);
		fread(&m_Reverse, sizeof(bool), 1, pFile);

		int SeqNameLength = -1;
		fread(&SeqNameLength, sizeof(int), 1, pFile);

		char SeqName[MAX_PATH] = {};
		fread(SeqName, sizeof(char), NameLength, pFile);

		m_SequenceName = Name;
		m_Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(m_SequenceName);

		if (!m_Sequence)
		{
			m_Sequence = new CAnimationSequence2D;
		}
		m_Sequence->Load(pFile);
	}

	return true;
}
