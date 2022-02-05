#include "PaperBurnConstantBuffer.h"

#include "ConstantBuffer.h"

CPaperBurnConstantBuffer::CPaperBurnConstantBuffer()
{}

CPaperBurnConstantBuffer::CPaperBurnConstantBuffer(const CPaperBurnConstantBuffer& Buffer) :
CConstantBufferBase(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CPaperBurnConstantBuffer::~CPaperBurnConstantBuffer()
{}

bool CPaperBurnConstantBuffer::Init()
{
	return true;
}

void CPaperBurnConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CPaperBurnConstantBuffer* CPaperBurnConstantBuffer::Clone()
{
	return new CPaperBurnConstantBuffer(*this);
}
