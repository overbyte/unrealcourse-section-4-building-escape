// Copyright Allandt Bik-Elliott 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY(EditAnywhere)
        float OpenSpeed = 2.f;

    UPROPERTY(EditAnywhere)
        float CloseSpeed = 2.f;

    UPROPERTY(EditAnywhere)
        float OpenAngle = 90.f;

    UPROPERTY(EditAnywhere)
        float CloseDelay = 2.f;

    UPROPERTY(EditAnywhere)
        ATriggerVolume* PressurePlate;

    UPROPERTY(EditAnywhere)
        AActor* ActorToOpen;

    float InitialYaw = 0.f;
    float CurrentYaw = 0.f;
    float LastOpenedTime = 0.f;

    void OpenDoor(float DeltaTime);
    void CloseDoor(float DeltaTime);
};
