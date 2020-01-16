#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetComponent.h"
#include "Custom/Events/Events.h"
#include "Custom/Utils/Macros.h"

AMainMenu::AMainMenu()
{
    INIT_ONCE(
        PrimaryActorTick.bCanEverTick = false;

        m_widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MainMenuWidget"));
        ConstructorHelpers::FClassFinder<UUserWidget> widget(TEXT("/Game/MainMenuWidget"));
        if (widget.Succeeded())
        {
            m_widget->SetWidgetClass(widget.Class);
        }

        SubcribeOnEvents();
        )
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

    Destroy();
}

void AMainMenu::BeginPlay()
{
    Super::BeginPlay();
}

void AMainMenu::EndPlay(const EEndPlayReason::Type reason)
{
    Super::EndPlay(reason);
}

void AMainMenu::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}