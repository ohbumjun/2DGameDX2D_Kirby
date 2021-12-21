#pragma once
#include "IMGUIWindow.h"

class CObjectHierarchy :
    public CIMGUIWindow
{
public :
    CObjectHierarchy();
    ~CObjectHierarchy();
private :
    class CIMGUIListBox* m_ObjectListBox;
    class CIMGUIListBox* m_ComponentListBox;
public :
    CIMGUIListBox* GetObjectListBox() const 
	{
		return m_ObjectListBox;
	}
    CIMGUIListBox* GetComponentListBox() const
    {
        return m_ComponentListBox;
    }
public :
    void SelectObject(int Index, const char* ObjectName);
    void SelectComponent(int Index, const char* ComponentName);
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
};


