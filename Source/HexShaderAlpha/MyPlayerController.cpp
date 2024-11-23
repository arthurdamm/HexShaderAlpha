#include "MyPlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

void AMyPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    // Perform selection when the left mouse button is pressed
    if (IsInputKeyDown(EKeys::LeftMouseButton))
    {
        SelectHexUnderCursor();
    }
}

void AMyPlayerController::SelectHexUnderCursor()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

    UE_LOG(LogTemp, Warning, TEXT("===> AMyPlayerController::SelectHexUnderCursor()"));

    if (HitResult.IsValidBlockingHit())
    {
        FVector HitLocation = HitResult.Location;

        // Log the hit location for debugging
        UE_LOG(LogTemp, Warning, TEXT("===> Hit Location: %s"), *HitLocation.ToString());
        DrawDebugSphere(GetWorld(), HitResult.Location, 50.0f, 12, FColor::Red, false, 2.0f);


        // Here, you can call additional functions to process the selected hex
    }
}
