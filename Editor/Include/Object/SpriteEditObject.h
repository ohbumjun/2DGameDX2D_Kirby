#pragma once

#include "GameObject/GameObject.h"

class CSpriteEditObject :
	public CGameObject
{
	friend class CScene;

protected:
	CSpriteEditObject();
	CSpriteEditObject(const CSpriteEditObject& obj);
	virtual ~CSpriteEditObject() override;

private:
	CSharedPtr<class CSpriteComponent> m_Sprite;
	float                              m_Distance;
	bool                               m_ObjMouseDown;
	Vector2                            m_ObjImageStartPos;
	Vector2                            m_ObjImageEndPos;
	class CIMGUIWindow*                m_EditWindow;
public:
	class CSpriteComponent* GetSpriteComponent() const
	{
		return m_Sprite;
	}

	void SetEditWindow(class CIMGUIWindow* Window)
	{
		m_EditWindow = Window;
	}

public:
	virtual bool               Init() override;
	virtual void               Update(float DeltaTime) override;
	virtual void               PostUpdate(float DeltaTime) override;
	virtual CSpriteEditObject* Clone() override;
	void                       UpdateClickInfo();
	void                       SetEditWindowTexture();
};
