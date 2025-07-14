// Fill out your copyright notice in the Description page of Project Settings.


#include "HexLandscapeMaterialComponent.h"

#include "Landscape.h"        // For ALandscape
#include "LandscapeProxy.h"   // For ALandscapeProxy

// Sets default values for this component's properties
UHexLandscapeMaterialComponent::UHexLandscapeMaterialComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHexLandscapeMaterialComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("UHexLandscapeMaterialComponent::BeginPlay!()"));

	ALandscape* Landscape = Cast<ALandscape>(GetOwner());
	if (Landscape) {
		UE_LOG(LogTemp, Warning, TEXT("Owner: %s"), *Landscape->GetFName().ToString());
	} else {
		UE_LOG(LogTemp, Warning, TEXT("NO OWNER!"));
		return;
	}

	// auto Proxies = Landscape->GetLandscapeProxies();
	// TArray<TWeakObjectPtr<ALandscapeProxy>> Proxies = Landscape->GetLandscapeProxies();
	// for (auto Proxy: Landscape->GetLandscapeProxies()) {
	// 	UE_LOG(LogTemp, Warning, TEXT("Proxy found: %s with %d components"), *Proxy->GetName(), Proxy->LandscapeComponents.Num());
	// }
	// UE_LOG(LogTemp, Error, TEXT("Proxies len: %d"), Proxies.Num());
	// for (auto Proxy : Proxies) {
	// 	// UE_LOG(LogTemp, Warning, TEXT("Proxy Components: %d"), Proxy->LandscapeComponents.Num());
	// }
	
}


// Called every frame
void UHexLandscapeMaterialComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

