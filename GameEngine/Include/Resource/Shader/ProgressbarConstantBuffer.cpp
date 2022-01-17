#include "ProgressbarConstantBuffer.h"

#include "ConstantBuffer.h"

CProgressbarConstantBuffer::CProgressbarConstantBuffer() :
m_BufferData{}
{}

CProgressbarConstantBuffer::CProgressbarConstantBuffer(const CProgressbarConstantBuffer& Buffer)
	: CConstantBufferBase(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CProgressbarConstantBuffer::~CProgressbarConstantBuffer()
{}

bool CProgressbarConstantBuffer::Init()
{
	SetConstantBuffer("ProgressCBuffer");
	return true;
}

void CProgressbarConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CProgressbarConstantBuffer* CProgressbarConstantBuffer::Clone()
{
	return new CProgressbarConstantBuffer(*this);
}
