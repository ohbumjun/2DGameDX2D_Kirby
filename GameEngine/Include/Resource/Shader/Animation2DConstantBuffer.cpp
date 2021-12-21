#include "Animation2DConstantBuffer.h"
#include "ConstantBuffer.h"

CAnimation2DConstantBuffer::CAnimation2DConstantBuffer() :
	m_BufferData{}
{
}

CAnimation2DConstantBuffer::CAnimation2DConstantBuffer(const CAnimation2DConstantBuffer& Buffer) :
	CConstantBufferBase(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CAnimation2DConstantBuffer::~CAnimation2DConstantBuffer()
{
}

bool CAnimation2DConstantBuffer::Init()
{
	SetConstantBuffer("Animation2DCBuffer");

	return true;
}

void CAnimation2DConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CAnimation2DConstantBuffer* CAnimation2DConstantBuffer::Clone()
{
	return new CAnimation2DConstantBuffer(*this);
}

void CAnimation2DConstantBuffer::Save(FILE* pFile)
{
	bool ContantBufferEnable = false;
	if (m_Buffer) // CConstantBuffer 
		ContantBufferEnable = true;
	fwrite(&ContantBufferEnable, sizeof(bool), 1, pFile);

	fwrite(&m_BufferData, sizeof(Animation2DCBuffer), 1, pFile);
}

void CAnimation2DConstantBuffer::Load(FILE* pFile)
{
	bool ContantBufferEnable = false;
	fread(&ContantBufferEnable, sizeof(bool), 1, pFile);

	if (ContantBufferEnable)
		SetConstantBuffer("Animation2DCBuffer");

	Animation2DCBuffer Animation2DCBufferData;
	fread(&Animation2DCBufferData, sizeof(Animation2DCBuffer), 1, pFile);
	SetAnimation2DCBuffer(Animation2DCBufferData);
}
