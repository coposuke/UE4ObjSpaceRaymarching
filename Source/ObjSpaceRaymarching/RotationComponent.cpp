// Fill out your copyright notice in the Description page of Project Settings.

#include "RotationComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
URotationComponent::URotationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void URotationComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void URotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor *actor = GetOwner();

    if (this->isAuto)
    {
        this->azimuth += DeltaTime * autoRotateSpeed;
    }

    FQuat altitudeAngle = FQuat::MakeFromEuler(FVector(0.0f, this->altitude, 0.0f));
    FQuat azimuthAngle = FQuat::MakeFromEuler(FVector(0.0f, 0.0f, this->azimuth));

    FVector position;
    position = altitudeAngle * FVector(this->distance, 0.0f, 0.0f);
    position = azimuthAngle * position;

    FRotator rotator = UKismetMathLibrary::FindLookAtRotation(position, this->lookat);

    actor->SetActorLocation(position);
    actor->SetActorRotation(rotator);
}

