#pragma once

#include <Animation/AnimationSequence2DInstance.h>
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
	bool m_ReverseMode;
public:
	bool GetReverseMode() const
	{
		return m_ReverseMode;
	}
	class CSpriteComponent* GetSpriteComponent() const
	{
		return m_Sprite;
	}
public :
	void SetEditWindow(class CIMGUIWindow* Window)
	{
		m_EditWindow = Window;
	}
	void SetMaterialTexture(class CTexture* Texture, int Index = 0)
	{
		m_Sprite->SetTexture(Index, Texture);
	}
	void SetAnimationNewTexture(class CTexture* Texture)
	{
		if (!m_Sprite || !m_Sprite->GetAnimationInstance())
			return;
		// Texture 세팅
		m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetSequenceTexture(Texture);
		// 기존 FrameData 비워주고
		m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->ClearSequenceFrame();
		// 새롭게 FrameData 추가해주기
		if (m_ReverseMode)
		{
			m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->AddFrame(
				Vector2(0.f, 0.f), Vector2((float)Texture->GetWidth() * -1.f, (float)Texture->GetHeight()));
		}
		else
		{
			m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->AddFrame(
				Vector2(0.f, 0.f), Vector2((float)Texture->GetWidth(), (float)Texture->GetHeight()));
		}
	}
	void SetTextureWorldScale(int Index = 0)
	{
		m_Sprite->SetTextureWorldScale(Index);
	}

	void SetReverse(bool Reverse);
public:
	virtual bool               Init() override;
	virtual void               Update(float DeltaTime) override;
	virtual void               PostUpdate(float DeltaTime) override;
	virtual CSpriteEditObject* Clone() override;
	void                       SetEditWindowTexture();
};
