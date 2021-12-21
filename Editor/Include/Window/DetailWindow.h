#pragma once

#include "IMGUIWindow.h"

class CDetailWindow :
    public CIMGUIWindow
{
public :
    CDetailWindow();
    ~CDetailWindow();
private :
    class CIMUGITextInput* m_PosX;
    class CIMUGITextInput* m_PosY;
    class CIMUGITextInput* m_PosZ;

	class CIMUGITextInput* m_RotX;
    class CIMUGITextInput* m_RotY;
    class CIMUGITextInput* m_RotZ;

    class CIMUGITextInput* m_ScaleX;
    class CIMUGITextInput* m_ScaleY;
    class CIMUGITextInput* m_ScaleZ;
public :
    bool Init();
    void Update(float DeltaTime);
public :
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
