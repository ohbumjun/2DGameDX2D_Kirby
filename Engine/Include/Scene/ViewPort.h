#pragma once

#include "../GameInfo.h"
#include "../UI/UIWindow.h"

class CViewPort
{
	friend class CScene;
protected:
	CViewPort();
	~CViewPort();
private :
	class CScene* m_Scene;
	std::list<CSharedPtr<class CUIWindow>> m_WindowList;
	class CUIWindow* m_BossHUD;
	class CUIWindow* m_PlayerHUD;
	std::function<void()> m_BossHUDStartCallback;
public:
	class CUIWindow* GetPlayerHUD() const
	{
		return m_PlayerHUD;
	}
	class CUIWindow* GetBossHUD() const
	{
		return m_BossHUD;
	}
public :
	bool Init();
	void Start();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Render();
	bool CollisionMouse();
	void Collision();
public :
	void SetPlayerHUD(class CUIWindow* Window)
	{
		m_PlayerHUD = Window;
	}
	void SetBossHUD(class CUIWindow* Window)
	{
		m_BossHUD = Window;
	}
	static bool SortWindow(CSharedPtr<class CUIWindow>& A, CSharedPtr<class CUIWindow>& B)
{
		return A->m_ZOrder > B->m_ZOrder;
}
	void SetScene(class CScene* Scene)
{
		m_Scene = Scene;
}
public :
	class CScene* GetScene() const
{
		return m_Scene;
}
	void StartBossHUDCallback();
public :
	template<typename T>
	T* FindUIWindow(const std::string& Name)
{
		auto iter = m_WindowList.begin();
		auto iterEnd = m_WindowList.end();
		for (; iter != iterEnd;++iter)
		{
			if ((*iter)->GetName() == Name)
				return (T*)(*iter);
		}
		return nullptr;
}
	template<typename T>
	T* CreateUIWindow(const std::string& Name)
{
		T* Window = new T;

		Window->SetName(Name);
		Window->SetScene(m_Scene);
		Window->m_ViewPort = this;

		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}

		m_WindowList.push_back(Window);
		return Window;
}
	template<typename T>
	void SetBossHUDStartCallback(T* Obj, void(T::*Func)())
{
		m_BossHUDStartCallback = std::bind(Func, Obj);
}
};

