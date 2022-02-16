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
	class CIMGUIComboBox* m_TileShapeCombo;
	class CIMGUITextInput*   m_TileCountX;
	class CIMGUITextInput*   m_TileCountY;
	class CIMGUITextInput*   m_TileSizeX;
	class CIMGUITextInput*   m_TileSizeY;
	class CIMGUIComboBox* m_TileType;
	class CIMGUIComboBox* m_TileEdit;
	class CIMGUITextInput* m_TileFrameStartX;
	class CIMGUITextInput* m_TileFrameStartY;
	class CIMGUITextInput* m_TileFrameEndX;
	class CIMGUITextInput* m_TileFrameEndY;
private :
	class CTileMapComponent* m_TileMap;
	Tile_EditMode m_EditMode;
public :
	void SetTileEditMode (Tile_EditMode EditMode)
	{
		m_EditMode = EditMode;
	}
	void SetTileMap (class CTileMapComponent* TileMap)
	{
		m_TileMap = TileMap;
	}
public :
	Tile_EditMode GetTileEditMode () const
{
		return m_EditMode;
}
public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
private :
	void CreateTile();
	void SetDefaultFrame();
	void SetEditModeCallback(int Index, const char* Name);
};

