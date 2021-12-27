#pragma once

#include "Scene.h"

class CSceneManager
{
private:
	CScene* m_Scene;
	CScene* m_NextScene;
private :
	std::function<void (CScene* Scene, const size_t SceneModeTypeID)> m_CreateSceneModeCallback;
	std::function<class CGameObject* (CScene* Scene, const size_t ObjectTypeID)> m_CreateObjectCallback;
	std::function<class CComponent* (class CGameObject* Object, const size_t ComponentTypeID)> m_CreateComponentCallback;
	std::function<void (class CSpriteComponent* Component, const size_t AnimInstanceTypeID)> m_CreateAnimInstanceCallback;
public :
	void CallCreateSceneModeFunc(CScene* Scene, const size_t SceneModeTypeID)
{
		if (m_CreateSceneModeCallback)
			m_CreateSceneModeCallback(Scene, SceneModeTypeID);
}
	class CGameObject* CallCreateObjectFunc(CScene* Scene, const size_t ObjectTypeID)
	{
		if (m_CreateObjectCallback)
			return m_CreateObjectCallback(Scene, ObjectTypeID);
		return nullptr;
	}
	class CComponent* CallCreateComponentFunc(class CGameObject* Object, const size_t ComponentTypeID)
	{
		if (m_CreateComponentCallback)
			return m_CreateComponentCallback(Object, ComponentTypeID);
		return nullptr;
	}
	void CallCreateAnimInstanceFunc(class CSpriteComponent* Component, const size_t AnimInstanceTypeID)
	{
		if (m_CreateAnimInstanceCallback)
			m_CreateAnimInstanceCallback(Component, AnimInstanceTypeID);
	}


public:
	CScene* GetScene() const
	{
		return m_Scene;
	}
public :
	template<typename T>
	void SetCreateSceneModeCallback(T*Obj, void(T::*Func)(CScene* Scene, const size_t SceneModeTypeID))
	{
		m_CreateSceneModeCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}
	template<typename T>
	void SetCreateObjectCallback(T* Obj, class CGameObject* (T::* Func)(CScene* Scene, const size_t ObjectTypeID))
	{
		m_CreateObjectCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}
	template<typename T>
	void SetCreateComponentCallback(T* Obj, class CComponent* (T::* Func)(class CGameObject* Object, const size_t ComponentTypeID))
	{
		m_CreateComponentCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}
	template<typename T>
	void SetCreateAnimInstanceCallback(T* Obj, void(T::* Func)(class CSpriteComponent* Component, const size_t AnimInstanceTypeID))
	{
		m_CreateAnimInstanceCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}

public:
	void Start();
	bool Init();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);

public:
	template <typename T>
	bool CreateSceneMode(bool Current = true)
	{
		if (Current)
			return m_Scene->CreateSceneMode<T>();

		return m_NextScene->CreateSceneMode<T>();
	}

	DECLARE_SINGLE(CSceneManager)
};
