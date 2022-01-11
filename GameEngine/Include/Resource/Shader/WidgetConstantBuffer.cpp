#include "WidgetConstantBuffer.h"

#include "ConstantBuffer.h"

CWidgetConstantBuffer::CWidgetConstantBuffer() :
m_BufferData{}
{
	// 기본 흰색 지정
	m_BufferData.Tint = Vector4(1.f, 1.f, 1.f, 0.f);
}

CWidgetConstantBuffer::CWidgetConstantBuffer(const CWidgetConstantBuffer& Widget) :
CConstantBufferBase(Widget)
{
	m_BufferData = Widget.m_BufferData;
}

CWidgetConstantBuffer::~CWidgetConstantBuffer()
{}

bool CWidgetConstantBuffer::Init()
{
	SetConstantBuffer("WidgetCBuffer");

	return true;
}

void CWidgetConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CWidgetConstantBuffer* CWidgetConstantBuffer::Clone()
{
	return new CWidgetConstantBuffer(*this);
}
