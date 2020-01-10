#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Custom/Events/EventsHandler.h"
#include "GameRulesMenu.generated.h"

class UWidgetComponent;
struct EventData;

UCLASS()
class TBS_PROJECT_API AGameRulesMenu : public AActor
{
    GENERATED_BODY()

public:
    AGameRulesMenu();
    void Tick(float DeltaTime) override;

protected:
    void BeginPlay() override;

private:
    void SubcribeOnEvents();

    void OnReturnToMainMenu(const EventData& eventData);

private:
    UPROPERTY()
        UWidgetComponent* m_widget;

    EventsHandler m_EventsHandler;

};
