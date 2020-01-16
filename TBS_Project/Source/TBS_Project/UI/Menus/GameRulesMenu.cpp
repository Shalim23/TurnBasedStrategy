#include "GameRulesMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetComponent.h"
#include "Custom/Events/Events.h"
#include "Custom/Utils/Macros.h"

AGameRulesMenu::AGameRulesMenu()
{
    INIT_ONCE(
        PrimaryActorTick.bCanEverTick = false;

        m_widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("GameRulesWidget"));
        ConstructorHelpers::FClassFinder<UUserWidget> widget(TEXT("/Game/GameRulesWidget"));
        if (widget.Succeeded())
        {
            m_widget->SetWidgetClass(widget.Class);
        }

        SubcribeOnEvents();
        )
}

void AGameRulesMenu::OnReturnToMainMenu(const EventData& eventData)
{
    if (eventData.eventType != GameplayEventType::ReturnToMainMenu)
    {
        return;
    }

    Destroy();
}

void AGameRulesMenu::SubcribeOnEvents()
{
    m_EventsHandler.subscribe({
        {GameplayEventType::ReturnToMainMenu, [this](const EventData& eventData) { OnReturnToMainMenu(eventData); }},
        });
}

void AGameRulesMenu::BeginPlay()
{
    Super::BeginPlay();

}

void AGameRulesMenu::EndPlay(const EEndPlayReason::Type reason)
{
    Super::EndPlay(reason);
}

void AGameRulesMenu::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}