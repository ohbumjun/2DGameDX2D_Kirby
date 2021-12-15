
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

	// sort �ٽ� �صα�
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

	// �⺻ ���̾� �����صα�
	RenderLayer* DefaultLayer = new RenderLayer;

	// ��� Scene Component ����, ������ Default �� m_LayerName���� ��� ���� ���̴�
	// SceneComponent ���忡���� PrevRender ���� m_Render�� true �̸� AddRenderList�� ���� ���´�
	// Default ��� �̸��� m_LayerName�� ���� SceneComponent ����, ���� ������ Default Layer �� ������ �ȴ�.
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

	// ���� ��� Layer �� ���鼭, Render Count�� 0���� �����صд�. 
	{
		auto iter = m_RenderLayerList.begin();
		auto iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->RenderCount = 0;
		}
	}

	// �Ʒ� PrevRender ���� ���� �׸� �༮���� �߰��Ѵ�. 
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
	// ���� ū�ְ� �ڷ�
	// ���� ���߿� �׷���, �� �տ� ������ �� ���̴�. 
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
