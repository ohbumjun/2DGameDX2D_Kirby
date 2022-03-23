#include "WaterFloat2_2.h"
#include "../Window/TileMapWindow.h"
#include "../EditorManager.h"
#include "Component/TileEmptyComponent.h"
#include "Component/Tile.h"
#include "Scene/Scene.h"

CWaterFloat2_2::CWaterFloat2_2()
{
	SetTypeID<CWaterFloat2_2>();
}

CWaterFloat2_2::CWaterFloat2_2(const CWaterFloat2_2& Monster)
{}

CWaterFloat2_2::~CWaterFloat2_2()
{}

void CWaterFloat2_2::Start()
{
	CGameObject::Start();
}

bool CWaterFloat2_2::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("ItemSprite");
	SetRootComponent(m_Sprite);
	SetWorldScale(70.f, 70.f, 1.f); //
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"EffectFloat2_2Water", TEXT("Effect_Float2_2_Water.anim"));

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);

	return true;
}

void CWaterFloat2_2::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CWaterFloat2_2::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
