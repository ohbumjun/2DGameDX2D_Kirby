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
    class CIMGUITextInput*   m_ObjectNameInput;
    class CIMGUIButton*      m_ObjectButton;

	class CIMGUIComboBox* m_ComponentComboBox;
    class CIMGUITextInput*  m_ComponentNameInput;
    class CIMGUIButton*      m_ComponentButton;

    class CIMGUIButton*      m_SaveSceneButton;
    class CIMGUIButton*      m_LoadSceneButton;

    class CIMGUIButton*      m_PlayButton;

	CreateObject_Type         m_ObjectType;
    CreateComponent_Type  m_ComponentType;
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
public :
    void CreateNewObject();
    void CreateNewComponent();
    void TogglePlay();
public :
    void SaveScene();
    void LoadScene();

};


