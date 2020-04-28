// Copyright Allandt Bik-Elliott 2020

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// much nicer for readability but no actual functionality
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

    FindPhysicsHandle();
    SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
    // check for physics handle component on default pawn
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle) {}
    else
    {
        UE_LOG(LogTemp, Error, TEXT("There must be a Physics Handle Component on the Default Pawn: %s"), *GetOwner()->GetName());
    }
}

void UGrabber::SetupInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent) {
        UE_LOG(LogTemp, Warning, TEXT("Found Input component on %s"), *GetOwner()->GetName());
        InputComponent->BindAction("GrabberMapping", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("GrabberMapping", EInputEvent::IE_Released, this, &UGrabber::Release);
    }
    else
    {

        UE_LOG(LogTemp, Error, TEXT("No Input component found on %s"), *GetOwner()->GetName());
    }
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("Grabber Grabbed"));

    FVector LookAtPosition = GetLookAtPosition();

    FHitResult HitResult = GetFirstPhysicsBodyInReach();

    // if something is hit
    if (HitResult.GetActor())
    {
        UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

        // attach a physics handle
        PhysicsHandle->GrabComponentAtLocation(
            ComponentToGrab,
            NAME_None,
            LookAtPosition
        );
    }
}

void UGrabber::Release()
{
    UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));

    if (PhysicsHandle->GetGrabbedComponent())
    {
        PhysicsHandle->ReleaseComponent();
    }
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FVector LookAtPosition = GetLookAtPosition();

    // if a physics handle is attached
    if (PhysicsHandle->GetGrabbedComponent()) 
    {
        // move the object
        PhysicsHandle->SetTargetLocation(LookAtPosition);
    }
}

FVector UGrabber::GetLookAtPosition() const
{
    // out params
    FVector PlayerViewPointPosition;
    FRotator PlayerViewPointRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointPosition,
        OUT PlayerViewPointRotation
    );
    //UE_LOG(LogTemp, Warning, TEXT("Loc: %s, Rot: %s"), *PlayerViewPointPosition.ToString(), *PlayerViewPointRotation.ToString());

    FVector LookAtPosition = PlayerViewPointPosition + (PlayerViewPointRotation.Vector() * Reach);

    return LookAtPosition;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    //FVector LookAtPosition = GetLookAtPosition();

    // TODO Dupliate code
    FVector PlayerViewPointPosition;
    FRotator PlayerViewPointRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointPosition,
        OUT PlayerViewPointRotation
    );
    //UE_LOG(LogTemp, Warning, TEXT("Loc: %s, Rot: %s"), *PlayerViewPointPosition.ToString(), *PlayerViewPointRotation.ToString());

    FVector LookAtPosition = PlayerViewPointPosition + (PlayerViewPointRotation.Vector() * Reach);
    // end of duplicat code

    /*
    DrawDebugLine(
        GetWorld(),
        PlayerViewPointPosition,
        LookAtPosition,
        FColor(0, 255, 0),
        false,
        0.f,
        0,
        5.f
    );
    */

    // out param for raycast
    FHitResult Hit;

    // collision query params (no name, don't use complex collision, ignore this
    // owner object
    FCollisionQueryParams CollisionParams(
        FName(TEXT("")),
        false,
        GetOwner()
    );

    GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewPointPosition,
        LookAtPosition,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        CollisionParams
    );

    AActor* ActorHit = Hit.GetActor();
    if (ActorHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Collision: %s"), *ActorHit->GetName());
    }

    return Hit;
}
