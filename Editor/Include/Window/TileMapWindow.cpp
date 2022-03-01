#include "TileMapWindow.h"
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "Component/TileEmptyComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../EditorManager.h"
#include "Input.h"
#include "PathManager.h"

CTileMapWindow::CTileMapWindow()
{}

CTileMapWindow::~CTileMapWindow()
{}

void CTileMapWindow::SetTileMap(CTileEmptyComponent* TileMap)
{
	m_TileMapEmpty = TileMap;


	m_CurrentTileCountX->SetInt(m_TileMapEmpty->GetTileCountX());
	m_CurrentTileCountY->SetInt(m_TileMapEmpty->GetTileCountY());

	m_CurrentTileSizeX->SetFloat(m_TileMapEmpty->GetTileEmptySize().x);
	m_CurrentTileSizeY->SetFloat(m_TileMapEmpty->GetTileEmptySize().y);

	// 1) Tile
	// 해당 TileMap에 사용된 Tile Texture의 Width, Height 를 IMGUIText에 세팅한다.
	/*
	if (!m_TileMap->GetTileMaterial()->EmptyTexture())
	{
		float TileTextureWidth = (float)m_TileMap->GetTileMaterial()->GetTextureWidth();
		float TileTextureHeight = (float)m_TileMap->GetTileMaterial()->GetTextureHeight();

		char TextureWidth[MAX_PATH] = {};
		sprintf_s(TextureWidth, "%.1f", TileTextureWidth);
		m_TextureWidth->SetText(TextureWidth);

		char TextureHeight[MAX_PATH] = {};
		sprintf_s(TextureHeight, "%.1f", TileTextureHeight);
		m_TextureHeight->SetText(TextureHeight);

		// Texture Sprite 에 세팅하기
		m_TextureImageSprite->SetTexture(m_TileMap->GetTileMaterial()->GetTexture());

		// Frame 정보가 있다면, TileImageSprite 도 세팅한다
		if (!m_TileFrameStartX->FloatEmpty())
		{
			m_TileImageSprite->SetTexture(m_TileMap->GetTileMaterial()->GetTexture());

			m_TileImageSprite->SetImageStart(Vector2(m_TileFrameStartX->GetValueFloat(), 
				m_TileFrameStartY->GetValueFloat()));

			m_TileImageSprite->SetImageEnd(Vector2(m_TileFrameEndX->GetValueFloat(), 
				m_TileFrameEndY->GetValueFloat()));
		}
	}
	*/

	// 2) BackMaterial 세팅 
	std::string BackMaterialName = TileMap->GetBackMaterial()->GetName();

	if (!m_TileMapEmpty->GetBackMaterial()->EmptyTexture())
	{
		// 여기서는 크기를 Texture의 크기가 아니라, 저장된 World Scale 크기로 세팅한다.
		// m_BackWorldScaleX->SetFloat((float)m_TileMap->GetBackMaterial()->GetTextureWidth());
		m_BackWorldScaleX->SetFloat((float)m_TileMapEmpty->GetWorldScale().x);
		// m_BackWorldScaleY->SetFloat((float)m_TileMap->GetBackMaterial()->GetTextureHeight());
		m_BackWorldScaleY->SetFloat((float)m_TileMapEmpty->GetWorldScale().y);

		m_BackImageSprite->SetTexture(m_TileMapEmpty->GetBackMaterial()->GetTexture());
		m_BackImageSprite->SetImageStart(Vector2(0.f, 0.f));
		m_BackImageSprite->SetImageEnd(Vector2((float)m_TileMapEmpty->GetBackMaterial()->GetTextureWidth(),
			(float)m_TileMapEmpty->GetBackMaterial()->GetTextureHeight()));

	}

}

bool CTileMapWindow::Init()
{
	if (!CIMGUIWindow::Init())
		return false;

	CIMGUIWindow::Init();

	// ==============================

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("TileMapInfo", 400.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	/*
	Label = AddWidget<CIMGUILabel>("TileMapShape", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileShapeCombo = AddWidget<CIMGUIComboBox>("TileShape", 100.f, 50.f);
	m_TileShapeCombo->SetHideName(true);
	m_TileShapeCombo->AddItem("사각형");
	m_TileShapeCombo->AddItem("마름모");
	*/
	// ==============================

	Label = AddWidget<CIMGUILabel>("Current Count", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	/*
	m_BackMaterialButton = AddWidget<CIMGUIButton>("Set Mtrl", 80.f, 30.f);
	m_BackMaterialButton->SetClickCallback(this, &CTileMapWindow::CreateBackMaterial);
	*/

	m_CurrentTileCountX = AddWidget<CIMGUITextInput>("CurCountX");
	m_CurrentTileCountX->SetSize(80.f, 40.f);
	m_CurrentTileCountX->SetInt(100);
	m_CurrentTileCountX->SetHideName(true);
	m_CurrentTileCountX->SetTextType(ImGuiText_Type::Int);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(230.f);

	m_CurrentTileCountY = AddWidget<CIMGUITextInput>("CurCountY");
	m_CurrentTileCountY->SetSize(80.f, 40.f);
	m_CurrentTileCountY->SetInt(100);
	m_CurrentTileCountY->SetHideName(true);
	m_CurrentTileCountY->SetTextType(ImGuiText_Type::Int);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Current Size", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_CurrentTileSizeX = AddWidget<CIMGUITextInput>("CurSizeX");
	m_CurrentTileSizeX->SetSize(80.f, 40.f);
	m_CurrentTileSizeX->SetFloat(100);
	m_CurrentTileSizeX->SetHideName(true);
	m_CurrentTileSizeX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(230.f);

	m_CurrentTileSizeY = AddWidget<CIMGUITextInput>("CurSizeY");
	m_CurrentTileSizeY->SetSize(80.f, 40.f);
	m_CurrentTileSizeY->SetFloat(100);
	m_CurrentTileSizeY->SetHideName(true);
	m_CurrentTileSizeY->SetTextType(ImGuiText_Type::Float);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);

	// ==============================

	Label = AddWidget<CIMGUILabel>("CountX", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileCountX = AddWidget<CIMGUITextInput>("TileCountX");
	m_TileCountX->SetSize(80.f, 40.f);
	m_TileCountX->SetInt(100);
	m_TileCountX->SetHideName(true);
	m_TileCountX->SetTextType(ImGuiText_Type::Int);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("CountY", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(320.f);

	m_TileCountY = AddWidget<CIMGUITextInput>("TileCountY");
	m_TileCountY->SetSize(80.f, 40.f);
	m_TileCountY->SetInt(100);
	m_TileCountY->SetHideName(true);
	m_TileCountY->SetTextType(ImGuiText_Type::Int);

	// ==============================

	Label = AddWidget<CIMGUILabel>("SizeX", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileSizeX = AddWidget<CIMGUITextInput>("SizeX");
	m_TileSizeX->SetSize(80.f, 40.f);
	m_TileSizeX->SetInt(100);
	m_TileSizeX->SetHideName(true);
	m_TileSizeX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("SizeY", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(320.f);

	m_TileSizeY = AddWidget<CIMGUITextInput>("SizeY");
	m_TileSizeY->SetSize(80.f, 40.f);
	m_TileSizeY->SetInt(100);
	m_TileSizeY->SetHideName(true);
	m_TileSizeY->SetTextType(ImGuiText_Type::Float);

	// ==============================

	CIMGUIButton* Button = AddWidget<CIMGUIButton>("CreateTileMap", 200.f, 30.f);
	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::CreateTile);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Button = AddWidget<CIMGUIButton>("ResizeTileMapToBack", 200.f, 30.f);
	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::SetTileMapSizeToBackGroundImage);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);

	// ==============================

	Label = AddWidget<CIMGUILabel>("TileEditInfo", 400.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("TileType", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileType = AddWidget<CIMGUIComboBox>("TileType", 120.f, 50.f);
	m_TileType->SetHideName(true);
	m_TileType->AddItem("Normal");
	m_TileType->AddItem("Wall");
	m_TileType->AddItem("Ceiling");
	m_TileType->AddItem("Water");
	m_TileType->SetSelectCallback<CTileMapWindow>(this, &CTileMapWindow::SetTileTypeCallback);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);

	// ==============================
	/*
	Label = AddWidget<CIMGUILabel>("TileEdit", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileEdit = AddWidget<CIMGUIComboBox>("TileEditMode", 120.f, 50.f);
	m_TileEdit->SetHideName(true);
	m_TileEdit->AddItem("Type");
	m_TileEdit->AddItem("Frame");
	m_TileEdit->SetSelectCallback<CTileMapWindow>(this, &CTileMapWindow::SetEditModeCallback);

	Label = AddWidget<CIMGUILabel>("TextureWidth", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TextureWidth = AddWidget<CIMGUIText>("TextureWidth");
	m_TextureWidth->SetSize(80.f, 40.f);
	m_TextureWidth->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("TextureHeight", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(320.f);

	m_TextureHeight = AddWidget<CIMGUIText>("TextureHeight");
	m_TextureHeight->SetSize(80.f, 40.f);
	m_TextureHeight->SetHideName(true);
	*/

	// ==============================
	/*
	Label = AddWidget<CIMGUILabel>("FrameStartX", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileFrameStartX = AddWidget<CIMGUITextInput>("FrameStartX");
	m_TileFrameStartX->SetSize(80.f, 40.f);
	m_TileFrameStartX->SetInt(100);
	m_TileFrameStartX->SetHideName(true);
	m_TileFrameStartX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("FrameStartY", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(320.f);

	m_TileFrameStartY = AddWidget<CIMGUITextInput>("FrameStartY");
	m_TileFrameStartY->SetSize(80.f, 40.f);
	m_TileFrameStartY->SetInt(100);
	m_TileFrameStartY->SetHideName(true);
	m_TileFrameStartY->SetTextType(ImGuiText_Type::Float);
	
	// ==============================

	Label = AddWidget<CIMGUILabel>("FrameEndX", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileFrameEndX = AddWidget<CIMGUITextInput>("FrameEndX");
	m_TileFrameEndX->SetSize(80.f, 40.f);
	m_TileFrameEndX->SetInt(100);
	m_TileFrameEndX->SetHideName(true);
	m_TileFrameEndX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("FrameEndY", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(320.f);

	m_TileFrameEndY = AddWidget<CIMGUITextInput>("FrameEndY");
	m_TileFrameEndY->SetSize(80.f, 40.f);
	m_TileFrameEndY->SetInt(100);
	m_TileFrameEndY->SetHideName(true);
	m_TileFrameEndY->SetTextType(ImGuiText_Type::Float);
	*/

	// ==============================
	/*
	Label = AddWidget<CIMGUILabel>("TextureImg", 195.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("TileImage", 195.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);
	*/

	// ==============================
	/*
	m_TextureImageSprite = AddWidget<CIMGUIImage>("Edit Tile Image", 195.f, 195.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	m_TileImageSprite = AddWidget<CIMGUIImage>("Edit Tile Image", 195.f, 195.f);
	*/
	// ==============================

	/*
	Button = AddWidget<CIMGUIButton>("SetDefaultFrame", 200.f, 30.f);
	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::SetDefaultFrame);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);
	*/

	// ==============================

	Label = AddWidget<CIMGUILabel>("BackGround Image Info", 400.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Back Img", 195.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("Texture Load", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	m_BackImageSprite = AddWidget<CIMGUIImage>("Edit Tile Image", 195.f, 195.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	m_BackImageLoadButton = AddWidget<CIMGUIButton>("Load BG", 100.f, 30.f);
	m_BackImageLoadButton->SetClickCallback(this, &CTileMapWindow::BackGroundImageLoadButton);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Back Scale", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	/*
	m_BackMaterialButton = AddWidget<CIMGUIButton>("Set Mtrl", 80.f, 30.f);
	m_BackMaterialButton->SetClickCallback(this, &CTileMapWindow::CreateBackMaterial);
	*/

	m_BackWorldScaleX = AddWidget<CIMGUITextInput>("X");
	m_BackWorldScaleX->SetSize(80.f, 40.f);
	m_BackWorldScaleX->SetInt(100);
	m_BackWorldScaleX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(230.f);

	m_BackWorldScaleY = AddWidget<CIMGUITextInput>("Y");
	m_BackWorldScaleY->SetSize(80.f, 40.f);
	m_BackWorldScaleY->SetInt(100);
	m_BackWorldScaleY->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(330.f);

	Button = AddWidget<CIMGUIButton>("Set Scale", 80.f, 30.f);
	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::SetBackGroundImageScale);

	// ==============================

	// Default Value 들 세팅
	m_TileCountX->SetInt(100);
	m_TileCountY->SetInt(100);
	m_TileSizeX->SetFloat(160.f);
	m_TileSizeY->SetFloat(80.f);

	m_CurrentTileCountX->SetInt(100);
	m_CurrentTileCountY->SetInt(100);

	m_CurrentTileSizeX->SetFloat(160.f);
	m_CurrentTileSizeY->SetFloat(80.f);

	/*
	m_TileFrameStartX->SetFloat(160.f);
	m_TileFrameStartY->SetFloat(80.f);
	m_TileFrameEndX->SetFloat(320.f);
	m_TileFrameEndY->SetFloat(160.f);
	*/

	m_BackWorldScaleX->SetFloat(0.f);
	m_BackWorldScaleY->SetFloat(0.f);

	return true;
}

void CTileMapWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	EditMode CurrentEditMode = CEditorManager::GetInst()->GetEditMode();


	if (CEditorManager::GetInst()->GetEditMode() == EditMode::Tile && m_TileMapEmpty)
	{
		m_TileMapEmpty->EnableEditMode(true);

		// Mouse를 눌렀다면 
		if (CEditorManager::GetInst()->GetMousePush())
		{
			Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

			CTileEmpty* Tile = m_TileMapEmpty->GetTileEmpty(Vector3(MouseWorldPos.x, MouseWorldPos.y, 0.f));

			if (!Tile) // 해당 위치에 타일이 존재하지 않는다면 처리 X
				return;

			int TypeIndex = m_TileType->GetSelectIndex();

			if (TypeIndex < 0 || TypeIndex >= (int)Tile_Type::End)
				return;

			Tile->SetTileType((Tile_Type)TypeIndex);

			/*
			switch (m_TileMapEmpty)
			{
			case Tile_EditMode::Type :
				{
				int TypeIndex = m_TileType->GetSelectIndex();

				if (TypeIndex < 0 || TypeIndex >= (int)Tile_Type::End)
					return;

				Tile->SetTileType((Tile_Type)TypeIndex);
				
				}
				break;
			case Tile_EditMode::Frame:
			{
				float FrameStartX = m_TileFrameStartX->GetValueFloat();
				float FrameStartY = m_TileFrameStartY->GetValueFloat();

				float FrameEndX = m_TileFrameEndX->GetValueFloat();
				float FrameEndY = m_TileFrameEndY->GetValueFloat();

				// 1) 이미지의 가로 세로 길이를 구해서, 범위 제한을 한다.
				// 2) 또한 실제 Input 에도 해당 내용으로 바꿔서 표시한다. --> 별도의 Text를 마련한다.
				// 어차피 TileMap 의 경우, 거꾸로 출력할 일이 없다.

				float TextureWidth  = (float)m_TileMap->GetTileMaterial()->GetTextureWidth();
				float TextureHeight = (float)m_TileMap->GetTileMaterial()->GetTextureHeight();

				if (FrameStartX < 0 || FrameStartX > TextureWidth)
					return;
				if (FrameStartY < 0 || FrameStartY > TextureHeight)
					return;
				if (FrameEndX < 0 || FrameEndX > TextureWidth)
					return;
				if (FrameEndY < 0 || FrameEndY > TextureHeight)
					return;

				Tile->SetFrameStart(FrameStartX, FrameStartY);
				Tile->SetFrameEnd(FrameEndX, FrameEndY);

			}
			break;
			}
			*/
		}
	}
}

void CTileMapWindow::CreateTile()
{
	if (!m_TileMapEmpty)
		return;

	// Tile Shape
	/*
	int TileShapeIndex = m_TileShapeCombo->GetSelectIndex();

	if (TileShapeIndex < 0 || TileShapeIndex >= (int)Tile_Shape::End)
		return;

	Tile_Shape Shape = (Tile_Shape)TileShapeIndex;
	*/

	// CountX, CountY
	int CountX = m_TileCountX->GetValueInt();
	int CountY = m_TileCountY->GetValueInt();

	if (CountX <= 0 || CountY <= 0)
		return;

	// Size
	float SizeX = m_TileSizeX->GetValueFloat();
	float SizeY = m_TileSizeY->GetValueFloat();

	if (SizeX <= 0 || SizeY <= 0)
		return;

	// 현재 정보 Update
	m_CurrentTileCountX->SetInt(CountX);
	m_CurrentTileCountY->SetInt(CountY);

	m_CurrentTileSizeX->SetFloat(SizeX);
	m_CurrentTileSizeY->SetFloat(SizeY);

	// Tile 생성
	// m_TileMapEmpty->CreateTile(Shape,CountX, CountY, Vector3(SizeX, SizeY, 1.f));
	m_TileMapEmpty->CreateTileEmpty(CountX, CountY, Vector3(SizeX, SizeY, 1.f));


	/*
	switch(Shape)
	{
	case Tile_Shape::Rect :
		{
			CSceneManager::GetInst()->GetScene()->GetResource()->LoadTexture("DefaultRectTile",
				TEXT("Floors.png"));

			Texture = CSceneManager::GetInst()->GetScene()->GetResource()->FindTexture("DefaultRectTile");
		}
		break;
	case Tile_Shape::Rhombus :
		{
			CSceneManager::GetInst()->GetScene()->GetResource()->LoadTexture("DefaultRhombusTile",
				TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor.png"));

			Texture = CSceneManager::GetInst()->GetScene()->GetResource()->FindTexture("DefaultRhombusTile");
		}
		break;
	}
	*/

	// Sprite Image에 세팅하기m_TextureImageSprite;
	// m_TextureImageSprite->SetTexture(Texture);
	// class CIMGUIImage* m_TileImageSprite;

	/*
	CMaterial* Material = m_TileMapEmpty->GetTileMaterial();

	if (Material->EmptyTexture())
		Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "TileTexture", Texture);
	else
		Material->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "TileTexture", Texture);

	// TileMapComponent 의 상수버퍼에 전체 ImageSize를 지정해주도록 세팅한다.
	m_TileMap->SetImageSizeToCBuffer();

	// 해당 TileMap에 사용된 Tile Texture의 Width, Height 를 IMGUIText에 세팅한다.
	float TileTextureWidth  = (float)Material->GetTextureWidth();
	float TileTextureHeight = (float)Material->GetTextureHeight();

	char TextureWidth[MAX_PATH] = {};
	sprintf_s(TextureWidth, "%.1f", TileTextureWidth);
	m_TextureWidth->SetText(TextureWidth);

	char TextureHeight[MAX_PATH] = {};
	sprintf_s(TextureHeight, "%.1f", TileTextureHeight);
	m_TextureHeight->SetText(TextureWidth);

	// Default Frame 세팅하기
	if (!m_TileFrameStartX->FloatEmpty())
	{
		float StartFrameX = m_TileFrameStartX->GetValueFloat();
		float StartFrameY = m_TileFrameStartY->GetValueFloat();

		float EndFrameX = m_TileFrameEndX->GetValueFloat();
		float EndFrameY = m_TileFrameEndY->GetValueFloat();

		if (StartFrameX < 0.f || StartFrameY < 0.f || EndFrameX < 0.f || EndFrameY < 0.f)
			return;

		m_TileMap->SetTileDefaultFrame(StartFrameX, StartFrameY, EndFrameX, EndFrameY);
	}
	*/

}

void CTileMapWindow::SetDefaultFrame()
{
	if (!m_TileMapEmpty)
		return;

	/*
	float StartFrameX = m_TileFrameStartX->GetValueFloat();
	float StartFrameY = m_TileFrameStartY->GetValueFloat();

	float EndFrameX = m_TileFrameEndX->GetValueFloat();
	float EndFrameY = m_TileFrameEndY->GetValueFloat();

	if (StartFrameX < 0.f || StartFrameY < 0.f || EndFrameX < 0.f || EndFrameY < 0.f)
		return;

	m_TileMap->SetTileDefaultFrame(StartFrameX, StartFrameY, EndFrameX, EndFrameY);

	// TileImage Sprite 세팅하기
	m_TileImageSprite->SetTexture(m_TileMap->GetTileMaterial()->GetTexture());

	m_TileImageSprite->SetImageStart(Vector2(StartFrameX, StartFrameY));

	m_TileImageSprite->SetImageEnd(Vector2(EndFrameX, EndFrameY));
	*/

}

void CTileMapWindow::SetEditModeCallback(int Index, const char* Name)
{
	// Editor 에 Tile Edit 이라고 세팅하기
	CEditorManager::GetInst()->SetEditMode(EditMode::Tile);

	// TileMapWindow 내에서 Tile 의 어떤 Mode를 수정할지를 세팅
	// m_EditMode = (Tile_EditMode)Index;
}

void CTileMapWindow::SetTileTypeCallback(int Index, const char* Name)
{}

void CTileMapWindow::SetTileMapSizeToBackGroundImage()
{
	if (!m_TileMapEmpty)
		return;

	if (!m_TileMapEmpty->GetBackMaterial() || m_TileMapEmpty->GetTileCount() == 0)
		return;

	Vector2 BackGroundSize = Vector2(m_TileMapEmpty->GetWorldScale().x, m_TileMapEmpty->GetWorldScale().y);

	float TileSizeX = BackGroundSize.x /(float) m_TileMapEmpty->GetTileCountX();
	float TileSizeY = BackGroundSize.y /(float) m_TileMapEmpty->GetTileCountY();

	// 현재 정보 Update
	m_CurrentTileSizeX->SetFloat(TileSizeX);
	m_CurrentTileSizeY->SetFloat(TileSizeY);

	m_TileMapEmpty->SetTileDefaultSize(TileSizeX, TileSizeY);
}

/*
void CTileMapWindow::TileMapSaveButton()
{
	if (!m_TileMapEmpty)
		return;

	TCHAR FilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0GameObject File\0*.gobj");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(OBJECT_PATH)->Path;

	if (GetSaveFileName(&OpenFile) != 0)
	{
		char ConvertFullPath[MAX_PATH] = {};

		int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, ConvertFullPath, Length, 0, 0);

		CGameObject* TileMapObject = m_TileMap->GetGameObject();

		TileMapObject->SaveFullPath(ConvertFullPath);

		std::string ExtraFolderName = ENGINE_OBJECT_PATH;

		const PathInfo* EngineSequenceFolder = CPathManager::GetInst()->FindPath(ExtraFolderName);

		// 파일 이름을 뽑아낸다.
		char SavedFileName[MAX_PATH] = {};
		char SavedExt[_MAX_EXT] = {};
		_splitpath_s(ConvertFullPath, nullptr, 0, nullptr, 0, SavedFileName, MAX_PATH, SavedExt, _MAX_EXT);

		// 최종 GameEngine 경로를 만든다.
		char SavedGameEnginePath[MAX_PATH] = {};
		strcpy_s(SavedGameEnginePath, EngineSequenceFolder->PathMultibyte);
		strcat_s(SavedGameEnginePath, SavedFileName);
		strcat_s(SavedGameEnginePath, SavedExt);

		// 현재 저장되는 경로와 다르다면, GameEngine 쪽에도 저장한다.
		if (strcmp(EngineSequenceFolder->PathMultibyte, ConvertFullPath) != 0)
		{
			TileMapObject->SaveFullPath(SavedGameEnginePath);
		}
	}
		
}

void CTileMapWindow::TileMapLoadButton()
{
	if (!m_TileMap)
		return;

	TCHAR LoadFilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.lpstrFile = LoadFilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0*.GameObject File\0*.gobj");
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(OBJECT_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char ConvertFullPath[MAX_PATH] = {};

		int Length = WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, 0, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, ConvertFullPath, Length, 0, 0);

		CGameObject* TileMapObject = m_TileMap->GetGameObject();

		TileMapObject->LoadFullPath(ConvertFullPath);
	}
}

/*
void CTileMapWindow::CreateBackMaterial()
{
	if (CEditorManager::GetInst()->GetEditMode() != EditMode::Tile)
		return;

	// Back Material 이라는 이름이 이미 만들어져 있는지 확인하기
	if (CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("BackMaterial"))
		return;

	// Back Material 을 하나 만들고
	CSceneManager::GetInst()->GetScene()->GetResource()->CreateMaterial<CMaterial>("BackMaterial");

	CMaterial* BackMaterial = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("BackMaterial");

	BackMaterial->SetShader("Mesh2DShader");
}
*/

void CTileMapWindow::BackGroundImageLoadButton()
{
	if (CEditorManager::GetInst()->GetEditMode() != EditMode::Tile)
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

		// FullPath 먼저 만들기

		// BackMaterial 이 만들어져 있지 않다면, 만든다.
		if (!m_TileMapEmpty->GetBackMaterial())
		{
			CSceneManager::GetInst()->GetScene()->GetResource()->CreateMaterial<CMaterial>("BackMaterial");

			CMaterial* BackMaterial = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("BackMaterial");

			BackMaterial->SetShader("Mesh2DShader");

			m_TileMapEmpty->SetBackMaterial(BackMaterial);
		}

		if (m_TileMapEmpty->GetBackMaterial()->EmptyTexture())
		{
			m_TileMapEmpty->GetBackMaterial()->AddTextureFullPath(0, (int)Buffer_Shader_Type::Pixel, ConvertFileName, FilePath);
		}
		else
		{
			m_TileMapEmpty->GetBackMaterial()->SetTextureFullPath(0, 0, (int)Buffer_Shader_Type::Pixel, ConvertFileName, FilePath);
		}

		m_TileMapEmpty->SetWorldScale((float)m_TileMapEmpty->GetBackMaterial()->GetTextureWidth(),
			(float)m_TileMapEmpty->GetBackMaterial()->GetTextureHeight(), 1.f);

		m_BackWorldScaleX->SetFloat((float)m_TileMapEmpty->GetBackMaterial()->GetTextureWidth());
		m_BackWorldScaleY->SetFloat((float)m_TileMapEmpty->GetBackMaterial()->GetTextureHeight());

		m_BackImageSprite->SetTexture(m_TileMapEmpty->GetBackMaterial()->GetTexture());
		m_BackImageSprite->SetImageStart(Vector2(0.f, 0.f));
		m_BackImageSprite->SetImageEnd(Vector2((float)m_TileMapEmpty->GetBackMaterial()->GetTextureWidth(),
			(float)m_TileMapEmpty->GetBackMaterial()->GetTextureHeight()));
	}
}

void CTileMapWindow::SetBackGroundImageScale()
{
	if (!m_TileMapEmpty)
		return;

	if (!m_BackWorldScaleX->FloatEmpty() && !m_BackWorldScaleY->FloatEmpty())
	{
		m_TileMapEmpty->SetWorldScale(m_BackWorldScaleX->GetValueFloat(), m_BackWorldScaleY->GetValueFloat(), 1.f);
	}
}

