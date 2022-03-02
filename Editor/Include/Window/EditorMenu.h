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

    class CIMGUIComboBox* m_EditMenuComboBox;
    class CIMGUIText*           m_CurrentEditMenu;

    class CIMGUIButton*      m_SaveSceneButton;
    class CIMGUIButton*      m_LoadSceneButton;
    class CIMGUIButton*      m_SaveObjectButton;
    class CIMGUIButton*      m_LoadObjectButton;

    class CIMGUIButton* m_ClearComponentButton;
    class CIMGUIButton* m_DeleteComponentButton;
    class CIMGUIButton* m_ClearObjectButton;
    class CIMGUIButton* m_DeleteObjectButton;

    class CIMGUIButton*      m_PlayButton;

	CreateObject_Type         m_ObjectType;
    CreateComponent_Type  m_ComponentType;
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
public :
    void SetEditModeText(const char* Text);
private  :
    void CreateNewObject();
    void CreateNewComponent();
    void TogglePlay();
private:
    void SaveScene();
    void LoadScene();
    void SaveObject();
    void LoadObject();
private:
    void ClearComponent();
    void DeleteComponent();
    void ClearObject();
    void DeleteObject();
private :
    void SelectEditModeCallback(int, const char*);

};


