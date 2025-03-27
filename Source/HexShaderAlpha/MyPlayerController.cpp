#include "MyPlayerController.h"

// deprecated 5.3?
// #include "Materials/MaterialUpdateContext.h"
#include "RHI.h"
#include "RendererInterface.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Math/MathFwd.h"

#include "Kismet/GameplayStatics.h"
#include "Landscape.h"        // For ALandscape
#include "LandscapeProxy.h"   // For ALandscapeProxy


#include "Engine/World.h"
#include "DrawDebugHelpers.h"


AMyPlayerController::AMyPlayerController()
{
    UE_LOG(LogTemp, Warning, TEXT("===> AMyPlayerController::Constructor() !!!"));
    
    PrimaryActorTick.bCanEverTick = true;

    // Enable the mouse cursor
    bShowMouseCursor = true;

    // Set the cursor to interactable mode (optional)
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/M_TestCPPTexture.M_TestCPPTexture"));
    if (MaterialFinder.Succeeded())
    {
        UE_LOG(LogTemp, Warning, TEXT("===> inline Found material!"));
        HexMaterial = MaterialFinder.Object;
    } else
    {
        UE_LOG(LogTemp, Error, TEXT("===> Failed to find material!"));
    }

}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

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

    // SetHexMaterial();

}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    // Perform selection when the left mouse button is pressed
    if (IsInputKeyDown(EKeys::LeftMouseButton))
    {
        SelectHexUnderCursor();
        // SetHexMaterial();
    }
}

void AMyPlayerController::SelectHexUnderCursor()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

    FMaterialUpdateContext UpdateContext;
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


// /Script/Engine.Material'/Game/M_TestCPPTexture.M_TestCPPTexture'
int AMyPlayerController::SetHexMaterial()
{
    UE_LOG(LogTemp, Warning, TEXT("===> AMyPlayerController::SetHexMaterial()!!"));

    ALandscape* MyLandscape = Cast<ALandscape>(UGameplayStatics::GetActorOfClass(GetWorld(), ALandscape::StaticClass()));
#if WITH_EDITOR    
    if (MyLandscape)
    {
        UE_LOG(LogTemp, Warning, TEXT("===> Found landscape! FName: %s, ActorLabel: %s, FullName: %s, PathName: %s Class: %s"),
            *MyLandscape->GetFName().ToString(), *MyLandscape->GetActorLabel(), *MyLandscape->GetFullName(), *MyLandscape->GetPathName(), *MyLandscape->GetClass()->GetName());
    } else {
        UE_LOG(LogTemp, Error, TEXT("===> Failed to find landscape!"));
        return 0;
    }

    if (!HexMaterial)
    {
        UE_LOG(LogTemp, Error, TEXT("===> HexMaterial is NULL!"));
        return 0;
    }
#endif

    const TArray<ULandscapeComponent*>& Components = MyLandscape->LandscapeComponents;
    if (Components.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("===> No landscape components found!"));
        return 0;        
    }


    ULandscapeComponent* FirstComp = Components[0];

    // Create a dynamic material instance so we can set parameters at runtime
    // UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(HexMaterial, this);
    // UMaterialInstanceDynamic* MID = MyLandscape->LandscapeComponents[0]->CreateDynamicMaterialInstance(0, MyLandscape->LandscapeMaterial);
    // if (!MID)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("Failed to create MID."));
    //     return 0;
    // }

    // FirstComp->SetMaterial(0, MID);
    // UE_LOG(LogTemp, Warning, TEXT("===> Set material on landscape! (Num: %d)"), Components.Num());

    TArray<UActorComponent*> FoundComponents;
    MyLandscape->GetComponents(ULandscapeComponent::StaticClass(), FoundComponents);
    UE_LOG(LogTemp, Log, TEXT("Found %d landscape components"), FoundComponents.Num());

    // Print landscape material MyLandscape->LandscapeMaterial
    UMaterialInterface* LandscapeMaterial = MyLandscape->LandscapeMaterial;
    if (LandscapeMaterial)
    {
        UE_LOG(LogTemp, Log, TEXT("===> BEFORE LandscapeMaterial: %s"), *LandscapeMaterial->GetName());
    } else
    {
        UE_LOG(LogTemp, Error, TEXT("===> LandscapeMaterial is NULL!"));
    }

    // MyLandscape->LandscapeMaterial = MID;


    
    ALandscapeProxy* Proxy = Cast<ALandscapeProxy>(MyLandscape);
    if (Proxy)
    {
        UE_LOG(LogTemp, Log, TEXT("Proxy found: %s with %d components"), *Proxy->GetName(), Proxy->LandscapeComponents.Num());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MyLandscape is not a LandscapeProxy?"));
    }

    // Array to hold landscape components
    TArray<ULandscapeComponent*> LandscapeComponents;

    // Retrieve all ULandscapeComponent instances attached to MyLandscape
    MyLandscape->GetComponents<ULandscapeComponent>(LandscapeComponents);

    // Iterate over each landscape component
    for (ULandscapeComponent* LandscapeComponent : LandscapeComponents)
    {
        if (LandscapeComponent)
        {
            // Get the current material applied to the landscape component
            UMaterialInterface* CurrentMaterial = LandscapeComponent->GetMaterial(0);
            if (!CurrentMaterial)
            {
                UE_LOG(LogTemp, Warning, TEXT("LandscapeComponent has no material assigned."));
                continue;
            }
            UMaterialInstanceDynamic* ExistingDynamicMaterial = Cast<UMaterialInstanceDynamic>(CurrentMaterial);
            if (ExistingDynamicMaterial)
            {
                UE_LOG(LogTemp, Warning, TEXT("===> ExistingDynamicMaterial found!"));
                continue;
            } else {
                UE_LOG(LogTemp, Warning, TEXT("===> ExistingDynamicMaterial NOT found!"));
            }

            // Create a dynamic material instance from the current material
            UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CurrentMaterial, this);
            if (!DynamicMaterial)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create dynamic material instance!"));
                continue;
            }

            // Assign the dynamic material to the landscape component
            LandscapeComponent->SetMaterial(0, DynamicMaterial);
        }
    }

    // MyLandscape->RecreateComponentsState();        
    // TArray<FComponentRecreateRenderStateContext> RecreateRenderStateContexts;
    // MyLandscape->UpdateAllComponentMaterialInstances(UpdateContext, RecreateRenderStateContexts, true);
    /*
Getting this error please help:
UATHelper: Packaging (Windows): C:\Users\indig\Documents\Unreal Projects\HexShaderAlpha\Source\HexShaderAlpha\MyPlayerController.cpp(228): error C2039: 'UpdateAllComponentMaterialInstances': is not a member of 'ALandscape'
UATHelper: Packaging (Windows): C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\Landscape\Classes\Landscape.h(208): note: see declaration of 'ALandscape'
*/


    UMaterialInterface* LandscapeMaterialAfter = MyLandscape->LandscapeMaterial;
    if (LandscapeMaterialAfter)
    {
        UE_LOG(LogTemp, Log, TEXT("===> AFTER LandscapeMaterial: %s"), *LandscapeMaterialAfter->GetName());
    } else
    {
        UE_LOG(LogTemp, Error, TEXT("===> LandscapeMaterial is NULL!"));
    }


    // TActorIterator<ALandscapeProxy> It(GetWorld());
    // for (; It; ++It)
    // {
    //     ALandscapeProxy* Proxy = *It;
    //     UE_LOG(LogTemp, Log, TEXT("===> Proxy: %s, Components: %d"), *Proxy->GetName(), Proxy->LandscapeComponents.Num());
    // }



    // HexMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/M_TestCPPTexture.M_TestCPPTexture"));
    // if (HexMaterial)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("===> HexMaterial is valid!"));
    // }
    // else
    // {
    //     UE_LOG(LogTemp, Error, TEXT("===> HexMaterial is NULL!"));
    // }
    return 1;
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
