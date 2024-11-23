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

    // Function to select hex under cursor
    void SelectHexUnderCursor();
};
