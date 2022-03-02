#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/SpriteMesh.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Material/Material.h"
#include "TileEmpty.h"

class CTile;

class CTileEmptyComponent :
    public CSceneComponent
{
    friend class CGameObject;
protected:
    CTileEmptyComponent();
    CTileEmptyComponent(const CTileEmptyComponent& Component);
    virtual ~CTileEmptyComponent();
protected:
    CSharedPtr<CSpriteMesh> m_ImageMesh;
    CSharedPtr<CMaterial> m_TileImageMaterial;
protected:
    CSharedPtr<CShader> m_TileShader;
    CSharedPtr<CMesh> m_TileMesh;
    std::vector<CTileEmpty*> m_vecTileEmpty;
    std::vector<TileEmptyInfo> m_vecTileEmptyInfo;
    class CStructuredBuffer* m_TileInfoBuffer;
protected:
    int m_CountX;
    int m_CountY;
    int m_Count;
    int m_RenderCount;
    Vector3 m_TileEmptySize;
    Vector4 m_TileEmptyColor[(int)Tile_Type::End];
    bool m_EditMode;
    float m_DeltaTime;
public:
    void EnableEditMode(bool Mode)
    {
        m_EditMode = Mode;
    }
public:
    Vector3 GetTileEmptySize () const
    {
        return m_TileEmptySize;
    }
    CMaterial* GetTileImageMaterial()    const
    {
        return m_TileImageMaterial;
    }
    int GetTileCountX() const
    {
        return m_CountX;
    }
    int GetTileCountY() const
    {
        return m_CountY;
    }
    int GetTileCount() const
    {
        return m_Count;
    }
    const Vector3& GetTileEmtpySize()    const
    {
        return m_TileEmptySize;
    }
public :
    void SetEditMode (bool Enable)
    {
        m_EditMode = Enable;
    }
public: // Tile Image
    void SetTileImageMaterial(CMaterial* Material); 
    void SetTileImageBaseColor(const Vector4& Color);
    void SetTileImageBaseColor(float r, float g, float b, float a);
    void SetTileImageRenderState(class CRenderState* State);
    void SetTileImageRenderState(const std::string& Name);
    void SetTileImageTransparency(bool Enable);
    void SetTileImageOpacity(float Opacity);
    void AddTileImageOpacity(float Opacity);
public:
    void AddTileImageTexture(int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
    void AddTileImageTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void AddTileImageTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
    void AddTileImageTexture(int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
    void SetTileImageTexture(int Index, int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
    void SetTileImageTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTileImageTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
    void SetTileImageTexture(int Index, int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
public :
    void CreateTileEmpty(int CountX, int CountY, const Vector3& Size);
public :
    void SetTileEmptyColor(Tile_Type Type, float r, float g, float b, float a);
    void SetTileEmptyColor(Tile_Type Type, const Vector4& Color);
    void SetTileDefaultSize(float x, float y);
public:
    int GetTileEmptyIndexX(const Vector3& Pos);
    int GetTileEmptyIndexX(float PosX);
    int GetTileEmptyIndexY(const Vector3& Pos);
    int GetTileEmptyIndexY(float PosY);
    int GetTileEmptyIndex(const Vector3& Pos);
    CTileEmpty* GetTileEmpty(const Vector3& Pos);
    CTileEmpty* GetTileEmpty(int x, int y);
    CTileEmpty* GetTileEmpty(int Index);
private:
    int GetTileEmptyRenderIndexX(const Vector3& Pos);
    int GetTileEmptyRenderIndexY(const Vector3& Pos);
public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CTileEmptyComponent* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
public:
    void SetWorldInfo();
};

