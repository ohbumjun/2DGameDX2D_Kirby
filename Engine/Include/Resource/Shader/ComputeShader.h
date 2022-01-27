#pragma once
#include "Shader.h"

class CComputeShader :
	public CShader
{
	friend class CShaderManager;

protected:
	CComputeShader();
	virtual ~CComputeShader() override = 0;
private :
	ID3D11ComputeShader* m_CSShader;
	ID3DBlob* m_CSBlob;
public:
	virtual bool Init() override = 0;
	bool LoadComputeShader(const char* EntryName, const TCHAR* FileName,
		const std::string& PathName);
	void Execute(unsigned int x, unsigned int y, unsigned int z);
	virtual void SetShader() override;
};
