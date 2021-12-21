#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CPlayerAnimation2D :
	public CAnimationSequence2DInstance
{
public:
	CPlayerAnimation2D();
	CPlayerAnimation2D(const CPlayerAnimation2D& Anim);
	virtual ~CPlayerAnimation2D() override;

public:
	virtual bool                Init() override;
	virtual CPlayerAnimation2D* Clone() override;
};
