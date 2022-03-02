#pragma once
#include "IMGUIWindow.h"
#include "../Editor.h"

class CTileMapWindow :
	public CIMGUIWindow
{
public:
	CTileMapWindow();
	virtual ~CTileMapWindow() override;
private: // todo : World Info
	class CIMGUIText* m_WorldSizeX;
	class CIMGUIText* m_WorldSizeY;
	class CIMGUITextInput* m_WorldSizeXInput;
	class CIMGUITextInput* m_WorldSizeYInput;
private: // todo : Tile Info
	// class CIMGUIComboBox* m_TileShapeCombo;
	class CIMGUITextInput* m_TileCountX;
	class CIMGUITextInput* m_TileCountY;
	class CIMGUITextInput* m_TileSizeX;
	class CIMGUITextInput* m_TileSizeY;
	class CIMGUIComboBox* m_TileType;
private:
	class CIMGUIText* m_CurrentTileCountX;
	class CIMGUIText* m_CurrentTileCountY;
	class CIMGUIText* m_CurrentTileSizeX;
	class CIMGUIText* m_CurrentTileSizeY;
private: // todo :  Tile Base Info
	// Material�� �⺻������ �����صΰ� --> �ҷ��� Texture�� �ٲ㳢�� ������ ����� ���̴�.
	class CIMGUIButton* m_TileImageLoadButton;
	class CIMGUIImage* m_TileBaseImageSprite;
	class CIMGUIText* m_TileBaseScaleX;
	class CIMGUIText* m_TileBaseScaleY;
	class CIMGUITextInput* m_TileBaseScaleXInput;
	class CIMGUITextInput* m_TileBaseScaleYInput;
private: // todo :  BackGround Info
	class CIMGUIButton* m_BackGroundImageLoadButton;
	class CIMGUIImage* m_BackGroundImageSprite;
	class CIMGUIText* m_BackImgScaleX;
	class CIMGUIText* m_BackImgScaleY;
	class CIMGUIText* m_BaseImgScrollRatio;
	class CIMGUITextInput* m_BaseImgScrollRatioInput;
private:
	// class CTileMapComponent* m_TileMap;
	// Tile_EditMode m_EditMode; --> ������ ���⼭�� Type�� ������ ���̴�.
	class CTileEmptyComponent* m_TileMapEmpty;
	class CBackGroundComponent* m_BackGround;
public:
	/*
	void SetTileEditMode (Tile_EditMode EditMode)
	{
		m_EditMode = EditMode;
	}
	*/
	void SetTileMap(class CTileEmptyComponent* TileMap);
public:
	/*
	Tile_EditMode GetTileEditMode () const
{
		return m_EditMode;
}
*/
public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
private:
	void CreateTile();
	void SetEditModeCallback(int Index, const char* Name);
	void SetTileTypeCallback(int Index, const char* Name);
private:
	void SetTileMapSizeToTileBaseImage();
private:
	void TileBaseImageLoadButton();
	void SetTileBaseImageScale();
};
