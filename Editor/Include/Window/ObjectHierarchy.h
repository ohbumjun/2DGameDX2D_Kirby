#pragma once
#include "IMGUIWindow.h"
#include "IMGUIListBox.h"

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
    CIMGUIListBox* GetCandidateObjectListBox() const
    {
        return m_ObjectList;
    }
    bool IsSpecificObjectSelected () const
{
        return m_SpecificObjectList->GetSelectIndex() != -1;
}
    CIMGUIListBox* GetSpecificObjectListBox() const
    {
        return m_SpecificObjectList;
    }
    CIMGUIListBox* GetCreatedObjectListBox() const 
	{
		return m_CreatedObjectListBox;
	}
    CIMGUIListBox* GetCreatedComponentListBox() const
    {
        return m_CreatedComponentListBox;
    }
public :
    void AddCreatedObject(const char* ObjectName);
    void SelectCreatedObject(int Index, const char* ObjectName);
    void SelectCreatedComponent(int Index, const char* ComponentName);
private :
    void SeeObjectList(int Index, const char* ObjectName);
    void SeeSpecificObjectList(int Index, const char* ComponentName);
public :
    bool CheckDuplicateObject(const std::string& ObjName);
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
};


