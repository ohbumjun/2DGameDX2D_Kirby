#pragma once
#include "IMGUIWindow.h"

class CDetailInfoWindow :
    public CIMGUIWindow
{
public:
    CDetailInfoWindow();
    ~CDetailInfoWindow();
private:
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

