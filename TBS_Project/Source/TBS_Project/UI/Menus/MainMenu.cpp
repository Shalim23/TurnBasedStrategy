#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetComponent.h"
#include "Custom/Events/Events.h"

AMainMenu::AMainMenu()
{
    PrimaryActorTick.bCanEverTick = false;

    m_widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MainMenuWidget"));
    ConstructorHelpers::FClassFinder<UUserWidget> widget(TEXT("/Game/MainMenuWidget"));
    if (widget.Succeeded())
    {
        m_widget->SetWidgetClass(widget.Class);
    }
}

void AMainMenu::SubcribeOnEvents()
{
    m_EventsHandler.subscribe({
        {GameplayEventType::LeaveFromMainMenu, [this](const EventData& eventData) { OnLeaveMainMenu(eventData); }},
        });
}

void AMainMenu::OnLeaveMainMenu(const EventData& eventData)
{
    if (eventData.eventType != GameplayEventType::LeaveFromMainMenu)
    {
        return;
    }

    m_EventsHandler.unsubscribe();
    Destroy();
}

void AMainMenu::BeginPlay()
{
    Super::BeginPlay();
    SubcribeOnEvents();
}

void AMainMenu::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}