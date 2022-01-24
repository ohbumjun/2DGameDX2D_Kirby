#pragma once

#include "Animation/AnimationManager.h"
#include "Material/MaterialManager.h"
#include "Mesh/MeshManager.h"
#include "Shader/ShaderManager.h"
#include "Texture/TextureManager.h"
#include "Sound/SoundManager.h"
#include "Font/FontManager.h"

class CResourceManager
{
private:
	CMeshManager*      m_MeshManager;
	CShaderManager*    m_ShaderManager;
	CMaterialManager*  m_MaterialManager;
	CTextureManager*   m_TextureManager;
	CAnimationManager* m_AnimationManager;
	CSoundManager*    m_SoundManager;
	CFontManager*	     m_FontManager;

public:
	bool Init();
	void Update();

public: // =================== Mesh =====================
	class CMesh* FindMesh(const std::string& Name);
	void ReleaseMesh(const std::string& Name);

public: // =================== Shader =====================
	template <typename T>
	bool CreateShader(const std::string& Name)
	{
		return m_ShaderManager->CreateShader<T>(Name);
	}

	class CShader* FindShader(const std::string& Name);

	void ReleaseShader(const std::string& Name);

	bool CreateConstantBuffer(const std::string& Name, int Size, int Register,
	                          int ConstantBufferShaderType = static_cast<int>(Buffer_Shader_Type::All));

	class CConstantBuffer* FindConstantBuffer(const std::string& Name);


public: // =================== Material =====================
	CMaterial* FindMaterial(const std::string& Name);

	void ReleaseMaterial(const std::string& Name);

public:
	template <typename T>
	bool CreateMaterial(const std::string& Name)
	{
		return m_MaterialManager->CreateMaterial<T>(Name);
	}
	template <typename T>
	T* CreateMaterialEmpty()
	{
		return m_MaterialManager->CreateMaterialEmpty<T>();
	}

public: // =================== Texture =====================
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
	                 const std::string& PathName = TEXTURE_PATH);
	bool            LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	class CTexture* FindTexture(const std::string& Name);
	void            ReleaseTexture(const std::string& Name);

public: // =================== Sequence2D =====================
	bool CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName,
	                               const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& Name, CTexture* Texture);
	void AddAnimationSequence2DFrame(const std::string& Name, const Vector2& Start, const Vector2& Size);
	void AddAnimationSequence2DFrame(const std::string& Name, float StartX, float StartY, float Width, float Height);
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& Name);
	void ReleaseAnimationSequence2D(const std::string& Name);
	auto GetAnimation2DCBuffer() const -> class CAnimation2DConstantBuffer*;

	bool AddSequence2D(CAnimationSequence2D* Sequence2D);
	bool LoadSequence2DFullPath(const char* FullPath, const class CScene* Scene = nullptr);
	bool LoadSequence2DFullPath(std::string& SequenceName, const char* FullPath, const class CScene* Scene = nullptr);
	bool LoadSequence2D(std::string& SequenceName, const char* FileName, const std::string& PathName = TEXTURE_PATH ,const class CScene* Scene = nullptr);

public: // =================== Sound =====================
	CSound* LoadSound(const std::string& GroupName, const std::string& SoundName,
		bool Loop, const TCHAR* FileName,
		const std::string& PathName = SOUND_PATH);
	bool CreateChannelGroup(const std::string& GroupName);
	void ReleaseSound(const std::string& SoundName);
	CSound* FindSound(const std::string& SoundName);
	FMOD::ChannelGroup* FindGroup(const std::string& GroupName);
	void SoundPlay(const std::string& SoundName);
	void SoundStop(const std::string& SoundName);
	void SoundResume(const std::string& SoundName);
	void SoundPause(const std::string& SoundName);
	void SetVolume(const std::string& SoundName, float Volume);
	void SetEntireVolume(float Volume);
	void SetGroupVolume(const std::string& GroupName, float Volume);

public: // =================== Font =====================
	bool CreateFontFile(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = FONT_PATH);
	bool LoadFont(const std::string& Name, const TCHAR* FontName, int Weight, float FontSize,
		const TCHAR* LocalName, int Stretch = DWRITE_FONT_STRETCH_NORMAL);
	const TCHAR* GetFontFaceName(const std::string& Name);
	const char* GetFontFaceNameMultibyte(const std::string& Name);
	bool CreateFontColor(float r, float g, float b, float a);
	bool CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool CreateFontColor(const Vector4& Color);
	bool CreateFontColor(unsigned int Color);
	ID2D1SolidColorBrush* FindFontColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& Color);
	ID2D1SolidColorBrush* FindFontColor(unsigned int Color);
	unsigned int CreateFontColorKey(float r, float g, float b, float a);
	unsigned int CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	unsigned int CreateFontColorKey(const Vector4& Color);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* Text, IDWriteTextFormat* Font, float Width,
		float Height);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* Text, const std::string& FontName, float Width,
		float Height);

	IDWriteFontCollection1* FindFontFile(const std::string& Name);
	IDWriteTextFormat* FindFont(const std::string& Name);

	DECLARE_SINGLE(CResourceManager);
};
