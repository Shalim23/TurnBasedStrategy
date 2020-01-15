#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTile.generated.h"

UCLASS()
class TBS_PROJECT_API ALevelTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelTile();
	void Tick(float DeltaTime) override;
	void SetId(int _id);

protected:
	void BeginPlay() override;

private:
    UFUNCTION()
    void ChangeColor(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);

private:
	UPROPERTY()
	UStaticMeshComponent* m_Mesh = nullptr;

    UPROPERTY()
    UMaterialInstanceDynamic* m_ColorMaterial = nullptr;

	int m_Id = 0;

};
