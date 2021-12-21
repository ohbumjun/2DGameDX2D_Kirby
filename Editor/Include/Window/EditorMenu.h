#pragma once
#include "IMGUIWindow.h"

class CEditorMenu :
    public CIMGUIWindow
{
public:
	CEditorMenu();
    ~CEditorMenu();
private :
    class CIMGUIComboBox* m_ObjectComboBox;
    class CIMGUITextInput* m_ObjectNameInput;
	class CIMGUIComboBox* m_ComponentComboBox;
    class CIMGUITextInput* m_ComponentNameInput;
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
public :
    void SelectObjectType(int Index, const char* ObjectType);
    void SelectComponentType(int Index, const char* ComponentType);
    void CreateNewObject();
    void CreateNewComponent();
};


