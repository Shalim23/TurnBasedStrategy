#include "TBSGameInstance.h"
#include "Custom/JsonDataHelper/JsonDataHelper.h"
#include "Custom/Events/EventDispatcher.h"


UTBSGameInstance::UTBSGameInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    //make sure we init singletons only once
    if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
    {
        EventDispatcher::GetInstance().Init();
        JsonDataHelper::GetInstance().Init();
    }
}

void UTBSGameInstance::Shutdown()
{
    Super::Shutdown();

    JsonDataHelper::GetInstance().Shutdown();
    EventDispatcher::GetInstance().Shutdown();
}
