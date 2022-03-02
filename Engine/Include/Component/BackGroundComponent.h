#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/SpriteMesh.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Material/Material.h"
#include "TileEmpty.h"

class CTile;

class CBackGroundComponent :
    public CSceneComponent
{
    friend class CGameObject;
protected:
    CBackGroundComponent();
    CBackGroundComponent(const CBackGroundComponent& Component);
    virtual ~CBackGroundComponent();
protected:
    CSharedPtr<CSpriteMesh> m_BackGroundMesh;
    CSharedPtr<CMaterial> m_BackGroundMaterial;
protected:
    float m_DeltaTime;
public:
    void EnableEditMode(bool Mode)
    {
        m_EditMode = Mode;
    }
    CMaterial* GetBackGroundMaterial()    const
    {
        return m_BackGroundMaterial;
    }

public: // BackGround Image
    void SetBackGroundMaterial(CMaterial* Material);
    void SetBackGroundBaseColor(const Vector4& Color);
    void SetBackGroundBaseColor(float r, float g, float b, float a);
    void SetBackGroundRenderState(class CRenderState* State);
    void SetBackGroundRenderState(const std::string& Name);
    void SetBackGroundTransparency(bool Enable);
    void SetBackGroundOpacity(float Opacity);
    void AddBackGroundOpacity(float Opacity);
public:
    void AddBackGroundTexture(int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
    void AddBackGroundTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void AddBackGroundTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
    void AddBackGroundTexture(int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
    void SetBackGroundTexture(int Index, int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
    void SetBackGroundTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetBackGroundTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
    void SetBackGroundTexture(int Index, int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
    
public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CBackGroundComponent* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

};

