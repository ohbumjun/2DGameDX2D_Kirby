#pragma once
#include "BossMonster.h"
class CBossTree :
    public CBossMonster
{
    friend class CScene;
protected:
    CBossTree();
    CBossTree(const CBossTree& Monster);
    virtual ~CBossTree();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBossTree* Clone() override;
};

