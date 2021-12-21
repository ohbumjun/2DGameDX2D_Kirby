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
	m_Reverse(false)
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

bool CAnimationSequence2DData::Save(FILE* pFile)
{
	int NameLength = static_cast<int>(m_Name.length());
	fwrite(&NameLength, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), NameLength, pFile);

	bool SequenceEnable = false;
	if (m_Sequence)
		SequenceEnable = true;

	if (m_Sequence)
	{
		m_Sequence->Save(pFile);
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
	}


	return true;
}

bool CAnimationSequence2DData::Load(FILE* pFile, const char* FullPath)
{
	int SequenceDataNameLength = -1;
	char SequenceAnimName[MAX_PATH] = {};

	fread(&SequenceDataNameLength, sizeof(int), 1, pFile);
	fread(SequenceAnimName, sizeof(char), SequenceDataNameLength, pFile);
	m_Name = SequenceAnimName;

	bool Sequence2DEnable = false;
	fread(&Sequence2DEnable, sizeof(bool), 1, pFile);

	int Frame = 0;
	float CurTime = 0, FrameTime = -1, PlayTime = -1, PlayScale = -1;
	bool Loop = false, Reverse = false;

	// --- CAnimationSequence2D
	if (Sequence2DEnable)
	{
		CAnimationSequence2D* Sequenc2D = new CAnimationSequence2D;
		Sequenc2D->Load(pFile);

		// Load 한 Sequence2D 정보를 가져오기 
		m_Sequence = Sequenc2D;

		// Frame 정보 
		fwrite(&Frame, sizeof(int), 1, pFile);
		m_Frame = Frame;
		fwrite(&CurTime, sizeof(float), 1, pFile);
		m_Time = CurTime;
		fwrite(&FrameTime, sizeof(float), 1, pFile);
		m_FrameTime = FrameTime;
		fwrite(&PlayTime, sizeof(float), 1, pFile);
		m_PlayTime = PlayTime;
		fwrite(&PlayScale, sizeof(float), 1, pFile);
		m_PlayScale = PlayScale;
		fwrite(&Loop, sizeof(bool), 1, pFile);
		m_Loop = Loop;
		fwrite(&Reverse, sizeof(bool), 1, pFile);
		m_Reverse = Reverse;
	}

	return true;

}
