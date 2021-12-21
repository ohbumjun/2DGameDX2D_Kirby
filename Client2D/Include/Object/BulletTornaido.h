#pragma once

#include "GameObject/GameObject.h"

class CBulletTornaido :
	public CGameObject
{
	friend class CScene;

protected:
	CBulletTornaido();
	CBulletTornaido(const CBulletTornaido& obj);
	virtual ~CBulletTornaido() override;

private:
	CSharedPtr<class CSceneComponent>  m_Root;
	CSharedPtr<class CSceneComponent>  m_Root1;
	CSharedPtr<class CSpriteComponent> m_Sprite;

public:
	virtual bool             Init() override;
	virtual void             Update(float DeltaTime) override;
	virtual void             PostUpdate(float DeltaTime) override;
	virtual CBulletTornaido* Clone() override;
};
