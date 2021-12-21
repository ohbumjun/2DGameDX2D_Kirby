#include "DepthStencilState.h"
#include "../Device.h"

CDepthStencilState::CDepthStencilState() :
	m_StencilRef(0xffffffff),
	m_PrevStencilRef(0xffffffff)
{
}

CDepthStencilState::~CDepthStencilState()
{
}

// 깊이 테스트, 스텐실 테스트를 초기화 한다. 
bool CDepthStencilState::CreateState(bool                       DepthEnable, D3D11_DEPTH_WRITE_MASK   DepthWriteMAsk,
                                     D3D11_COMPARISON_FUNC      DepthFunc, bool                       StencilEnable,
                                     UINT8                      StencilReadMask, UINT8                StencilWriteMask,
                                     D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	D3D11_DEPTH_STENCIL_DESC Desc = {};

	// 깊이 테스트 활성화 여부 
	Desc.DepthEnable = DepthEnable;
	// 쓰기 
	Desc.DepthWriteMask = DepthWriteMAsk;
	// 앞쪽 물체 렌더링 
	Desc.DepthFunc = DepthFunc;
	// 스탠실 테스트 활성화 
	Desc.StencilEnable = StencilEnable;
	// 스텐실 읽기
	Desc.StencilReadMask = StencilReadMask;
	// 스탠실 쓰기 
	Desc.StencilWriteMask = StencilWriteMask;
	Desc.FrontFace        = FrontFace;
	Desc.BackFace         = BackFace;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilState(&Desc,
		           (ID3D11DepthStencilState**)&m_State)))
		return false;

	return true;
}

void CDepthStencilState::SetState()
{
	CDevice::GetInst()->GetContext()->
	                    OMGetDepthStencilState((ID3D11DepthStencilState**)&m_PrevState, &m_PrevStencilRef);
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState(static_cast<ID3D11DepthStencilState*>(m_State),
	                                                         m_StencilRef);
}

void CDepthStencilState::ResetState()
{
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState(static_cast<ID3D11DepthStencilState*>(m_PrevState),
	                                                         m_PrevStencilRef);
	SAFE_RELEASE(m_PrevState);
}
