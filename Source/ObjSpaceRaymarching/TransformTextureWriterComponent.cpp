#include "TransformTextureWriterComponent.h"
#include "Kismet/GameplayStatics.h"

UTransformTextureWriterComponent::UTransformTextureWriterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTransformTextureWriterComponent::BeginPlay()
{
	Super::BeginPlay();
	
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
    this->texture->ConditionalBeginDestroy();
    this->materialInstance->ConditionalBeginDestroy();
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
