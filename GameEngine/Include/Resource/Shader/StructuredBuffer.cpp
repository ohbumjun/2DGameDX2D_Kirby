#include "StructuredBuffer.h"
#include "../../Device.h"

CStructuredBuffer::CStructuredBuffer() :
	m_Name{},
	m_Buffer(nullptr),
	m_SRV(nullptr),
	m_UAV(nullptr),
	m_Size(0),
	m_Count(0),
	m_StructuredBufferShaderType(0),
	m_Register(0),
	m_Dynamic(0)
{}

CStructuredBuffer::CStructuredBuffer(const CStructuredBuffer& Buffer)
{
	*this = Buffer;

	// Buffer 생성
	CDevice::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer);

	// Shader 자원 뷰
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;
	SRVDesc.BufferEx.NumElements = m_Count;

	CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_Buffer, &SRVDesc, &m_SRV);

	// Unordered 뷰
	if (!m_Dynamic)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.Flags = 0;
		UAVDesc.Buffer.NumElements = m_Count;

		CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Buffer, &UAVDesc, &m_UAV);
	}

}

CStructuredBuffer::~CStructuredBuffer()
{
	SAFE_RELEASE(m_Buffer);
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
}

bool CStructuredBuffer::Init(const std::string& Name, int Register, int Size, bool Dynamic, int Count,
	int StructuredBufferShaderType)
{
	SAFE_RELEASE(m_Buffer);
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);

	m_Dynamic = Dynamic;
	m_Register = Register;
	m_Size = Size;
	m_Count = Count;
	m_Name = Name;
	m_StructuredBufferShaderType = StructuredBufferShaderType;

	// 구조체 버퍼 서술 구조체
	m_Desc.ByteWidth = m_Size * m_Count;
	m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_Desc.StructureByteStride = m_Size;

	if (m_Dynamic)
	{
		m_Desc.Usage = D3D11_USAGE_DYNAMIC;
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else
	{
		m_Desc.Usage = D3D11_USAGE_DEFAULT;
		m_Desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	}

	// 구조 버퍼 생성
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer)))
		return false;

	// 셰이더 자원 뷰 생성
	CD3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;
	SRVDesc.BufferEx.NumElements = Count;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_Buffer, &SRVDesc, &m_SRV)))
		return false;

	// gpu 상에서의 읽기, 쓰기를 지원할 것이라면
	if (!m_Dynamic)
	{
		CD3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};

		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.Flags = 0;
		UAVDesc.Buffer.NumElements = Count;

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Buffer, &UAVDesc, &m_UAV)))
			return false;
	}

	return true;
}

void CStructuredBuffer::UpdateBuffer(void* Data, int Count)
{
	if (!m_Dynamic)
		return;

	D3D11_MAPPED_SUBRESOURCE Map = {};

	CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&Map);

	memcpy(Map.pData, Data, m_Size * Count);

	CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);
}

CStructuredBuffer* CStructuredBuffer::Clone()
{
	return new CStructuredBuffer(*this);
}

void CStructuredBuffer::SetShader()
{
	// Shader Resource View 의 경우, 다양한 파이프라인에 동시에 연결 가능
	if (m_Dynamic)
	{
		if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Vertex)
			CDevice::GetInst()->GetContext()->VSSetShaderResources(m_Register, 1, &m_SRV);

		if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Pixel)
			CDevice::GetInst()->GetContext()->PSSetShaderResources(m_Register, 1, &m_SRV);

		if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Domain)
			CDevice::GetInst()->GetContext()->DSSetShaderResources(m_Register, 1, &m_SRV);

		if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Hull)
			CDevice::GetInst()->GetContext()->HSSetShaderResources(m_Register, 1, &m_SRV);

		if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Geometry)
			CDevice::GetInst()->GetContext()->GSSetShaderResources(m_Register, 1, &m_SRV);

		if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Compute)
			CDevice::GetInst()->GetContext()->CSSetShaderResources(m_Register, 1, &m_SRV);
	}

	// Unordered 의 경우, 한번만 연결가능
	else
	{
		unsigned int  Count = -1;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(m_Register, 1, &m_UAV, &Count);
	}

}

void CStructuredBuffer::ResetShader()
{
	// Shader Resource View 의 경우, 다양한 파이프라인에 동시에 연결 가능
	if (m_Dynamic)
	{
		ID3D11ShaderResourceView* SRV = nullptr;

		if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Vertex)
			CDevice::GetInst()->GetContext()->VSSetShaderResources(m_Register, 1, &SRV);

		if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Pixel)
			CDevice::GetInst()->GetContext()->PSSetShaderResources(m_Register, 1, &SRV);

		if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Domain)
			CDevice::GetInst()->GetContext()->DSSetShaderResources(m_Register, 1, &SRV);

		if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Hull)
			CDevice::GetInst()->GetContext()->HSSetShaderResources(m_Register, 1, &SRV);

		if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Geometry)
			CDevice::GetInst()->GetContext()->GSSetShaderResources(m_Register, 1, &SRV);

		if (m_StructuredBufferShaderType & (int)Buffer_Shader_Type::Compute)
			CDevice::GetInst()->GetContext()->CSSetShaderResources(m_Register, 1, &SRV);
	}

	// Unordered 의 경우, 한번만 연결가능
	else
	{
		ID3D11UnorderedAccessView* UAV = nullptr;
		unsigned int  Count = -1;

		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(m_Register, 1, &UAV, &Count);
	}

}

/*
void CStructuredBuffer::SetShader(int Register, int StructuredBufferShaderType)
{
		if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Vertex)
		CDevice::GetInst()->GetContext()->VSSetShaderResources(Register, 1, &m_SRV);

	if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Pixel)
		CDevice::GetInst()->GetContext()->PSSetShaderResources(Register, 1, &m_SRV);

	if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Domain)
		CDevice::GetInst()->GetContext()->DSSetShaderResources(Register, 1, &m_SRV);

	if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Hull)
		CDevice::GetInst()->GetContext()->HSSetShaderResources(Register, 1, &m_SRV);

	if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Geometry)
		CDevice::GetInst()->GetContext()->GSSetShaderResources(Register, 1, &m_SRV);

	if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Compute)
		CDevice::GetInst()->GetContext()->CSSetShaderResources(Register, 1, &m_SRV);
}

void CStructuredBuffer::ResetShader(int Register, int StructuredBufferShaderType)
{
		ID3D11ShaderResourceView* SRV = nullptr;

	if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Vertex)
		CDevice::GetInst()->GetContext()->VSSetShaderResources(Register, 1, &SRV);

	if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Pixel)
		CDevice::GetInst()->GetContext()->PSSetShaderResources(Register, 1, &SRV);

	if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Domain)
		CDevice::GetInst()->GetContext()->DSSetShaderResources(Register, 1, &SRV);

	if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Hull)
		CDevice::GetInst()->GetContext()->HSSetShaderResources(Register, 1, &SRV);

	if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Geometry)
		CDevice::GetInst()->GetContext()->GSSetShaderResources(Register, 1, &SRV);

	if (StructuredBufferShaderType & (int)Buffer_Shader_Type::Compute)
		CDevice::GetInst()->GetContext()->CSSetShaderResources(Register, 1, &SRV);
}
*/

void CStructuredBuffer::CopyData(void* Data)
{
	D3D11_MAPPED_SUBRESOURCE Map = {};

	CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_READ, 0, &Map);

	memcpy(Data, Map.pData, m_Size * m_Count);

	CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);

}

void CStructuredBuffer::CopyResource(CStructuredBuffer* Buffer)
{
	CDevice::GetInst()->GetContext()->CopyResource(Buffer->m_Buffer, m_Buffer);

}
