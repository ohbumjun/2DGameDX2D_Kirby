#include "SpriteEditObject.h"
#include "Engine.h"
#include "IMGUIImage.h"
#include "IMGUIWidget.h"
#include "IMGUIWindow.h"
#include "Input.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"

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

bool CSpriteEditObject::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("SpriteEdit");

	SetRootComponent(m_Sprite);

	int       DefaultTextureIdx = 0;
	CTexture* DefaultTexture    = m_Sprite->GetTexture(DefaultTextureIdx);
	m_Sprite->SetRelativeScale(static_cast<float>(DefaultTexture->GetWidth()),
	                           static_cast<float>(DefaultTexture->GetHeight()), 1.f);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

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