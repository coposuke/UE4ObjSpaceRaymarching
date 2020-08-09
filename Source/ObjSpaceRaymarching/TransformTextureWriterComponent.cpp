#include "TransformTextureWriterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TextureRenderTarget2D.h"
#include "SceneViewport.h"

UTransformTextureWriterComponent::UTransformTextureWriterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTransformTextureWriterComponent::BeginPlay()
{
	Super::BeginPlay();
    
    // SceneDepthを手動で作成するので、ここで作成し、SceneCapture2Dに設定する
    if(this->sceneCapture != nullptr)
    {
        auto playerController = GetWorld()->GetFirstPlayerController();
        ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(playerController->Player);
        FIntPoint viewportSize = LocalPlayer->ViewportClient->Viewport->GetSizeXY();
        // Camera(Camera Option)のContainAspectRatioに対応できていない（どこにある？）

        auto textureTarget = NewObject<UTextureRenderTarget2D>(GetTransientPackage(), NAME_None, RF_Transient);
        textureTarget->RenderTargetFormat = RTF_R32f;
        textureTarget->InitAutoFormat(viewportSize.X, viewportSize.Y);
        textureTarget->ClearColor = FLinearColor::Blue;
        textureTarget->UpdateResource();
        
        this->sceneCaptureComponent = Cast<USceneCaptureComponent2D>(this->sceneCapture->GetComponentByClass(USceneCaptureComponent2D::StaticClass()));
        if(this->sceneCaptureComponent != nullptr)
            this->sceneCaptureComponent->TextureTarget = textureTarget;
    }

	// ワールドに存在するタグ付きのアクターを取得 (TArray<AActor*> actors)
	UGameplayStatics::GetAllActorsWithTag(this->GetWorld(), "MetaballObject", this->actors);

    if(0 < this->actors.Num())
    {
        // テクスチャの作成 
        int width = this->actors.Num();
        this->texture = UTexture2D::CreateTransient(width, 1, PF_A32B32G32R32F);

        // マテリアルのインスタンス
        this->materialInstance = UMaterialInstanceDynamic::Create(this->material, nullptr);

        // マテリアルにテクスチャをバインド
        this->materialInstance->SetFlags(RF_Transient);
        this->materialInstance->SetScalarParameterValue(FName(TEXT("ActorsNum")), this->actors.Num());
        this->materialInstance->SetTextureParameterValue(FName(TEXT("MetaballBuffer")), this->texture);

        if(this->sceneCaptureComponent != nullptr)
            this->materialInstance->SetTextureParameterValue(FName(TEXT("SceneDepth")), this->sceneCaptureComponent->TextureTarget);

        // 各アクターのStaticMeshComponentのマテリアルを差し替える
        for(int i=0 ; i<this->actors.Num() ; ++i)
        {
            auto actor = this->actors[i];
            auto component = Cast<UStaticMeshComponent>(actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

            if(component != nullptr)
            {
                for(int32 k=0 ; k<component->GetNumMaterials() ; k++)
                {
                    component->SetMaterial(k, this->materialInstance);
                }
            }
        }
    }
}

void UTransformTextureWriterComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if(this->texture != nullptr)
    {
        this->texture->ConditionalBeginDestroy();
        this->texture = nullptr;
    }

    if(this->materialInstance != nullptr)
    {
        this->materialInstance->ConditionalBeginDestroy();
        this->materialInstance = nullptr;
    }

    if(this->sceneCaptureComponent != nullptr)
    {
        this->sceneCaptureComponent->TextureTarget->ConditionalBeginDestroy();
        this->sceneCaptureComponent = nullptr;
    }

	Super::EndPlay(EndPlayReason);
}

#pragma optimize("", off)
void UTransformTextureWriterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 座標更新
    if(this->texture != nullptr)
    {
        // テクスチャに書き込む用のバッファを取得
        volatile float* mappedPtr = (float*)this->texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

        // テクスチャに各アクターの座標を書き込む
        for(int i=0 ; i<actors.Num() ; ++i)
        {
            FVector position = actors[i]->GetTransform().GetLocation();
            *mappedPtr = position.X; mappedPtr++; // R
            *mappedPtr = position.Y; mappedPtr++; // G
            *mappedPtr = position.Z; mappedPtr++; // B
            *mappedPtr = i; mappedPtr++; // A
        }

        // テクスチャのアンロック
        this->texture->PlatformData->Mips[0].BulkData.Unlock();
        this->texture->UpdateResource();
    }
}
#pragma optimize("", on) 
