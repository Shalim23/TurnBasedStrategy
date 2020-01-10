#pragma once

#include "Custom/Events/EventsHandler.h"
#include "Custom/Network/NetworkMessagesHandler.h"
#include "Custom/Network/NetworkSocketHandler.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetworkManager.generated.h"

struct EventData;

UCLASS()
class TBS_PROJECT_API ANetworkManager : public AActor
{
    GENERATED_BODY()

public:
    ANetworkManager();
    void Tick(float DeltaTime) override;

protected:
    void BeginPlay() override;

private:
    void OnReturnToMainMenu(const EventData& eventData);
    void OnPlayerReady(const EventData& eventData);
    void Connect();

    void Shutdown();
    void SubscribeOnEvents();
    void ProcessMessage(const TSharedPtr<FJsonObject>& jsonObject);
    void OnConnectionError();

private:

    bool m_IsConnected = false;
    EventsHandler m_EventsHandler;
    NetworkMessagesHandler m_NetworkMessagesHandler;
    NetworkSocketHandler m_NetworkSocketHandler;

};

