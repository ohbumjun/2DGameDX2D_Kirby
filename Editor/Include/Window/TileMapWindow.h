#pragma once
#include "IMGUIWindow.h"
#include "../Editor.h"

class CTileMapWindow :
    public CIMGUIWindow
{
public:
	CTileMapWindow();
	virtual ~CTileMapWindow() override;
private :
	// class CIMGUIComboBox* m_TileShapeCombo;
	class CIMGUITextInput*   m_TileCountX;
	class CIMGUITextInput*   m_TileCountY;
	class CIMGUITextInput*   m_TileSizeX;
	class CIMGUITextInput*   m_TileSizeY;
	class CIMGUIComboBox* m_TileType;
private :
	class CIMGUITextInput* m_CurrentTileCountX;
	class CIMGUITextInput* m_CurrentTileCountY;
	class CIMGUITextInput* m_CurrentTileSizeX;
	class CIMGUITextInput* m_CurrentTileSizeY;
	/*
	class CIMGUIComboBox* m_TileEdit;
	class CIMGUITextInput* m_TileFrameStartX;
	class CIMGUITextInput* m_TileFrameStartY;
	class CIMGUITextInput* m_TileFrameEndX;
	class CIMGUITextInput* m_TileFrameEndY;
	*/
private : // Sprite
	// class CIMGUIImage* m_TextureImageSprite;
	// class CIMGUIImage* m_TileImageSprite;

private :
	// class CIMGUIButton* m_TileMapSaveButton; --> �׳� SaveObject, Load Object �� ���� �����Ѵ�.
	// class CIMGUIButton* m_TileMapLoadButton;
private :
	// Material�� �⺻������ �����صΰ� --> �ҷ��� Texture�� �ٲ㳢�� ������ ����� ���̴�.
	class CIMGUIButton* m_BackMaterialButton;
	class CIMGUIButton* m_BackImageLoadButton;
	class CIMGUIImage* m_BackImageSprite;
private: // Back Material Size
	class CIMGUITextInput* m_BackWorldScaleX;
	class CIMGUITextInput* m_BackWorldScaleY;
private :
	// class CTileMapComponent* m_TileMap;
	// Tile_EditMode m_EditMode; --> ������ ���⼭�� Type�� ������ ���̴�.
	class CTileEmptyComponent* m_TileMapEmpty;
public :
	/*
	void SetTileEditMode (Tile_EditMode EditMode)
	{
		m_EditMode = EditMode;
	}
	*/
	void SetTileMap(class CTileEmptyComponent* TileMap);
public :
	/*
	Tile_EditMode GetTileEditMode () const
{
		return m_EditMode;
}
*/
public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
private :
	void CreateTile();
	void SetDefaultFrame();
	void SetEditModeCallback(int Index, const char* Name);
	void SetTileTypeCallback(int Index, const char* Name);
private :
	void SetTileMapSizeToBackGroundImage();
	// void TileMapSaveButton();
	// void TileMapLoadButton();
private :
	void BackGroundImageLoadButton();
	void SetBackGroundImageScale();

};

