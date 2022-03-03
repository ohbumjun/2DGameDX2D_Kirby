#pragma once
#include "IMGUIWindow.h"

class CObjectHierarchy :
    public CIMGUIWindow
{
public :
    CObjectHierarchy();
    ~CObjectHierarchy();
private :
    class CIMGUIListBox* m_CreatedObjectListBox;
    class CIMGUIListBox* m_CreatedComponentListBox;
private :
    class CIMGUIListBox* m_ObjectList;
    class CIMGUIListBox* m_SpecificObjectList;
private :
    class CGameObject* m_SelectObject;
    class CSceneComponent* m_SelectComponent;
private :
    std::unordered_map<std::string, std::vector<std::string>> m_mapVecObject;
public :
    CIMGUIListBox* GetObjectListBox() const 
	{
		return m_CreatedObjectListBox;
	}
    CIMGUIListBox* GetComponentListBox() const
    {
        return m_CreatedComponentListBox;
    }
public :
    void AddObject(const char* ObjectName);
    void SelectCreatedObject(int Index, const char* ObjectName);
    void SelectCreatedComponent(int Index, const char* ComponentName);
private :
    void SeeObjectList(int Index, const char* ObjectName);
    void SeeSpecificObjectList(int Index, const char* ComponentName);
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
};


