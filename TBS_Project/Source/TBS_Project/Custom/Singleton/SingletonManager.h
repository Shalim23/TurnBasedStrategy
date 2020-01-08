#pragma once
#include "Singleton.h"

class SingletonManager : public Singleton<SingletonManager>
{
public:
    void Init() override;
    void Shutdown();

private:
    void ChangeInitializationState();

private:
    bool m_IsInitialized = false;
};
