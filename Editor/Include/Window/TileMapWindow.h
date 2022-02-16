#pragma once
#include "IMGUIWindow.h"

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
public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
};

