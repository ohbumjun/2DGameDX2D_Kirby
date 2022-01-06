#pragma once
#include "ColliderComponent.h"

class CColliderPixel :
    public CColliderComponent
{
    friend class CGameObject;
protected :
    CColliderPixel();
    CColliderPixel(const CColliderPixel& Pixel);
    virtual ~CColliderPixel() override;
protected :
    PixelInfo m_Info;
    CSharedPtr<class CMesh> m_PixelMesh;
    CSharedPtr<class CShader> m_PixelShader;
public :
    PixelInfo GetInfo() const
{
        return m_Info;
}
    // 사실상 SetInfo는 Texture를 Load하여 세팅하는 것과 같은 과정을 거치게 될 것이다.
    void SetInfo(const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetInfoFullPath(const TCHAR* FullPath);
    void SetInfoMultibyte(const char* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetInfoMultibyteFullPath(const char* FullPath);
    void SetInfoCollisionType(PixelCollision_Type Type);
    void SetPixelColor(unsigned char r, unsigned char g, unsigned char b);
    void SetPixelAlpha(unsigned char a);
    void SetPixelCollisionType(PixelCollision_Type Type);
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CColliderPixel* Clone() override;
public :
    virtual void Save(FILE* pFile) override;
    virtual void Load(FILE* pFile) override;
public :
    virtual bool Collision(CColliderComponent* Dest) override;
    virtual bool CollisionMouse(const Vector2& MousePos) override;

};

