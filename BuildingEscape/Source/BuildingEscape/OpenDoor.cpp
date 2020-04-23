// Copyright Allandt Bik-Elliott 2020


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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
    TargetYaw += InitialYaw;

    if (!PressurePlate)
    {
        UE_LOG(LogTemp, Error, TEXT("PressurePlate should be assigned on component %s"), *GetOwner()->GetName());
    }

    ActorToOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (PressurePlate->IsOverlappingActor(ActorToOpen))
    {
        OpenDoor(DeltaTime);
    }

	// ...
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
    // set the current yaw to the interpolated rotation
    CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, OpenSpeed);
    // worth doing this in case this has other rotation (pitch, roll) assigned to it
    FRotator DoorRotation = GetOwner()->GetActorRotation();
    DoorRotation.Yaw = CurrentYaw;
    GetOwner()->SetActorRotation(DoorRotation);
};
