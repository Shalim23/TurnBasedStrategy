#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Custom/Events/EventsHandler.h"
#include "FindGameMenu.generated.h"

class UWidgetComponent;
struct EventData;

UCLASS()
class TBS_PROJECT_API AFindGameMenu : public AActor
{
    GENERATED_BODY()

public:
    AFindGameMenu();
    void Tick(float DeltaTime) override;

protected:
    void BeginPlay() override;

private:

    void OnReturnToMainMenu(const EventData& eventData);
    void OnNoServerConnection(const EventData& eventData);
    void OnConnectedToServer(const EventData& eventData);
    void OnGameFound(const EventData& eventData);
    void OnAllPlayersReady(const EventData& eventData);
    void OnNotReady(const EventData& eventData);
    void OnWaitingForPlayers(const EventData& eventData);

    void OnWidgetConstructed();

    void SubcribeOnEvents();

private:
    UPROPERTY()
        UWidgetComponent* m_widgetComponent;

    EventsHandler m_EventsHandler;
};
