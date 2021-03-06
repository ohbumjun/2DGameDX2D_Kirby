#pragma once
#include "GameObject\GameObject.h"

class CDynaNest :
    public CGameObject
{
    friend class CScene;
    friend class CDynaNestBuilder;
    friend class CBossDyna;
private :
    CDynaNest();
    ~CDynaNest();
private :
    CSharedPtr<class CSpriteComponent> m_NestSprite;
private :
    float m_TimeFlowed;
    float m_GenerateDynaBabyTime;
private :
    // class CBossDyna* m_BossDyna;
    std::function<void()> m_CreateBossDynaFunction;

private :
    void MakeDynaBabyEffect();
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private :
    template<typename T>
    void SetCreateBossDynaFunction(T* Obj, void(T::*Func)())
{
        m_CreateBossDynaFunction = std::bind(Func, Obj);
}
};

