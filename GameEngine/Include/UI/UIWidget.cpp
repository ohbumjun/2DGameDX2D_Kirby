#include "UIWidget.h"
#include "../Component/CameraComponent.h"
#include "UIWindow.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"
#include "../Scene/CameraManager.h"
#include "../Scene/Scene.h"
#include "../Scene/ViewPort.h"

CUIWidget::CUIWidget() :
	m_Owner(nullptr),
	m_Start(false),
	m_CBuffer(nullptr),
	m_Shader(nullptr),
	m_Mesh(nullptr),
	m_ZOrder(0),
	m_Size(50.f, 50.f),
	m_MoueHovered(false),
	m_CollisionMouseEnable(true)

{
	SetTypeID<CUIWidget>();
}

CUIWidget::CUIWidget(const CUIWidget& Widget)
{
	*this = Widget;

	m_Owner = nullptr;
	m_RefCount = 0;

	m_MoueHovered = false;

	m_CBuffer = new CWidgetConstantBuffer;
	m_CBuffer->Init();
}

CUIWidget::~CUIWidget()
{
	SAFE_DELETE(m_CBuffer);
}

void CUIWidget::SetShader(const std::string& Name)
{
	m_Shader = m_Owner->GetViewPort()->GetScene()->GetResource()->FindShader(Name);
}

void CUIWidget::SetUseTexture(bool Use)
{
	m_CBuffer->SetUseTexture(Use);
}

bool CUIWidget::CollisionMouse(const Vector2& MousePos)
{
	if (!m_CollisionMouseEnable)
		return false;

	if (m_RenderPos.x <= MousePos.x &&
		MousePos.x <= m_RenderPos.x + m_Size.x &&
		m_RenderPos.y <= MousePos.y &&
		m_RenderPos.y + m_Size.y >= MousePos.y)
	{
		m_MoueHovered = true;
		return true;
	}
	m_MoueHovered = false;
	return false;

}

void CUIWidget::Start()
{
	m_Start = true;
}

bool CUIWidget::Init()
{
	m_Shader = m_Owner->GetViewPort()->GetScene()->GetResource()->FindShader("WidgetShader");
	m_Mesh = m_Owner->GetViewPort()->GetScene()->GetResource()->FindMesh("WidgetMesh");

	m_CBuffer = new CWidgetConstantBuffer;
	m_CBuffer->Init();

	return true;
}

void CUIWidget::Update(float DeltaTime)
{
	if (!m_Start)
		Start();
}

void CUIWidget::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();

	m_RenderPos = m_Pos;

	if (m_Owner)
		m_RenderPos += m_Owner->GetWindowPos();
}

void CUIWidget::Render()
{
	if (!m_Start)
		Start();

	Matrix matWorld, matProj, matWP;

	Matrix matScale, matRotate, matTranslate;

	matScale.Scaling(m_Size.x, m_Size.y, 1.f);
	matRotate.Rotation(0.f, 0.f, m_Angle);
	matTranslate.Translation(m_RenderPos.x, m_RenderPos.y, 1.f);

	CCameraComponent* UICamera = m_Owner->GetViewPort()->GetScene()->GetCameraManager()->GetCurrentCamera();

	matProj = UICamera->GetProjMatrix();
	matWorld = matScale * matRotate * matTranslate;
	matWP = matWorld * matProj;
	// View 변환은 적용하지 않는 것을 확인할 수 있다.
	matWP.Transpose();

	m_CBuffer->SetWP(matWP);
	m_CBuffer->SetTint(m_Tint);

	m_CBuffer->UpdateCBuffer();

	m_Shader->SetShader();

	m_Mesh->Render();

}

CUIWidget* CUIWidget::Clone()
{
	return new CUIWidget(*this);
}
