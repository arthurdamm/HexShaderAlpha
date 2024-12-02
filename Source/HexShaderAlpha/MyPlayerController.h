// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HEXSHADERALPHA_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    virtual void PlayerTick(float DeltaTime) override;
    virtual void BeginPlay() override;

    // Function to select hex under cursor
    void SelectHexUnderCursor();

    float size = 100.0f;

private:
   UMaterialParameterCollectionInstance* MPCInstance;
};

FVector2D axial_round_branchless(FVector2D hex);
FVector2D pixel_to_pointy_hex(FVector point, float size);