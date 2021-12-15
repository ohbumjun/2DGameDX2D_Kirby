#pragma once

#include "RenderState.h"

class CBlendState :
    public CRenderState
{
    friend class CRenderStateManager;

protected:
    CBlendState();
    virtual ~CBlendState();

protected:
    std::vector<D3D11_RENDER_TARGET_BLEND_DESC> m_vecDesc;
    unsigned int    m_SampleMask;
    unsigned int    m_PrevSampleMask;
    float           m_BlendFactor[4];
    float           m_PrevBlendFactor[4];

public:
    void SetBlendFactor(float r, float g, float b, float a)
    {
        m_BlendFactor[0] = a;
        m_BlendFactor[1] = r;
        m_BlendFactor[2] = g;
        m_BlendFactor[3] = b;
    }

public:
    void AddBlendInfo(bool BlendEnable = true, D3D11_BLEND SrcBlend = D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND DestBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD,
        D3D11_BLEND SrcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND DestBlendAlpha = D3D11_BLEND_ZERO,
        D3D11_BLEND_OP BlendOpAlpha = D3D11_BLEND_OP_ADD,
        UINT8 RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
    bool CreateState(bool AlphaToCoverageEnable, bool IndependentBlendEnable);

public:
    virtual void SetState();
    virtual void ResetState();
};

/*
Out Merger State 는, 픽셀 셰이더에서 출력된 픽셀값을 렌더 타겟에 쓰는 스테이지이다.
- Blend State 의 설정에 따라, 최종적으로 쓰여지는 값을 계산하고
- Depth Stencil State에 따라, 픽셀값을 쓸것인가를 최종적으로 판정한다.

(리소스)
-> 텍스쳐 리소스
-> 렌더 타겟 뷰 + 깊이 스탠실 뷰
ID3D11RenderTargetView + ID3D11DepthStencilView
-> ID3D11DeviceContext::OMSetRenderTarget()

+

(출력 관련 요소)
1) BlendState
-> D3D11_BLEND_DESC 구조체
-> ID3D11DeviceContext::CreateBlendState() : 블렌드 스테이트 오브젝트 생성
-> ID3D11BlendState * 인터페이스 얻기 == 혼합 상태 설정 Control하는 인터페이스
-> ID3D11DeviceContext::OMSetBlendState() : ID3D11BlendState 에 세팅된 설정값 파이프라인에 세팅하기 

1) DepthStencilState
->D3D11_Depth_Stencil_DESC 구조체
->ID3D11DeviceContext::CreateDepthStencilState() : 깊이/스탠실  스테이트 오브젝트 생성
->ID3D11DepthStencilState* 인터페이스 얻기 == 깊이 / 스탠실 정보  Control하는 인터페이스
->ID3D11DeviceContext::OMSetBlendState() : ID3D11DepthStencilState 에 세팅된 설정값 파이프라인에 세팅하기
*/
