#pragma once
#include "Item.h"

class CHPRedItem :
    public CItem
{
    friend class CScene;
protected:
    CHPRedItem();
    CHPRedItem(const CHPRedItem& Monster);
    virtual ~CHPRedItem();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CHPRedItem* Clone() override;
public :
    virtual void Save(FILE* pFile) override;
    virtual void Load(FILE* pFile) override;
};

