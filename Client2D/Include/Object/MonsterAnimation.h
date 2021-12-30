#pragma once
#include "Animation\AnimationSequence2DInstance.h"

class CMonsterAnimation :
    public CAnimationSequence2DInstance
{
public :
    CMonsterAnimation();
    CMonsterAnimation(const CMonsterAnimation& Anim);
    virtual ~CMonsterAnimation();
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);

};

