#include "PingPongComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UPingPongComponent::UPingPongComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPingPongComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor *actor = GetOwner();
	this->time += DeltaTime;

	FVector offset;
	offset.X = FMath::Sin(this->speed.X * this->time);
	offset.Y = FMath::Sin(this->speed.Y * this->time);
	offset.Z = FMath::Sin(this->speed.Z * this->time);
    actor->SetActorLocation(offset * this->distance + center);
}

