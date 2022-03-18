#pragma once
#include "Item.h"

class CHPYellowItem :
    public CItem
{
    friend class CScene;
protected:
    CHPYellowItem();
    CHPYellowItem(const CHPYellowItem& Monster);
    virtual ~CHPYellowItem();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CHPYellowItem* Clone() override;
public:
    virtual void Save(FILE* pFile) override;
};

