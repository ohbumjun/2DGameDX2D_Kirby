#pragma once
#include "Component/SpriteComponent.h"
#include "GameObject\GameObject.h"

class CEffectSceneChangeAlpha :
    public CGameObject
{
public:
    CEffectSceneChangeAlpha();
    virtual ~CEffectSceneChangeAlpha();
private :
    CSharedPtr<class CSpriteComponent> m_Sprite;
    bool m_SceneStart;
    float m_SceneChangeTime;
    float m_SceneChangeTimeMax;
    std::function<void()> m_SceneChangeCallback;
    
public :
    void SetSceneStart(bool Enable)
{
        m_SceneStart = Enable;

    if (Enable)
    {
        m_Sprite->SetOpacity(0.f);
    }
    else
    {
        m_Sprite->SetOpacity(1.f);
    }
}
private :
    void UpdateSceneChangeTime(float DeltaTime);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime)override;
public :
    template<typename T>
    void SetSceneChangeCallback(T* Obj, void(T::*Func)())
{
        m_SceneChangeCallback = std::bind(Func, Obj);
}
};

