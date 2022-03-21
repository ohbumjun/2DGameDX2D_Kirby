#include "WaterFloat1.h"
#include "../Window/TileMapWindow.h"
#include "../EditorManager.h"
#include "Component/TileEmptyComponent.h"
#include "Component/Tile.h"
#include "Scene/Scene.h"

CWaterFloat1::CWaterFloat1()
{
	SetTypeID<CWaterFloat1>();
}

CWaterFloat1::CWaterFloat1(const CWaterFloat1& Monster)
{}

CWaterFloat1::~CWaterFloat1()
{}

void CWaterFloat1::Start()
{
	CGameObject::Start();
}

bool CWaterFloat1::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("ItemSprite");
	SetRootComponent(m_Sprite);
	SetWorldScale(70.f, 70.f, 1.f); //
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"EffectFloat1Water", TEXT("Effect_Float1_Water.anim"));

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);

	return true;
}

void CWaterFloat1::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CWaterFloat1::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
