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
private: // todo : Tile Info
	// class CIMGUIComboBox* m_TileShapeCombo;
	class CIMGUITextInput* m_TileCountCol;
	class CIMGUITextInput* m_TileCountRow;
	class CIMGUITextInput* m_TileSizeCol;
	class CIMGUITextInput* m_TileSizeRow;
	class CIMGUIComboBox* m_TileType;
private:
	class CIMGUIText* m_CurrentTileCountCol;
	class CIMGUIText* m_CurrentTileCountRow;
	class CIMGUIText* m_CurrentTileSizeCol;
	class CIMGUIText* m_CurrentTileSizeRow;
private: // todo :  Tile Base Info
	// Material을 기본적으로 세팅해두고 --> 불러온 Texture만 바꿔끼는 개념을 사용할 것이다.
	class CIMGUIButton* m_TileImageLoadButton;
	class CIMGUIImage* m_TileBaseImageSprite;
	class CIMGUIText* m_TileBaseScaleX;
	class CIMGUIText* m_TileBaseScaleY;
	class CIMGUITextInput* m_TileBaseWPosXInput;
	class CIMGUITextInput* m_TileBaseWPosYInput;
	class CIMGUITextInput* m_TileBaseScaleXInput;
	class CIMGUITextInput* m_TileBaseScaleYInput;
private:
	// class CTileMapComponent* m_TileMap;
	// Tile_EditMode m_EditMode; --> 무조건 여기서는 Type만 적용할 것이다.
	class CTileEmptyComponent* m_TileMapEmpty;
	class CBackGroundComponent* m_BackGround;
public :
	class CTileEmptyComponent* GetTileMapEmpty() const
{
		return m_TileMapEmpty;
}
public:
	void SetTileMap(class CTileEmptyComponent* TileMap);
	void SetTileMapComponentEditMode(bool Enable);
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
	void SetTileBaseWorldPos();
	void SetTileBaseImageResolutionScale();

};
