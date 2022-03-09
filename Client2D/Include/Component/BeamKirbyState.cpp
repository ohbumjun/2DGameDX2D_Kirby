#include "BeamKirbyState.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"

CBeamKirbyState::CBeamKirbyState()
{}

CBeamKirbyState::CBeamKirbyState(const CBeamKirbyState& Kirby) : CKirbyState(Kirby)
{}

CBeamKirbyState::~CBeamKirbyState()
{}

void CBeamKirbyState::Attack()
{}

void CBeamKirbyState::FallDownAttack()
{}

void CBeamKirbyState::Start()
{
	CKirbyState::Start();

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);
}

bool CBeamKirbyState::Init()
{
	if (!CKirbyState::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Beam", TEXT("Kirby_Beam.anim"));

	SetAnimationInstance(AnimationInstance);

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);

	return true;
}

void CBeamKirbyState::Update(float DeltaTime)
{
	CKirbyState::Update(DeltaTime);
}

void CBeamKirbyState::PostUpdate(float DeltaTime)
{
	CKirbyState::PostUpdate(DeltaTime);
}

void CBeamKirbyState::PrevRender()
{
	CKirbyState::PrevRender();
}

void CBeamKirbyState::Render()
{
	CKirbyState::Render();
}

void CBeamKirbyState::PostRender()
{
	CKirbyState::PostRender();
}

CBeamKirbyState* CBeamKirbyState::Clone()
{
	return new CBeamKirbyState(*this);
}
