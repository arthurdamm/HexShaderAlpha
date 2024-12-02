// First, include the necessary headers
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Math/MathFwd.h"


#include "MyPlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"



void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Enable the mouse cursor
    bShowMouseCursor = true;

    // Set the cursor to interactable mode (optional)
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    UE_LOG(LogTemp, Warning, TEXT("===> AMyPlayerController::BeginPlay()"));

    // Find the Material Parameter Collection (MPC) asset in your project
    UMaterialParameterCollection* Collection = LoadObject<UMaterialParameterCollection>(nullptr, TEXT("/Game/MPC_Hex.MPC_Hex"));

    // Get the MPC Instance
    MPCInstance = GetWorld()->GetParameterCollectionInstance(Collection);
    if (MPCInstance)
    {
        MPCInstance->GetScalarParameterValue(TEXT("HexSize"), size);
        UE_LOG(LogTemp, Warning, TEXT("===> MPCInstance is valid!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("===> MPCInstance is NULL!"));
    }

}

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
        // DrawDebugSphere(GetWorld(), HitResult.Location, 50.0f, 12, FColor::Red, false, 2.0f);

        
        // Now, you can update parameters (e.g., a scalar parameter named "MyParameter")
        if (MPCInstance)
        {
            FVector2D hex = pixel_to_pointy_hex(HitLocation, size);
            UE_LOG(LogTemp, Warning, TEXT("Hex: %s"), *hex.ToString());
            MPCInstance->SetScalarParameterValue(TEXT("ActiveHexX"), hex.X);
            MPCInstance->SetScalarParameterValue(TEXT("ActiveHexY"), hex.Y);
        } else
        {
            UE_LOG(LogTemp, Warning, TEXT("MPCInstance is NULL!"));
        }


        // Here, you can call additional functions to process the selected hex
    }
}

FVector2D axial_round_branchless(float x, float y) {
  float xgrid = round(x), ygrid = round(y);
  x -= xgrid, y -= ygrid;
  float dx = round(x + 0.5*y) * (y*y <= x*x);
  float dy = round(y + 0.5*x) * (x*x <= y*y);
  return FVector2D(xgrid + dx, ygrid + dy);
}

FVector2D pixel_to_pointy_hex(FVector point, float size) {
    float q = (sqrt(3.)/3. * point.X  -  1./3. * point.Y) / size;
    float r = (                        2./3. * point.Y) / size;
    return axial_round_branchless(q, r);
}