#pragma once
#include "Item.h"

class CHPGreenItem :
    public CItem
{
    friend class CScene;
protected:
    CHPGreenItem();
    CHPGreenItem(const CHPGreenItem& Monster);
    virtual ~CHPGreenItem();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CHPGreenItem* Clone() override;
public:
    virtual void Save(FILE* pFile) override;
    virtual void Load(FILE* pFile) override;
};

