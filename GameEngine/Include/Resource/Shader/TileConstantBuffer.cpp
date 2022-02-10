#include "TileConstantBuffer.h"

#include "ConstantBuffer.h"

CTileConstantBuffer::CTileConstantBuffer()
{}

CTileConstantBuffer::CTileConstantBuffer(const CTileConstantBuffer& Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CTileConstantBuffer::~CTileConstantBuffer()
{}

void CTileConstantBuffer::SetTileStartUV(const Vector2& Start)
{
	m_BufferData.Start = Start;
}

void CTileConstantBuffer::SetTileEndUV(const Vector2& End)
{
	m_BufferData.End = End;
}

void CTileConstantBuffer::SetTileSize(const Vector2& Size)
{
	m_BufferData.Size = Size;
}

void CTileConstantBuffer::SetTileImageSize(const Vector2& Size)
{
	m_BufferData.ImageSize = Size;
}

void CTileConstantBuffer::SetMatWVP(const Matrix& matWVP)
{
	m_BufferData.matWVP = matWVP;
}

bool CTileConstantBuffer::Init()
{
	SetConstantBuffer("TileCBuffer");
	return true;
}

void CTileConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}
