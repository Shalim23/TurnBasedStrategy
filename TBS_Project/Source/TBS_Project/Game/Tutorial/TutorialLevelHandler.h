#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialLevelHandler.generated.h"

class ALevelTile;

UCLASS()
class TBS_PROJECT_API ATutorialLevelHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	ATutorialLevelHandler();
	void Tick(float DeltaTime) override;

protected:
	void BeginPlay() override;

private:
	void GenerateTiles();

private:
	UPROPERTY()
	TArray<ALevelTile*> m_Tiles;

};
