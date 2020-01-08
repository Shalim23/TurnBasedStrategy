// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;

UCLASS()
class TBS_PROJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UMainMenuWidget(const FObjectInitializer& objectInitializer);


protected:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UButton* StartGameButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UButton* GameRulesButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UButton* ExitGameButton;

private:
    void NativeConstruct() override;

    UFUNCTION()
        void ExitGame();

    UFUNCTION()
        void GoToGameRules();

    UFUNCTION()
        void FindGame();
};
