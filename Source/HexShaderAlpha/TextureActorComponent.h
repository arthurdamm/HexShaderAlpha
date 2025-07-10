// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TextureActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DisplayName="My Texture Actor Component" )
class HEXSHADERALPHA_API UTextureActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTextureActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	uint8* TextureData;
	int32 TextureDataSize;
	int32 TextureDataSqrtSize;
	int32 TextureTotalPixels;

	UPROPERTY()
	UTexture2D* DynamicTexture;

	FUpdateTextureRegion2D* TextureRegion;

	void InitializeTexture();

	void UpdateTexture(bool bFreeData = false);

public:
	UPROPERTY(EditDefaultsOnly)
	int32 TextureWidth = 512;

	UPROPERTY(EditDefaultsOnly)
	int32 TextureHeight = 512;

	UPROPERTY(BlueprintReadWrite)
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditDefaultsOnly)
	FName DynamicMaterialParamName = "DynamicTexture";

	UFUNCTION(BlueprintCallable, Category = "Dynamic Texture")
	void FillTexture(FLinearColor Color);

	UFUNCTION(BlueprintCallable, Category = "Dynamic Texture")
	void SetPixelColor(int32 X, int32 Y, FLinearColor Color);

	UFUNCTION(BlueprintCallable, Category = "Dynamic Texture")
	void DrawRectangle(int32 X, int32 Y, int32 Width, int32 Height, FLinearColor Color);

};
