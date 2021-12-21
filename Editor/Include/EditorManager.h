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

public:
	EditMode GetEditMode()
	{
		return m_EditMode;
	}

	class CDragObject* GetDragObject() const
	{
		return m_DragObj;
	}

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
