#include "LevelTile.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Custom/Utils/PrintScreenHelper.h"
#include "Custom/Utils/Macros.h"


//#TODO remove debug operations
ALevelTile::ALevelTile()
{
	INIT_ONCE(
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

        m_Mesh->SetMaterial(0, m_ColorMaterial);

        m_Mesh->OnClicked.AddDynamic(this, &ALevelTile::ChangeColor);

        RootComponent = m_Mesh;
        )
}

void ALevelTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelTile::ChangeColor(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
	PrintOnScreenHelper::PrintOnScreenInt(m_Id);
	//m_ColorMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::Red);
}

void ALevelTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTile::SetId(int _id)
{
	m_Id = _id;
}

