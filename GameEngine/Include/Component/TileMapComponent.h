#pragma once
#include "SceneComponent.h"
#include "Tile.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Mesh/SpriteMesh.h"

class CTileMapComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CTileMapComponent();
    CTileMapComponent(const CTileMapComponent& com);
    virtual ~CTileMapComponent();

protected:
    Tile_Shape m_TileShape;
    int m_RenderCount;
    int m_CountX;
    int m_CountY;
    Vector3 m_TileSize;
    CSharedPtr<CSpriteMesh> m_BackMesh;
    CSharedPtr<CMaterial> m_BackMaterial;
    CSharedPtr<CMaterial> m_TileMaterial;
    std::vector<CTile*> m_vecTile;
    class CTileConstantBuffer* m_CBuffer;

public:
    CMaterial* GetMaterial()    const
    {
        return m_BackMaterial;
    }

public:
    void SetBackMaterial(CMaterial* Material);
    void SetTileMaterial(CMaterial* Material);
    void CreateTile(Tile_Shape Shape, int CountX, int CountY, const Vector3& Size);
public:
    void SetBackBaseColor(const Vector4& Color);
    void SetBackBaseColor(float r, float g, float b, float a);
    void SetBackRenderState(class CRenderState* State);
    void SetBackRenderState(const std::string& Name);
    void SetBackTransparency(bool Enable);
    void SetBackOpacity(float Opacity);
    void AddBackOpacity(float Opacity);

public:
    void AddBackTexture(int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
    void AddBackTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void AddBackTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
    void AddBackTexture(int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);


    void SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
    void SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetBackTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
    void SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CTileMapComponent* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};


