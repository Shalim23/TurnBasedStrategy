#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Custom/Events/EventsHandler.h"
#include "MainMenu.generated.h"

class UWidgetComponent;
struct EventData;

UCLASS()
class TBS_PROJECT_API AMainMenu : public AActor
{
    GENERATED_BODY()

public:
    AMainMenu();
    void Tick(float DeltaTime) override;

protected:
    void BeginPlay() override;
    void EndPlay(const EEndPlayReason::Type reason) override;

private:
    void OnLeaveMainMenu(const EventData& eventData);
    void SubcribeOnEvents();

private:
    UPROPERTY()
    UWidgetComponent* m_widget;

    EventsHandler m_EventsHandler;

};
