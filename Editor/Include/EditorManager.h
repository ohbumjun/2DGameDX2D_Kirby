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
	class CShowObject* m_SceneEditObj;
	class CSpriteEditWindow* m_SpriteWindow;
	class CEditorMenu* m_EditorMenu;
	class CObjectHierarchy* m_ObjectHierarchy;
	class CDetailInfoWindow* m_DetailInfoWindow;

public:
	EditMode GetEditMode()
	{
		return m_EditMode;
	}

	class CDragObject* GetDragObject() const
	{
		return m_DragObj;
	}
	class CShowObject* GetSceneEditObject() const
	{
		return m_SceneEditObj;
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

public :
	void CreateSceneMode(class CScene* Scene, const size_t SceneModeTypeID);
	class CGameObject* CreateGameObject(class CScene* Scene, const size_t GameObjectTypeID);
	class CComponent* CreateComponent( class CGameObject* Object, const size_t ComponentTypeID);
	void CreateAnimationInstance(class CSpriteComponent* Component, const size_t AnimationTypeID);
	

	DECLARE_SINGLE(CEditorManager)
};
