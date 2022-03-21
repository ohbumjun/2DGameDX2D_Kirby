#include "KirbyState.h"

CKirbyState::CKirbyState() :
	m_ExtraAttackAbility(0.f)
{}

CKirbyState::CKirbyState(const CKirbyState& Kirby) : CSpriteComponent(Kirby)
{}

CKirbyState::~CKirbyState()
{}

void CKirbyState::Start()
{
	CSpriteComponent::Start();
}

bool CKirbyState::Init()
{
	if (!CSpriteComponent::Init())
		return false;

	return true;
}

void CKirbyState::Update(float DeltaTime)
{
	CSpriteComponent::Update(DeltaTime);
}

void CKirbyState::PostUpdate(float DeltaTime)
{
	CSpriteComponent::PostUpdate(DeltaTime);
}

void CKirbyState::PrevRender()
{
	CSpriteComponent::PrevRender();
}

void CKirbyState::Render()
{
	CSpriteComponent::Render();
}

void CKirbyState::PostRender()
{
	CSpriteComponent::PostRender();
}
