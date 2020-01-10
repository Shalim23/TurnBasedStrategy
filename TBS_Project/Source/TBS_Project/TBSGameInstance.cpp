#include "TBSGameInstance.h"
#include "Custom/Singleton/SingletonManager.h"


UTBSGameInstance::UTBSGameInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SingletonManager::GetInstance().Init();
}

void UTBSGameInstance::Shutdown()
{
    Super::Shutdown();

    SingletonManager::GetInstance().Shutdown();
}
