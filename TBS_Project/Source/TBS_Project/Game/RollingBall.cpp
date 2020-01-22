#include "RollingBall.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "CameraHandler.h"
#include "Custom/Utils/PrintScreenHelper.h"

ARollingBall::ARollingBall()
{
 	PrimaryActorTick.bCanEverTick = true;
	
    ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
    if (MeshAsset.Succeeded())
    {
        m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
        m_Mesh->SetStaticMesh(MeshAsset.Object);
    }

    RootComponent = m_Mesh;
    m_Mesh->SetSimulatePhysics(true);

    m_Mesh->BodyInstance.SetMassOverride(1.0f);
    //m_Mesh->BodyInstance.bLockZRotation = true;
}

void ARollingBall::BeginPlay()
{
	Super::BeginPlay();
}

void ARollingBall::MoveForwardBackward(float value)
{
    m_Mesh->AddForce(FVector(value * 200.0f, 0.0f, 0.0f));
    /*m_Mesh->SetWorldLocation(m_Mesh->GetComponentLocation() + FVector(value, 0.0f, 0.0f),
        false, 0, ETeleportType::None);*/
}

void ARollingBall::MoveLeftRight(float value)
{
    //m_Mesh->setfor((m_Mesh->GetForwardVector() + FVector(0.0f, value, 0.0f)).Rotation());
    /*m_Mesh->SetWorldLocation(m_Mesh->GetComponentLocation() + FVector(0.0f, value, 0.0f),
        false, 0, ETeleportType::None);*/
}

void ARollingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (m_Camera)
    {
        m_Camera->UpdateCamera(m_Mesh->GetPhysicsLinearVelocity(), m_Mesh->GetComponentLocation());
    }

    PrintOnScreenHelper::PrintOnScreenFVector(m_Mesh->GetPhysicsLinearVelocity());
}

void ARollingBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("Forward", this, &ARollingBall::MoveForwardBackward);
    PlayerInputComponent->BindAxis("Left", this, &ARollingBall::MoveLeftRight);
}

void ARollingBall::SetCamera(ACameraHandler* camera)
{
    m_Camera = camera;
}

