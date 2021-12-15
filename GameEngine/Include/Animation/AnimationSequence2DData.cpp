
#include "AnimationSequence2DData.h"
#include "../Resource/Animation/AnimationSequence2D.h"

CAnimationSequence2DData::CAnimationSequence2DData()	:
	m_Frame(0),
	m_Time(0.f),
	m_FrameTime(0.f),
	m_PlayTime(0.f),
	m_PlayScale(1.f),
	m_Loop(0),
	m_Reverse(0)
{
}

CAnimationSequence2DData::~CAnimationSequence2DData()
{
	auto	iter = m_vecNotify.begin();
	auto	iterEnd = m_vecNotify.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
}
