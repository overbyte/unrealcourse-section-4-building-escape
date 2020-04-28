// Copyright Allandt Bik-Elliott 2020


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

    InitialYaw = CurrentYaw =  GetOwner()->GetActorRotation().Yaw;
    OpenAngle += InitialYaw;
    LastOpenedTime = GetWorld()->GetTimeSeconds();

    if (!PressurePlate)
    {
        UE_LOG(LogTemp, Error, TEXT("PressurePlate should be assigned on component %s"), *GetOwner()->GetName());
    }

    ActorToOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}

float UOpenDoor::GetTotalMassOfOverlappingActors() const
{
    float TotalMass = 0.f;

    TArray<AActor*> OverlappingActors;
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        TotalMass = Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    }

    return TotalMass;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (GetTotalMassOfOverlappingActors() > MassToOpen)
    {
        OpenDoor(DeltaTime);
        LastOpenedTime = GetWorld()->GetTimeSeconds();
        //UE_LOG(LogTemp, Warning, TEXT("last opened: %f"), LastOpenedTime);
    }
    else if (GetWorld()->GetTimeSeconds() > LastOpenedTime + CloseDelay)
    {
        CloseDoor(DeltaTime);
    }

	// ...
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
    // set the current yaw to the interpolated rotation
    CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * OpenSpeed);
    // worth doing this in case this has other rotation (pitch, roll) assigned to it
    FRotator DoorRotation = GetOwner()->GetActorRotation();
    DoorRotation.Yaw = CurrentYaw;
    GetOwner()->SetActorRotation(DoorRotation);
};

void UOpenDoor::CloseDoor(float DeltaTime)
{
    // set the current yaw to the interpolated rotation
    CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * CloseSpeed);
    // worth doing this in case this has other rotation (pitch, roll) assigned to it
    FRotator DoorRotation = GetOwner()->GetActorRotation();
    DoorRotation.Yaw = CurrentYaw;
    GetOwner()->SetActorRotation(DoorRotation);
};
