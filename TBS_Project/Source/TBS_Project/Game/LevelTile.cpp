#include "LevelTile.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Custom/Utils/PrintScreenHelper.h"

ALevelTile::ALevelTile()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Plane"));
	if (MeshAsset.Succeeded())
	{
		m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh"));
		m_Mesh->SetStaticMesh(MeshAsset.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/ColorMaterial"));
	
	if (MaterialAsset.Succeeded())
	{
		m_ColorMaterial = CreateDefaultSubobject<UMaterialInstanceDynamic>(TEXT("Material"));
		m_ColorMaterial = UMaterialInstanceDynamic::Create(MaterialAsset.Object, m_Mesh);
	}

	m_Mesh->SetWorldLocation(FVector(0.0f, 0.0f, 10.0f), false, 0, ETeleportType::None);
	m_Mesh->SetMaterial(0, m_ColorMaterial);

	m_Mesh->OnClicked.AddDynamic(this, &ALevelTile::ChangeColor);
}

void ALevelTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelTile::ChangeColor(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
	PrintOnScreenHelper::PrintOnScreenFString("ALevelTile::ChangeColor");
	m_ColorMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::Red);
}

void ALevelTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

