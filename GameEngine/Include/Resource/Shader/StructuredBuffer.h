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
	std::string GetName() const
{
		return m_Name;
}
public :
	bool Init(const std::string& Name, int Register, int Size, bool Dynamic, int Count,
		int StructuredBufferShaderType = (int)Buffer_Shader_Type::Compute);
	void UpdateBuffer(void* Data, int Count);
	CStructuredBuffer* Clone();
	void SetShader();
	void SetShader(int Register, int ShaderType);
	void ResetShader();
	void ResetShader(int Register, int ShaderType);
	void CopyData(void* Data);// gpu 의 계산 셰이더 결과 내용 읽어오기 
	void CopyResource(CStructuredBuffer* Buffer); // 자원 복사
private :
	// void SetShader(int Register, int StructuredBufferShaderType);
	// void ResetShader(int Register, int StructuredBufferShaderType);
};

