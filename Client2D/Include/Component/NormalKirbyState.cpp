#include "NormalKirbyState.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"

CNormalKirbyState::CNormalKirbyState()
{}

CNormalKirbyState::CNormalKirbyState(const CNormalKirbyState& Kirby) : CKirbyState(Kirby)
{}

CNormalKirbyState::~CNormalKirbyState()
{}

void CNormalKirbyState::Attack()
{}

void CNormalKirbyState::FallDownAttack()
{}

void CNormalKirbyState::GoUpAttack()
{}

void CNormalKirbyState::Start()
{
	CKirbyState::Start();

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("RightSpitOut")->SetReverse(false);
	m_Animation->FindAnimationSequence2DData("LeftSpitOut")->SetReverse(false);
}

bool CNormalKirbyState::Init()
{
	if (!CKirbyState::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Normal", TEXT("Kirby_Normal.anim"), ENGINE_ANIMATION_PATH);

	SetAnimationInstance(AnimationInstance);


	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("RightSpitOut")->SetReverse(false);
	m_Animation->FindAnimationSequence2DData("LeftSpitOut")->SetReverse(false);

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
