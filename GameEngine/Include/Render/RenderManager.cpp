
#include "RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/SceneComponent.h"
#include "RenderStateManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "DepthStencilState.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	m_RenderStateManager(nullptr),
	m_Standard2DCBuffer(nullptr)
{
}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE(m_Standard2DCBuffer);
	SAFE_DELETE(m_RenderStateManager);

	auto iter      = m_RenderLayerList.begin();
	auto iterEnd = m_RenderLayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(*iter);
	}
	m_RenderLayerList.clear();
}

void CRenderManager::AddRenderList(CSceneComponent* Component)
{
	RenderLayer* Layer = nullptr;
	
	auto iter = m_RenderLayerList.begin();
	auto iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Component->GetLayerName())
		{
			Layer = (*iter);
			break;
		}
	}
	if (!Layer)
		return;

	if (Layer->RenderCount == (int)Layer->RenderList.size())
		Layer->RenderList.resize(Layer->RenderCount * 2);

	Layer->RenderList[Layer->RenderCount] = Component;
	++Layer->RenderCount;
}

void CRenderManager::CreateLayer(const std::string& Name, int Priority)
{
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = Name;
	Layer->LayerPriority = Priority;

	m_RenderLayerList.push_back(Layer);

	// sort 다시 해두기
	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

void CRenderManager::SetLayerPriority(const std::string& Name, int Priority)
{
	auto iter = m_RenderLayerList.begin();
	auto iterEnd = m_RenderLayerList.end();
	
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->LayerPriority = Priority;
			break;
		}
	}
	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;

	m_RenderStateManager->Init();

	m_Standard2DCBuffer = new CStandard2DConstantBuffer;
	m_Standard2DCBuffer->Init();

	// 기본 레이어 세팅해두기
	RenderLayer* DefaultLayer = new RenderLayer;

	// 모든 Scene Component 들은, 생성시 Default 를 m_LayerName으로 들고 있을 것이다
	// SceneComponent 입장에서는 PrevRender 에서 m_Render가 true 이면 AddRenderList를 통해 들어온다
	// Default 라는 이름의 m_LayerName을 가진 SceneComponent 들은, 지금 세팅한 Default Layer 에 들어오게 된다.
	DefaultLayer->Name = "Default";
	DefaultLayer->LayerPriority = 0;

	m_RenderLayerList.push_back(DefaultLayer);

	m_DepthDisable = FindRenderState("DepthDisable");

	return true;
}

void CRenderManager::Render()
{
	// DepthStencilState
	m_DepthDisable->SetState();

	// 먼저 모든 Layer 를 돌면서, Render Count를 0으로 세팅해둔다. 
	{
		auto iter = m_RenderLayerList.begin();
		auto iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->RenderCount = 0;
		}
	}

	// 아래 PrevRender 에서 실제 그릴 녀석들을 추가한다. 
	{
		auto	iter = m_ObjectList->begin();
		auto	iterEnd = m_ObjectList->end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->PrevRender();
		}
	}

	{
		auto iter = m_RenderLayerList.begin();
		auto iterEnd = m_RenderLayerList.end();
		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; i++)
			{
				(*iter)->RenderList[i]->Render();
			}
		}
	}

	{
		auto iter = m_RenderLayerList.begin();
		auto iterEnd = m_RenderLayerList.end();
		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; i++)
			{
				(*iter)->RenderList[i]->PostRender();
			}
		}
	}

	m_DepthDisable->ResetState();
}

bool CRenderManager::SortLayer(RenderLayer* Src, RenderLayer* End)
{
	// 제일 큰애가 뒤로
	// 가장 나중에 그려서, 맨 앞에 나오게 할 것이다. 
	return Src->LayerPriority < End->LayerPriority;
}

void CRenderManager::SetBlendFactor(const std::string& Name, float r, float g,
	float b, float a)
{
	m_RenderStateManager->SetBlendFactor(Name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& Name, bool BlendEnable,
	D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp, 
	D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha, 
	D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	m_RenderStateManager->AddBlendInfo(Name, BlendEnable, SrcBlend, DestBlend,
		BlendOp, SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& Name, 
	bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(Name, AlphaToCoverageEnable, IndependentBlendEnable);
}

CRenderState* CRenderManager::FindRenderState(const std::string& Name)
{
	return m_RenderStateManager->FindRenderState(Name);
}
