#pragma once
#include "IMGUIWindow.h"

class CDetailInfoWindow :
    public CIMGUIWindow
{
public:
    CDetailInfoWindow();
    ~CDetailInfoWindow();
private : // Image 정보
    class CIMGUIImage* m_CharacterSprite;
private :
    CSharedPtr<class CSceneComponent> m_ClickedComponent;
    class CIMGUIText* m_CharacterCurrentAnimation;
    class CIMGUIComboBox* m_SetAnimationComboBox;
private : // Detail Info
    class CIMGUIText*           m_CharacterName;
    class CIMGUIText*           m_IsGroundText;
    class CIMGUIComboBox* m_IsGroundComboBox;
private: // Transform 정보
    class CIMGUITextInput* m_PosX;
    class CIMGUITextInput* m_PosY;
    class CIMGUITextInput* m_PosZ;

    class CIMGUITextInput* m_RotX;
    class CIMGUITextInput* m_RotY;
    class CIMGUITextInput* m_RotZ;

    class CIMGUITextInput* m_ScaleX;
    class CIMGUITextInput* m_ScaleY;
    class CIMGUITextInput* m_ScaleZ;
public:
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
public :
    void SetPosRotScaleInfo(class CGameObject* Object);
public:
    // Set Clicked Object Info
    void SetClickedObjectInfo(CSceneComponent* Component);
private :
    // Animation
    void SetCurrentAnimationCallback(int Index, const char* Animation);

    // Detail
    void SetDetailInfoName(const std::string& Name);
    void SetIsGroundInfo(bool Enable);
    void SetCurrentAnimationName(const std::string& Name);

    void SetIsGroundCallback(int Index, const char* Animation);

    // Transform
    void SetTransformInfo(CSceneComponent* Component);

    void SetPositionXCallback();
    void SetPositionYCallback();
    void SetPositionZCallback();

    void SetRotationXCallback();
    void SetRotationYCallback();
    void SetRotationZCallback();

    void SetScalingXCallback();
    void SetScalingYCallback();
    void SetScalingZCallback();
};

