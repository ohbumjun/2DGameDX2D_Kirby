#include "SpriteEditObject.h"
#include "Component/SpriteComponent.h"
#include "Component/SceneComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"
#include "Engine.h"
#include "IMGUIWindow.h"
#include "IMGUIWidget.h"
#include "IMGUIImage.h"

CSpriteEditObject::CSpriteEditObject() :
	m_Distance(600.f),
	m_ObjMouseDown(false),
	m_EditWindow(nullptr)
{
}

CSpriteEditObject::CSpriteEditObject(const CSpriteEditObject& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("BulletSprite");
	m_Distance = obj.m_Distance;
}

CSpriteEditObject::~CSpriteEditObject()
{
}

bool CSpriteEditObject::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("SpriteEdit");

	SetRootComponent(m_Sprite);

	int DefaultTextureIdx = 0;
	CTexture* DefaultTexture = m_Sprite->GetTexture(DefaultTextureIdx);
	m_Sprite->SetRelativeScale((float)DefaultTexture->GetWidth(), (float)DefaultTexture->GetHeight(), 1.f);

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

void CSpriteEditObject::UpdateClickInfo()
{
	bool MouseDown = CInput::GetInst()->GetLMouseDown();
	bool MouseUp     = CInput::GetInst()->GetLMouseUp();
	bool MousePush     = CInput::GetInst()->GetLMousePush();

	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	float MousePosX = MousePos.x;
	float MousePosY = CEngine::GetInst()->GetResolution().Height - MousePos.y; // 좌 하단 기준, 마우스 높이 

	Vector3 ObjectStPos = m_RootComponent->GetWorldPos();
	Vector3 ObjectEdPos = ObjectStPos + m_RootComponent->GetWorldScale();
	
	// Down
	if (MouseDown && !m_ObjMouseDown)
	{
		// 범위 안에 들어올 때만 
		if (ObjectStPos.x <= MousePosX &&
			MousePosX <= ObjectEdPos.x &&
			ObjectStPos.y <= MousePosY && 
			MousePosY <= ObjectEdPos.y)
		{
			m_ObjMouseDown = true;
			float EndYPos = (ObjectEdPos.y - MousePosY);
			m_ObjImageStartPos = Vector2(MousePosX, EndYPos);
		}
	}
	// drag 하다가 범위 벗어나면 다시 초기화 
	else if (MousePush)
	{
		// 범위 안에 들어올 때만 
		if (ObjectStPos.x > MousePosX ||
			MousePosX > ObjectEdPos.x ||
			ObjectStPos.y > MousePosY ||
			MousePosY > ObjectEdPos.y)
		{
			m_ObjMouseDown = false;
		}
	}
	else if (MouseUp && m_ObjMouseDown)
	{
		// 범위 벗어나면 x
		if (ObjectStPos.x > MousePosX ||
			MousePosX > ObjectEdPos.x ||
			ObjectStPos.y > MousePosY ||
			MousePosY > ObjectEdPos.y)
		{
			m_ObjMouseDown = false;
		}
		// 범위 안에 들어오면
		else
		{
			m_ObjMouseDown = false;
			float EndYPos = (ObjectEdPos.y - MousePosY);
			m_ObjImageEndPos = Vector2(MousePosX, EndYPos);;
			SetEditWindowTexture();
		}
	}
}

void CSpriteEditObject::SetEditWindowTexture()
{
	
	CIMGUIImage* SpriteSampled = dynamic_cast<CIMGUIImage*>(m_EditWindow->FindWidget("SpriteSampled"));
	std::string TextureName = m_Sprite->GetTextureName();
	SpriteSampled->SetTexture(TextureName);

	Vector2 SpriteSize = SpriteSampled->GetSize();
	float RatioX = SpriteSize.x / m_RootComponent->GetWorldScale().x;
	float RatioY = SpriteSize.y / m_RootComponent->GetWorldScale().y;

	SpriteSampled->SetImageStart(m_ObjImageStartPos.x * RatioX, m_ObjImageStartPos.y * RatioY);
	SpriteSampled->SetImageEnd(m_ObjImageEndPos.x * RatioX, m_ObjImageEndPos.y * RatioY);

	MessageBox(0, TEXT("bb"), TEXT("bb"), MB_OK);
}