#include "GameRulesMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetComponent.h"
#include "Custom/Events/Events.h"

AGameRulesMenu::AGameRulesMenu()
{
    PrimaryActorTick.bCanEverTick = false;

    m_widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("GameRulesWidget"));
    ConstructorHelpers::FClassFinder<UUserWidget> widget(TEXT("/Game/GameRulesWidget"));
    if (widget.Succeeded())
    {
        m_widget->SetWidgetClass(widget.Class);
    }
}

void AGameRulesMenu::OnReturnToMainMenu(const EventData& eventData)
{
    if (eventData.eventType != GameplayEventType::ReturnToMainMenu)
    {
        return;
    }

    m_EventsHandler.unsubscribe();
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

    SubcribeOnEvents();
}

void AGameRulesMenu::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
