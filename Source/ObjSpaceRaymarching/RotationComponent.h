// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OBJSPACERAYMARCHING_API URotationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotationComponent();

public:
	UPROPERTY(EditInstanceOnly, Category="Rotation")
	bool isAuto;
	
	UPROPERTY(EditInstanceOnly, Category="Rotation")
	float autoRotateSpeed = 50.0f;

	UPROPERTY(EditInstanceOnly, Category="Rotation")
	float distance;

	UPROPERTY(EditInstanceOnly, Category="Rotation")
	float altitude;

	UPROPERTY(EditInstanceOnly, Category="Rotation")
	float azimuth;
	
	UPROPERTY(EditInstanceOnly, Category="Rotation")
	FVector lookat;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
