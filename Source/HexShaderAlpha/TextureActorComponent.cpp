// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureActorComponent.h"
#include "RHICommandList.h"
#include "Rendering/Texture2DResource.h"

// Sets default values for this component's properties
UTextureActorComponent::UTextureActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTextureActorComponent::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("UTextureActorComponent::BeginPlay()"));
	InitializeTexture();
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTextureActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateTexture();
	if (DynamicMaterial) {
		UE_LOG(LogTemp, Warning, TEXT("Setting DynamicMaterial parameter"));
		DynamicMaterial->SetTextureParameterValue("DynamicTexture", DynamicTexture);
	}
}

void UTextureActorComponent::InitializeTexture()
{
	UE_LOG(LogTemp, Warning, TEXT("UTextureActorComponent::InitializeTexture()"));
	TextureTotalPixels = TextureWidth * TextureHeight;
	TextureDataSize = TextureTotalPixels * 4;
	TextureDataSqrtSize = TextureWidth * 4;
	TextureData = new uint8[TextureDataSize];

	DynamicTexture = UTexture2D::CreateTransient(TextureWidth, TextureHeight, EPixelFormat::PF_B8G8R8A8, "My Dynamic Texture");
	DynamicTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	DynamicTexture->SRGB = 0;
	DynamicTexture->Filter = TextureFilter::TF_Nearest;
	DynamicTexture->AddToRoot();
	DynamicTexture->UpdateResource();

	TextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, TextureWidth, TextureHeight);

	FillTexture(FLinearColor::Black);
	DrawRectangle(0, 0, 100, 100, FLinearColor::Red);


}

void UTextureActorComponent::UpdateTexture(bool bFreeData) {
	if (DynamicTexture == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Dynamic Texture failed to load!"));
		return;
	}

	struct FUpdateTextureRegionsData {
		FTexture2DResource* Texture2DResource;
		FRHITexture2D* TextureRHI;
		int32 MipIndex;
		uint32 NumRegions;
		FUpdateTextureRegion2D* Regions;
		uint32 SrcPitch;
		uint32 SrcBpp;
		uint8* SrcData;
	};

	FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;
	UTexture2D* Texture = DynamicTexture;

	RegionData->Texture2DResource = (FTexture2DResource*)Texture->GetResource();
	RegionData->TextureRHI = RegionData->Texture2DResource->GetTexture2DRHI();
	RegionData->MipIndex = 0;
	RegionData->NumRegions = 1;
	RegionData->Regions = TextureRegion;
	RegionData->SrcPitch = TextureDataSqrtSize;
	RegionData->SrcBpp = 4;
	RegionData->SrcData = TextureData;

	ENQUEUE_RENDER_COMMAND(UpdateTextureRegionsData)(
		[RegionData, bFreeData, Texture](FRHICommandListImmediate& RHICmdList) {
			for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex) {
				int32 CurrentFirstMip = Texture->FirstResourceMemMip;
				if (RegionData->TextureRHI && RegionData->MipIndex >= CurrentFirstMip) {
					RHIUpdateTexture2D(
						RegionData->TextureRHI,
						RegionData->MipIndex - CurrentFirstMip,
						RegionData->Regions[RegionIndex],
						RegionData->SrcPitch,
						RegionData->SrcData + RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch + RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
					);
				}
			}
			if (bFreeData) {
				FMemory::Free(RegionData->Regions);
				FMemory::Free(RegionData->SrcData);
			}
			delete RegionData;

		}
	);

}

void UTextureActorComponent::FillTexture(FLinearColor Color)
{
	for (uint32 i = 0; i < TextureTotalPixels; i++) {
		TextureData[i*4] = Color.B * 255;
		TextureData[i*4 + 1] = Color.G * 255;
		TextureData[i*4 + 2] = Color.R * 255;
		TextureData[i*4 + 3] = Color.A * 255;
	}

}

void UTextureActorComponent::SetPixelColor(int32 X, int32 Y, FLinearColor Color) {
	uint32 start = ((Y * TextureWidth) + X) * 4;
	if (start < 0 || start > TextureDataSize - 4) {
		return;
	}
	TextureData[start] = Color.B * 255;
	TextureData[start + 1] = Color.G * 255;
	TextureData[start + 2] = Color.R * 255;
	TextureData[start + 3] = Color.A * 255;
}

void UTextureActorComponent::DrawRectangle(int32 X, int32 Y, int32 Width, int32 Height, FLinearColor Color)
{
	for (int32 y = 0; y < Height; y++) {
		for (int32 x = 0; x < Width; x++) {
			SetPixelColor(X + x, Y + y, Color);
		}
	}
}
