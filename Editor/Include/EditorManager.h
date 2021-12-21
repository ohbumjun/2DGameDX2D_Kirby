#pragma once

#include "GameInfo.h"

enum class EditMode
{
	Scene,
	Sprite
};

class CEditorManager
{
private:
	EditMode           m_EditMode;
	class CDragObject* m_DragObj;
	class CSpriteWindow* m_SpriteWindow;
	class CEditorMenu* m_EditorMenu;
	class CObjectHierarchy* m_ObjectHierarchy;
	class CDetailWindow* m_DetailWindow;

public:
	EditMode GetEditMode()
	{
		return m_EditMode;
	}

	class CDragObject* GetDragObject() const
	{
		return m_DragObj;
	}
	class CSpriteWindow* GetSpriteWindow() const
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
	class CDetailWindow* GetDetailWindow() const
	{
		return m_DetailWindow;
	}

public:
	void SetEditMode(EditMode Mode);

public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int  Run();

public:
	void MouseLButtonDown(float DeltaTime);
	void MouseLButtonPush(float DeltaTime);
	void MouseLButtonUp(float DeltaTime);

	void KeyBoardUp(float DeltaTime);
	void KeyBoardLeft(float DeltaTime);
	void KeyBoardRight(float DeltaTime);
	void KeyBoardDown(float DeltaTime);

	void IncreaseXSize(float DeltaTime);
	void DecreaseXSize(float DeltaTime);
	void IncreaseYSize(float DeltaTime);
	void DecreaseYSize(float DeltaTime);

	DECLARE_SINGLE(CEditorManager)
};
