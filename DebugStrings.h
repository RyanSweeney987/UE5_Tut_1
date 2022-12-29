// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Debug/DebugDrawComponent.h"
#include "DebugStrings.generated.h"

struct FDebugSceneProxyData
{
	struct FDebugText
	{
		FVector Location;
		FString Text;

		FDebugText() {}
		FDebugText(const FVector& InLocation, const FString& InText) : Location(InLocation), Text(InText){}
	};

	TArray<FDebugText> DebugLabels;
};

class FDebugSceneProxy : public FDebugRenderSceneProxy
{
public:
	FDebugSceneProxy(const UPrimitiveComponent* InComponent, FDebugSceneProxyData* ProxyData);

	FDebugSceneProxyData ProxyData;
};

class FDebugTextDelegateHelper : public FDebugDrawDelegateHelper
{
public:
	virtual void DrawDebugLabels(UCanvas* Canvas, APlayerController*) override;

	void SetupFromProxy(const FDebugSceneProxy* InSceneProxy);

	TArray<FDebugSceneProxyData::FDebugText> DebugLabels;
};

/**
 * 
 */
UCLASS(ClassGroup = Custom, meta = (BlueprintSpawnableComponent))
class UE5_TUT_1_API UDebugStrings : public UDebugDrawComponent
{
	GENERATED_BODY()

public:
	UDebugStrings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual FDebugRenderSceneProxy* CreateDebugSceneProxy() override;
	virtual FDebugDrawDelegateHelper& GetDebugDrawDelegateHelper() override { return DebugDrawDelegateManager; }
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	
	FDebugTextDelegateHelper DebugDrawDelegateManager;
};
