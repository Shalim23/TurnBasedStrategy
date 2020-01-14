#include "TBSGameViewportClient.h"

void UTBSGameViewportClient::ReceivedFocus(FViewport* viewport)
{
    UGameViewportClient::ReceivedFocus(viewport);

    APlayerController* controller = GetWorld()->GetFirstPlayerController();
    if (controller)
    {
        FInputModeGameAndUI inputMode;
        inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
        controller->SetInputMode(inputMode);
    }
}
