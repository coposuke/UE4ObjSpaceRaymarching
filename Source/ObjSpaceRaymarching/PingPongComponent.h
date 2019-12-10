#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PingPongComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OBJSPACERAYMARCHING_API UPingPongComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPingPongComponent();

public:
	UPROPERTY(EditInstanceOnly, Category="Pingpong")
	FVector speed;

	UPROPERTY(EditInstanceOnly, Category="Pingpong")
	float distance;

	UPROPERTY(EditInstanceOnly, Category="Pingpong")
	FVector center;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float time;
};
