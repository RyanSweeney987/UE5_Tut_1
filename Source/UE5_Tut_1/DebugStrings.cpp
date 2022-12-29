// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugStrings.h"

#include "Engine/Canvas.h"


FDebugSceneProxy::FDebugSceneProxy(const UPrimitiveComponent* InComponent, FDebugSceneProxyData* ProxyData)
 : FDebugRenderSceneProxy(InComponent)
{
	DrawType = EDrawType::SolidAndWireMeshes;
	ViewFlagName = TEXT("DebugText");

	this->ProxyData = *ProxyData;

	for(const auto& Text : ProxyData->DebugLabels)
	{
		this->Texts.Add({
			Text.Text,
			Text.Location,
			FColor::White
		});
	}
}

void FDebugTextDelegateHelper::DrawDebugLabels(UCanvas* Canvas, APlayerController* PlayerController)
{
	if(!Canvas) return;

	const FColor OldDrawColor = Canvas->DrawColor;
	Canvas->SetDrawColor(FColor::White);
	const FSceneView* View = Canvas->SceneView;
	const UFont* Font = GEngine->GetSmallFont();
	const FDebugSceneProxyData::FDebugText* DebugText = DebugLabels.GetData();

	for(int32 i = 0; i < DebugLabels.Num(); ++i, ++DebugText)
	{
		if(View->ViewFrustum.IntersectSphere(DebugText->Location, 1.0f))
		{
			const FVector ScreenLoc = Canvas->Project(DebugText->Location);
			Canvas->DrawText(Font, DebugText->Text, ScreenLoc.X, ScreenLoc.Y);
		}
	}

	Canvas->SetDrawColor(OldDrawColor);
}

void FDebugTextDelegateHelper::SetupFromProxy(const FDebugSceneProxy* InSceneProxy)
{
	DebugLabels.Reset();
	DebugLabels.Append(InSceneProxy->ProxyData.DebugLabels);
} 

UDebugStrings::UDebugStrings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FEngineShowFlags::RegisterCustomShowFlag(TEXT("DebugText"), false, SFG_Normal,
		FText::FromString("Debug Text"));
}

FDebugRenderSceneProxy* UDebugStrings::CreateDebugSceneProxy()
{
	FDebugSceneProxyData ProxyData;
	ProxyData.DebugLabels.Add({
		FVector(0, 0, 100),
		TEXT("Hello World")
	});

	FDebugSceneProxy* DebugSceneProxy = new FDebugSceneProxy(this, &ProxyData);
	DebugDrawDelegateManager.SetupFromProxy(DebugSceneProxy);

	return DebugSceneProxy;
}

FBoxSphereBounds UDebugStrings::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds(FBox(FVector(-1000, -1000, -1000), FVector(1000, 1000, 1000)));
}
