#pragma once

#include <Component/SpriteComponent.h>

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
	class CIMGUIWindow*                m_EditWindow;
public:
	class CSpriteComponent* GetSpriteComponent() const
	{
		return m_Sprite;
	}
public :
	void SetEditWindow(class CIMGUIWindow* Window)
	{
		m_EditWindow = Window;
	}
	void SetTexture(class CTexture* Texture, int Index = 0)
	{
		m_Sprite->SetTexture(Index, Texture);
	}

public:
	virtual bool               Init() override;
	virtual void               Update(float DeltaTime) override;
	virtual void               PostUpdate(float DeltaTime) override;
	virtual CSpriteEditObject* Clone() override;
	void                       SetEditWindowTexture();
};
