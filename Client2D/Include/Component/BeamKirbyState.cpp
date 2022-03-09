#include "BeamKirbyState.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CBeamKirbyState::CBeamKirbyState()
{}

CBeamKirbyState::CBeamKirbyState(const CBeamKirbyState& Kirby)
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
}

bool CBeamKirbyState::Init()
{
	if (!CKirbyState::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Beam", TEXT("Kirby_Beam.anim"));

	SetAnimationInstance(AnimationInstance);

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
