#pragma once

#include "GameInfo.h"

enum class EditMode
{
	Sprite,
	Scene,
	Tile,
	Back,
	Line,
	CharacterCreate,
	CharacterEdit,
	LineCreate,
	LineEdit
};

class CEditorManager
{
private:
	EditMode           m_EditMode;
	// Obj
	class CDragObject* m_DragObj;
	class CShowObject* m_ShowObj;
	// Window
	class CSpriteEditWindow* m_SpriteWindow;
	class CTileMapWindow* m_TileMapWindow;
	class CBackGroundWindow* m_BackGroundWindow;
	class CEditorMenu* m_EditorMenu;
	class CObjectHierarchy* m_ObjectHierarchy;
	class CDetailInfoWindow* m_DetailInfoWindow;
	class CLineEditWindow* m_LineEditWindow;
private :
	float m_CameraMoveSpeed;
	bool m_MousePush;
	Vector2 m_Prev2DMouseWorldPos;
public:
	bool GetMousePush() const
	{
		return m_MousePush;
	}
	EditMode GetEditMode()
	{
		return m_EditMode;
	}
	class CLineEditWindow* GetLineEditWindow() const
	{
		return m_LineEditWindow;
	}
	class CTileMapWindow* GetTileMapWindow () const
	{
		return m_TileMapWindow;
	}
	class CBackGroundWindow* GetBackGroundWindow() const
	{
		return m_BackGroundWindow;
	}
	class CDragObject* GetDragObject() const
	{
		return m_DragObj;
	}
	class CShowObject* GetShowObject() const
	{
		return m_ShowObj;
	}
	class CSpriteEditWindow* GetSpriteWindow() const
	{
		return m_SpriteWindow;
	}
	class CEditorMenu* GetEditorMenu() const
	{
		return m_EditorMenu;
	}
	class CObjectHierarchy* GetObjectHierarchy() const
	{
		return m_ObjectHierarchy;
	}
	class CDetailInfoWindow* GetDetailWindow() const
	{
		return m_DetailInfoWindow;
	}

public:
	void SetEditMode(EditMode Mode);
	void SetSceneEditObject();
	void SetSceneEditObjectPos(class CGameObject* Object);
	// void CreateCameraObject();
public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int  Run();
public:
	void MouseLButtonDown(float DeltaTime);
	void MouseLButtonPush(float DeltaTime);
	void MouseLButtonUp(float DeltaTime);

	void MouseRButtonDown(float DeltaTime);
	void MouseRButtonPush(float DeltaTime);

	void KeyBoardUp(float DeltaTime);
	void KeyBoardLeft(float DeltaTime);
	void KeyBoardRight(float DeltaTime);
	void KeyBoardDown(float DeltaTime);

	void IncreaseXSize(float DeltaTime);
	void DecreaseXSize(float DeltaTime);
	void IncreaseYSize(float DeltaTime);
	void DecreaseYSize(float DeltaTime);

public :
	void CreateSceneMode(class CScene* Scene, const size_t SceneModeTypeID);
	class CComponent* CreateComponent( class CGameObject* Object, const size_t ComponentTypeID);
	void CreateAnimationInstance(class CSpriteComponent* Component, const size_t AnimationTypeID);
	class CGameObject* CreateGameObject(class CScene* Scene, const size_t GameObjectTypeID);

	DECLARE_SINGLE(CEditorManager)
};
