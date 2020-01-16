#include "TBSGameInstance.h"
#include "Custom/Utils/Macros.h"
#include "Custom/JsonDataHelper/JsonDataHelper.h"
#include "Custom/Events/EventDispatcher.h"


UTBSGameInstance::UTBSGameInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    INIT_ONCE(
        EventDispatcher::GetInstance().Init();

        JsonDataHelper::GetInstance().Init();
        )
}

void UTBSGameInstance::Shutdown()
{
    Super::Shutdown();

    JsonDataHelper::GetInstance().Shutdown();
    EventDispatcher::GetInstance().Shutdown();
}
