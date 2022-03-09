#include "FightKirbyState.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"

CFightKirbyState::CFightKirbyState()
{}

CFightKirbyState::CFightKirbyState(const CFightKirbyState& Kirby) : CKirbyState(Kirby)
{}

CFightKirbyState::~CFightKirbyState()
{}

void CFightKirbyState::Attack()
{}

void CFightKirbyState::FallDownAttack()
{}

void CFightKirbyState::Start()
{
	CKirbyState::Start();

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);
}

bool CFightKirbyState::Init()
{
	if (!CKirbyState::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Fight", TEXT("Kirby_Fight.anim"));

	SetAnimationInstance(AnimationInstance);

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);

	return true;
}

void CFightKirbyState::Update(float DeltaTime)
{
	CKirbyState::Update(DeltaTime);
}

void CFightKirbyState::PostUpdate(float DeltaTime)
{
	CKirbyState::PostUpdate(DeltaTime);
}

void CFightKirbyState::PrevRender()
{
	CKirbyState::PrevRender();
}

void CFightKirbyState::Render()
{
	CKirbyState::Render();
}

void CFightKirbyState::PostRender()
{
	CKirbyState::PostRender();
}

CFightKirbyState* CFightKirbyState::Clone()
{
	return new CFightKirbyState(*this);
}
