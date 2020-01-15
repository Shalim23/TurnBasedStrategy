#include "TutorialLevelHandler.h"
#include "Game/LevelTile.h"
#include "Engine/World.h"

ATutorialLevelHandler::ATutorialLevelHandler()
{
 	PrimaryActorTick.bCanEverTick = true;
	GenerateTiles();
}

void ATutorialLevelHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATutorialLevelHandler::GenerateTiles()
{
	//generating 10x10 field
	const int TilesAmount = 100;
	m_Tiles.Reserve(TilesAmount);

	if (UWorld* const world = GetWorld())
	{
		const float Size = 100.0f;
	
		for (int i = 0; i < TilesAmount; ++i)
		{
			const int CurrentDimension = i / 10;
			const float CurrentYPos = CurrentDimension * Size;
			const float CurrentXPos = (i % 10) * Size;

			ALevelTile* tile =
				world->SpawnActor<ALevelTile>(FVector(CurrentXPos, CurrentYPos, 10.0f), FRotator(0.0f, 0.0f, 0.0f));

			tile->SetId(i);

			m_Tiles.Add(tile);
		}

	}

}

void ATutorialLevelHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

