#include "SpriteEditObject.h"

#include <Scene/Scene.h>

#include "Engine.h"
#include "IMGUIImage.h"
#include "IMGUIWidget.h"
#include "IMGUIWindow.h"
#include "Input.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Resource/ResourceManager.h"

CSpriteEditObject::CSpriteEditObject() :
	m_Distance(600.f),
	m_EditWindow(nullptr)
{
}

CSpriteEditObject::CSpriteEditObject(const CSpriteEditObject& obj) :
	CGameObject(obj)
{
	m_Sprite   = static_cast<CSpriteComponent*>(FindComponent("SpriteEdit"));
	m_Distance = obj.m_Distance;
}

CSpriteEditObject::~CSpriteEditObject()
{
}

void CSpriteEditObject::SetReverse(bool Reverse)
{
	CTexture* CurrentTexture = m_Sprite->GetTexture();

	if (!CurrentTexture)
		return;

	m_ReverseMode = Reverse;

	if (Reverse)
		m_Sprite->GetAnimationInstance()->SetFrameData(Vector2(0.f, 0.f), Vector2(-1.f * (float)CurrentTexture->GetWidth(),  (float)CurrentTexture->GetHeight()));
	else
		m_Sprite->GetAnimationInstance()->SetFrameData(Vector2(0.f, 0.f), Vector2( (float)CurrentTexture->GetWidth(),  (float)CurrentTexture->GetHeight()));
}

bool CSpriteEditObject::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("SpriteEdit");

	SetRootComponent(m_Sprite);

	CTexture* DefaultTexture    = m_Sprite->GetTexture();
	m_Sprite->SetRelativeScale(static_cast<float>(DefaultTexture->GetWidth()),
	                           static_cast<float>(DefaultTexture->GetHeight()), 1.f);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	CAnimationSequence2D* AddedSequence = nullptr;
	if (m_Scene)
	{
		m_Scene->GetResource()->AddSequence2D("FightKirbySequence", DefaultTexture);
		AddedSequence = m_Scene->GetResource()->FindAnimationSequence2D("FightKirbySequence");
	}
	else
	{
		CResourceManager::GetInst()->AddSequence2D("FightKirbySequence", DefaultTexture);
		AddedSequence = CResourceManager::GetInst()->FindAnimationSequence2D("FightKirbySequence");
	}

	// todo : 이거 수정해야 한다.
	m_Sprite->GetAnimationInstance()->AddAnimation("SpriteEditObjAnim", "FightKirbySequence", AddedSequence);

	// Frame 추가
	CAnimationSequence2DData* Animation = m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("SpriteEditObjAnim");
	Animation->AddFrame(Vector2(0.f, 0.f), Vector2((float)DefaultTexture->GetWidth(), (float)DefaultTexture->GetHeight()));

	return true;
}

void CSpriteEditObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	// UpdateClickInfo();
}

void CSpriteEditObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CSpriteEditObject* CSpriteEditObject::Clone()
{
	return new CSpriteEditObject(*this);
}

/*
void CSpriteEditObject::SetEditWindowTexture()
{
	CIMGUIImage* SpriteSampled = dynamic_cast<CIMGUIImage*>(m_EditWindow->FindWidget("SpriteSampled"));
	std::string  TextureName   = m_Sprite->GetTextureName();
	SpriteSampled->SetTexture(TextureName);

	Vector2 SpriteSize = SpriteSampled->GetSize();
	float   RatioX     = SpriteSize.x / m_RootComponent->GetWorldScale().x;
	float   RatioY     = SpriteSize.y / m_RootComponent->GetWorldScale().y;

	SpriteSampled->SetImageStart(m_ObjImageStartPos.x * RatioX, m_ObjImageStartPos.y * RatioY);
	SpriteSampled->SetImageEnd(m_ObjImageEndPos.x * RatioX, m_ObjImageEndPos.y * RatioY);

	MessageBox(nullptr, TEXT("bb"), TEXT("bb"), MB_OK);
}
*/