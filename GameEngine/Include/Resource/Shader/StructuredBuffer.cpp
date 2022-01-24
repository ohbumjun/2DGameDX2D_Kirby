#include "StructuredBuffer.h"
#include "../../Device.h"

CStructuredBuffer::CStructuredBuffer()
{}

CStructuredBuffer::CStructuredBuffer(const CStructuredBuffer& Buffer)
{}

CStructuredBuffer::~CStructuredBuffer()
{}

bool CStructuredBuffer::Init(const std::string& Name, unsigned Size, unsigned Count, int Register, bool Dynamic,
	int StructuredBufferShaderType)
{
	m_Dynamic = Dynamic;
	m_Name    = Name;
	m_Size      = Size;
	m_Count    = Count;
	m_Register = Register;
	m_StructuredBufferShaderType = StructuredBufferShaderType;

	m_Desc.ByteWidth = m_Size * m_Count;
	m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_Desc.StructureByteStride = m_Size;

	if (m_Dynamic)
	{
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_Desc.Usage = D3D11_USAGE_DYNAMIC;
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_Desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		m_Desc.Usage = D3D11_USAGE_DEFAULT;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr, &m_Buffer)))
		return false;

	// ¼ÎÀÌ´õ ÀÚ¿ø ºä
	CD3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.NumElements = m_Count;;
	SRVDesc.BufferEx.Flags = 0;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_Buffer, &SRVDesc, &m_SRV)))
		return false;


	// ¼ø¼­ ¾ø´Â Á¢±Ù ºä
	if (!m_Dynamic)
	{
		CD3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.NumElements = m_Count;;
		UAVDesc.Buffer.Flags = 0;

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Buffer, &UAVDesc, &m_UAV)))
			return false;
	}

	return true;
}
