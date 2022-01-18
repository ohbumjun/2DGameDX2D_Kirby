#include "UISlideBar.h"
#include "../Input.h"

CUISlideBar::CUISlideBar() :
	m_MousePush(false)
{
	SetTypeID<CUISlideBar>();
}

CUISlideBar::CUISlideBar(const CUISlideBar& SlideBar)
{}

CUISlideBar::~CUISlideBar()
{}

bool CUISlideBar::Init()
{
	if (!CUIWidget::Init())
		return false;

	return true;
}

void CUISlideBar::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);

	// Mouse 의 Move 에 반응시켜 SlideBar를 이동시킨다
	if (m_MouseHovered)
	{
		bool MouseDown = CInput::GetInst()->GetLMouseDown();
		bool MouseUp = CInput::GetInst()->GetLMouseUp();

		// 지금 누른 것
		if (MouseDown)
		{
			m_MousePush = true;
			m_Tint = Vector4(1.f, 0.f, 0.f, 1.f);
		}
		else if (MouseUp)
		{
			m_MousePush = false;
			m_Tint = Vector4(1.f, 1.f, 1.f, 1.f);
		}
	}
	else
	{
		m_MousePush = false;
		m_Tint = Vector4(1.f, 1.f, 1.f, 1.f);
	}

	// m_MousePush 라면, Mouse 이동량 만큼 이동시키기
	if (m_MousePush)
	{
		Vector2 MouseMove = CInput::GetInst()->GetMouseMove();
		m_Pos.x += MouseMove.x;
	}
	
}

void CUISlideBar::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);

	// BGLineMesh의 Pos를 세팅한다.
}

void CUISlideBar::Render()
{
	CUIWidget::Render();
}

CUISlideBar* CUISlideBar::Clone()
{
	return new CUISlideBar(*this);
}
