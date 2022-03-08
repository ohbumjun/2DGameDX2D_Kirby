#include "NormalKirbyState.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CNormalKirbyState::CNormalKirbyState()
{}

CNormalKirbyState::CNormalKirbyState(const CNormalKirbyState& Kirby)
{}

CNormalKirbyState::~CNormalKirbyState()
{}

void CNormalKirbyState::Attack()
{}

void CNormalKirbyState::FallDownAttack()
{}

void CNormalKirbyState::Start()
{
	CKirbyState::Start();
}

bool CNormalKirbyState::Init()
{
	if (!CKirbyState::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Normal", TEXT("Kirby_Normal.anim"));

	SetAnimationInstance(AnimationInstance);

	return true;
}

void CNormalKirbyState::Update(float DeltaTime)
{
	CKirbyState::Update(DeltaTime);
}

void CNormalKirbyState::PostUpdate(float DeltaTime)
{
	CKirbyState::PostUpdate(DeltaTime);
}

void CNormalKirbyState::PrevRender()
{
	CKirbyState::PrevRender();
}

void CNormalKirbyState::Render()
{
	CKirbyState::Render();
}

void CNormalKirbyState::PostRender()
{
	CKirbyState::PostRender();
}

CNormalKirbyState* CNormalKirbyState::Clone()
{
	return new CNormalKirbyState(*this);
}
