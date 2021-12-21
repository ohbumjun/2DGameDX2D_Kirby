#include "DetailWindow.h"
#include "IMGUILabel.h"
#include "IMGUISameLine.h"
#include "IMGUITextInput.h"

CDetailWindow::CDetailWindow()
{
}

CDetailWindow::~CDetailWindow()
{
}

bool CDetailWindow::Init()
{
	CIMGUIWindow::Init();

	// ===============================================
	CIMGUILabel* Label = AddWidget<CIMGUILabel>("Pos", 40.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	CIMGUITextInput* Input = AddWidget<CIMGUITextInput>("PosX", 40.f, 30.f);
	Input->SetHideName(true);
	Input->SetCallback(this, &CDetailWindow::SetPositionXCallback);
	Input->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Input = AddWidget<CIMGUITextInput>("PosY", 40.f, 30.f);
	Input->SetHideName(true);
	Input->SetCallback(this, &CDetailWindow::SetPositionYCallback);
	Input->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Z", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Input = AddWidget<CIMGUITextInput>("PosZ", 40.f, 30.f);
	Input->SetHideName(true);
	Input->SetCallback(this, &CDetailWindow::SetPositionZCallback);
	Input->SetTextType(ImGuiText_Type::Float);

	// ===============================================
	Label = AddWidget<CIMGUILabel>("Rot", 40.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Input = AddWidget<CIMGUITextInput>("RotX", 40.f, 30.f);
	Input->SetHideName(true);
	Input->SetCallback(this, &CDetailWindow::SetRotationXCallback);
	Input->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Input = AddWidget<CIMGUITextInput>("RotY", 40.f, 30.f);
	Input->SetHideName(true);
	Input->SetCallback(this, &CDetailWindow::SetRotationYCallback);
	Input->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Input = AddWidget<CIMGUITextInput>("RotZ", 40.f, 30.f);
	Input->SetHideName(true);
	Input->SetCallback(this, &CDetailWindow::SetRotationZCallback);
	Input->SetTextType(ImGuiText_Type::Float);

	// ==============================================

	Label = AddWidget<CIMGUILabel>("Scale", 40.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Input = AddWidget<CIMGUITextInput>("ScaleX", 40.f, 30.f);
	Input->SetHideName(true);
	Input->SetCallback(this, &CDetailWindow::SetScalingXCallback);
	Input->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Input = AddWidget<CIMGUITextInput>("ScaleY", 40.f, 30.f);
	Input->SetHideName(true);
	Input->SetCallback(this, &CDetailWindow::SetScalingYCallback);
	Input->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Input = AddWidget<CIMGUITextInput>("ScaleZ", 40.f, 30.f);
	Input->SetHideName(true);
	Input->SetCallback(this, &CDetailWindow::SetScalingZCallback);
	Input->SetTextType(ImGuiText_Type::Float);

	return true;
}

void CDetailWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CDetailWindow::SetPositionXCallback()
{
}

void CDetailWindow::SetPositionYCallback()
{
}

void CDetailWindow::SetPositionZCallback()
{
}

void CDetailWindow::SetRotationXCallback()
{
}

void CDetailWindow::SetRotationYCallback()
{
}

void CDetailWindow::SetRotationZCallback()
{
}

void CDetailWindow::SetScalingXCallback()
{
}

void CDetailWindow::SetScalingYCallback()
{
}

void CDetailWindow::SetScalingZCallback()
{
}