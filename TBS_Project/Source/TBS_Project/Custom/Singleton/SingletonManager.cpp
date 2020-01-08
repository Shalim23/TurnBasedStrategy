#include "SingletonManager.h"

#include "Custom/Events/EventDispatcher.h"
#include "Custom/JsonDataHelper/JsonDataHelper.h"

void SingletonManager::Init()
{
    if (!m_IsInitialized)
    {
        EventDispatcher::GetInstance().Init();
        JsonDataHelper::GetInstance().Init();

        ChangeInitializationState();
    }
}

void SingletonManager::Shutdown()
{
    if (m_IsInitialized)
    {
        ChangeInitializationState();
    }
}

void SingletonManager::ChangeInitializationState()
{
    m_IsInitialized = !m_IsInitialized;
}
