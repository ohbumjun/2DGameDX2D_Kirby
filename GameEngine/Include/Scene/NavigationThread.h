#pragma once
#include "../Thread.h"
#include "../ThreadQueue.h"

class CNavigationThread :
    public CThread
{
    friend class CNavigationManager;

public :
    CNavigationThread();
    virtual ~CNavigationThread();
private :
    class CNavigationManager* m_NavManager;
    class CNavigation* m_Navigation;
    HANDLE m_ExitEvent;
    CThreadQueue<NavWorkData> m_WorkQueue;

public :
    int GetWorkCount ()
{
        return m_WorkQueue.size();
}
public :
    void CreateNavigationNode(class CTileMapComponent* TileMap);
private :
    virtual void Run() override;

public :
    template<typename T, typename ComponentType>
    void AddWork (T* Obj, void(T::*Func)(const std::list<Vector3>&),
        ComponentType* OwnerComponent, const Vector3& End)
{
        NavWorkData Data;

        // Data.Start = Start;

		Data.Callback = std::bind(Func, Obj, std::placeholders::_1);
        Data.End = End;
        Data.OwnerComponent = OwnerComponent;

        m_WorkQueue.push(Data);
}

};

