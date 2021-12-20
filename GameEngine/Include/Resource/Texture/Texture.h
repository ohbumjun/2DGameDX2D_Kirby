#pragma once

#include "../../Ref.h"
#include "DirectXTex.h"

#ifdef _DEBUG

#pragma comment(lib, "DirectXTex_Debug.lib")

#else

#pragma comment(lib, "DirectXTex.lib")

#endif // _DEBUG

struct TextureResourceInfo
{
	ScratchImage*	Image;
	ID3D11ShaderResourceView* SRV;
	unsigned int	Width;
	unsigned int	Height;
	TCHAR* FileName;
	char* PathName;
	TCHAR* FullPath;

	TextureResourceInfo()	:
		Image(nullptr),
		SRV(nullptr),
		FileName(nullptr),
		PathName(nullptr),
		FullPath(nullptr),
		Width(0),
		Height(0)
	{
	}

	~TextureResourceInfo()
	{
		SAFE_RELEASE(SRV);
		SAFE_DELETE_ARRAY(FileName);
		SAFE_DELETE_ARRAY(PathName);
		SAFE_DELETE_ARRAY(FullPath);
		SAFE_DELETE(Image);
	}
};

class CTexture :
    public CRef
{
	friend class CTextureManager;

protected:
	CTexture();
	virtual ~CTexture();

protected:
	class CScene* m_Scene;
	std::vector<TextureResourceInfo*>	m_vecTextureInfo;
	Image_Type	m_ImageType;

public:
	ID3D11ShaderResourceView* GetResource(int Index = 0)	const
	{
		return m_vecTextureInfo[Index]->SRV;
	}
	Image_Type GetImageType()	const
	{
		return m_ImageType;
	}

	unsigned int GetWidth(int Index = 0)	const
	{
		return m_vecTextureInfo[Index]->Width;
	}

	unsigned int GetHeight(int Index = 0)	const
	{
		return m_vecTextureInfo[Index]->Height;
	}
	TCHAR* GetFileName(int Index = 0) const
	{
		return m_vecTextureInfo[Index]->FileName;
	}
	size_t GetImageCount()	const
	{
		return m_vecTextureInfo.size();
	}

public:
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);

private:
	bool CreateResource(int Index);
public:
	void SetShader(int Register, int ShaderType, int Index);
	void ResetShader(int Register, int ShaderType, int Index);
public :
	void Save(FILE* pFile);
};

