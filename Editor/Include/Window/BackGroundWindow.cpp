#include "BackGroundWindow.h"
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "Component/BackGroundComponent.h"
#include "../EditorManager.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

CBackGroundWindow::CBackGroundWindow()
{}

CBackGroundWindow::~CBackGroundWindow()
{}

void CBackGroundWindow::SetBackGround(CBackGroundComponent* BackGround)
{
	if (m_BackGround)
		return;

	// 2) BackMaterial 세팅 
	std::string BackMaterialName = BackGround->GetBackGroundMaterial()->GetName();

	m_BackGround = BackGround;

	if (!m_BackGround->GetBackGroundMaterial()->EmptyTexture())
	{
		char TextureWidth[MAX_PATH] = {};
		sprintf_s(TextureWidth, "%.1f", (float)m_BackGround->GetBackGroundMaterial()->GetTextureWidth());
		m_BackImgScaleX->SetText(TextureWidth);

		char TextureHeight[MAX_PATH] = {};
		sprintf_s(TextureHeight, "%.1f", (float)m_BackGround->GetBackGroundMaterial()->GetTextureHeight());
		m_BackImgScaleY->SetText(TextureHeight);

		char ScrollRatio[MAX_PATH] = {};
		sprintf_s(ScrollRatio, "%.1f", 100.f);
		m_BaseImgScrollRatio->SetText(ScrollRatio);

		m_BaseImgScrollRatioInput->SetFloat(100.f);

		m_BackGroundImageSprite->SetTexture(m_BackGround->GetBackGroundMaterial()->GetTexture());
		m_BackGroundImageSprite->SetImageStart(Vector2(0.f, 0.f));
		m_BackGroundImageSprite->SetImageEnd(Vector2((float)m_BackGround->GetBackGroundMaterial()->GetTextureWidth(),
			(float)m_BackGround->GetBackGroundMaterial()->GetTextureHeight()));

	}
}

bool CBackGroundWindow::Init()
{
	if (!CIMGUIWindow::Init())
		return false;

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("Back Image Info", 400.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Back Img", 195.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("Back Img Load", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	m_BackGroundImageSprite = AddWidget<CIMGUIImage>("BackGround Image", 195.f, 195.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	m_BackGroundImageLoadButton = AddWidget<CIMGUIButton>("Load Back Img", 100.f, 30.f);
	m_BackGroundImageLoadButton->SetClickCallback(this, &CBackGroundWindow::BackGroundImageLoadButton);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Current Scale", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_BackImgScaleX = AddWidget<CIMGUIText>("Back X");
	m_BackImgScaleX->SetSize(80.f, 40.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(230.f);

	m_BackImgScaleY = AddWidget<CIMGUIText>("Back X");
	m_BackImgScaleY->SetSize(80.f, 40.f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Current Ratio", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_BaseImgScrollRatio = AddWidget<CIMGUIText>("ScrollRatio");
	m_BaseImgScrollRatio->SetSize(80.f, 40.f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Set Ratio", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_BaseImgScrollRatioInput = AddWidget<CIMGUITextInput>("Scroll Ratio Input");
	m_BaseImgScrollRatioInput->SetSize(80.f, 40.f);
	m_BaseImgScrollRatioInput->SetInt(100);
	m_BaseImgScrollRatioInput->SetHideName(true);
	m_BaseImgScrollRatioInput->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	CIMGUIButton* Button = AddWidget<CIMGUIButton>("Change", 100.f, 30.f);
	Button->SetClickCallback<CBackGroundWindow>(this, &CBackGroundWindow::SetBackGroundScrollRatio);


	// BackGround Image Info 세팅 
	char BackSizeX[MAX_PATH] = {};
	sprintf_s(BackSizeX, "%.1f", 0.f);
	m_BackImgScaleX->SetText(BackSizeX);

	char BackSizeY[MAX_PATH] = {};
	sprintf_s(BackSizeY, "%.1f", 00.f);
	m_BackImgScaleY->SetText(BackSizeY);

	// Scroll Ratio 세팅
	m_BaseImgScrollRatioInput->SetFloat(0.f);


	return true;
}

void CBackGroundWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CBackGroundWindow::SetEditModeCallback(int Index, const char* Name)
{
	CEditorManager::GetInst()->SetEditMode(EditMode::Back);
}

void CBackGroundWindow::BackGroundImageLoadButton()
{
	if (CEditorManager::GetInst()->GetEditMode() != EditMode::Back)
		return;

	TCHAR FilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};

	OpenFile.lStructSize = sizeof(OpenFile);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0DDSFILE\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*jpg\0JPEGFile\0*.jpeg\0BMPFile\0*bmp");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		// File 이름 뽑아내기
		TCHAR FileName[MAX_PATH] = {};

		_wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, MAX_PATH, nullptr, 0);

		char ConvertFileName[MAX_PATH] = {};

		int  Length = WideCharToMultiByte(CP_ACP, 0, FileName, -1, nullptr, 0, nullptr, nullptr);

		WideCharToMultiByte(CP_ACP, 0, FileName, -1, ConvertFileName, Length, nullptr, nullptr);

		if (!m_BackGround)
			return;

		// BackMaterial 이 만들어져 있지 않다면, 만든다.
		if (!m_BackGround->GetBackGroundMaterial())
		{
			CSceneManager::GetInst()->GetScene()->GetResource()->CreateMaterial<CMaterial>("BackMaterial");

			CMaterial* BackMaterial = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("BackMaterial");

			BackMaterial->SetShader("Mesh2DShader");

			m_BackGround->SetBackGroundMaterial(BackMaterial);
		}

		if (m_BackGround->GetBackGroundMaterial()->EmptyTexture())
		{
			m_BackGround->GetBackGroundMaterial()->AddTextureFullPath(0, (int)Buffer_Shader_Type::Pixel, ConvertFileName, FilePath);
		}
		else
		{
			m_BackGround->GetBackGroundMaterial()->SetTextureFullPath(0, 0, (int)Buffer_Shader_Type::Pixel, ConvertFileName, FilePath);
		}

		m_BackGround->SetWorldScale((float)m_BackGround->GetBackGroundMaterial()->GetTextureWidth(),
			(float)m_BackGround->GetBackGroundMaterial()->GetTextureHeight(), 1.f);

		char BackImgWidth[MAX_PATH] = {};
		sprintf_s(BackImgWidth, "%1.f", (float)m_BackGround->GetBackGroundMaterial()->GetTextureWidth());
		m_BackImgScaleX->SetText(BackImgWidth);

		char BackImgHeight[MAX_PATH] = {};
		sprintf_s(BackImgHeight, "%1.f", (float)m_BackGround->GetBackGroundMaterial()->GetTextureHeight());
		m_BackImgScaleY->SetText(BackImgHeight);

		m_BackGroundImageSprite->SetTexture(m_BackGround->GetBackGroundMaterial()->GetTexture());
		m_BackGroundImageSprite->SetImageStart(Vector2(0.f, 0.f));
		m_BackGroundImageSprite->SetImageEnd(Vector2((float)m_BackGround->GetBackGroundMaterial()->GetTextureWidth(),
			(float)m_BackGround->GetBackGroundMaterial()->GetTextureHeight()));
	}
}

void CBackGroundWindow::SetBackGroundScrollRatio()
{
	if (m_BaseImgScrollRatioInput->FloatEmpty())
		return;

	char Ratio[MAX_PATH] = {};
	sprintf_s(Ratio, "%1.f", m_BaseImgScrollRatioInput->GetValueFloat());
	m_BaseImgScrollRatio->SetText(Ratio);
}
