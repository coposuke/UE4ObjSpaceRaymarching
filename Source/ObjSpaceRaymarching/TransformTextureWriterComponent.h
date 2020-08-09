#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/SceneCapture2D.h"
#include "TransformTextureWriterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OBJSPACERAYMARCHING_API UTransformTextureWriterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTransformTextureWriterComponent();

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category=Material)
	class UMaterial* material;
	
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category=Material)
	class ASceneCapture2D* sceneCapture;

protected:
	virtual void BeginPlay() override;
  	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<AActor*> actors;
    UTexture2D* texture;
	UMaterialInstanceDynamic* materialInstance;
	USceneCaptureComponent2D* sceneCaptureComponent;
};
