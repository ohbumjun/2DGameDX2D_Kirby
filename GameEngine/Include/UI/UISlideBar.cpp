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

	// Mouse �� Move �� �������� SlideBar�� �̵���Ų��
	if (m_MouseHovered)
	{
		bool MouseDown = CInput::GetInst()->GetLMouseDown();
		bool MouseUp = CInput::GetInst()->GetLMouseUp();

		// ���� ���� ��
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

	// m_MousePush ���, Mouse �̵��� ��ŭ �̵���Ű��
	if (m_MousePush)
	{
		Vector2 MouseMove = CInput::GetInst()->GetMouseMove();
		m_Pos.x += MouseMove.x;
	}
	
}

void CUISlideBar::PostUpdate(float DeltaTime)
{
	CUIWidget::PostUpdate(DeltaTime);

	// BGLineMesh�� Pos�� �����Ѵ�.
}

void CUISlideBar::Render()
{
	CUIWidget::Render();
}

CUISlideBar* CUISlideBar::Clone()
{
	return new CUISlideBar(*this);
}
