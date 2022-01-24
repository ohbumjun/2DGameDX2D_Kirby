#pragma once

#include "../../GameInfo.h"

class CStructuredBuffer
{
public :
	CStructuredBuffer();
	CStructuredBuffer(const CStructuredBuffer& Buffer);
	~CStructuredBuffer();
private :
	std::string m_Name;
	D3D11_BUFFER_DESC m_Desc;
	ID3D11Buffer* m_Buffer;
private :
	ID3D11ShaderResourceView* m_SRV;
	ID3D11UnorderedAccessView* m_UAV;
private :
	unsigned int m_Size;
	unsigned int m_Count;
	int m_StructuredBufferShaderType;
	int m_Register;
	int m_Dynamic;
public :
	ID3D11Buffer* GetBuffer() const
{
		return m_Buffer;
}
public :
	bool Init(const std::string& Name, unsigned int Size, unsigned int Count,
		int Register, bool Dynamic = false, int StructuredBufferShaderType = (int)Buffer_Shader_Type::Compute);
};

