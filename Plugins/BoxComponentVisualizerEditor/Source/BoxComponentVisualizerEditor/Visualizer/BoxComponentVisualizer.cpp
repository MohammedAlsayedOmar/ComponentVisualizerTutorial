// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoxComponentVisualizer.h"

#include "Components/BoxComponent.h"
#include "Engine/Font.h"
#include "CanvasTypes.h"
#include "CanvasItem.h"


/** Proxy for Box component */
struct HBoxComponentVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY();

	HBoxComponentVisProxy(const UActorComponent* InComponent, EBoxSide boxSide)
	: HComponentVisProxy(InComponent, HPP_Wireframe)
    , Side(boxSide)
	{}

	EBoxSide Side;
};

IMPLEMENT_HIT_PROXY(HBoxComponentVisProxy, HComponentVisProxy);

void FBoxComponentVisualizer::DrawVisualization(
    const UActorComponent* Component, 
    const FSceneView* View, 
    FPrimitiveDrawInterface* PDI)
{
    const UBoxComponent* comp = Cast<const UBoxComponent>(Component);
    if (comp == nullptr) 
    { 
        EndEditing();
        return; 
    }

    PDI->SetHitProxy(NULL);
    FVector boxExtent = comp->GetScaledBoxExtent();

    FVector pointLocation = comp->GetComponentLocation() + comp->GetForwardVector() * boxExtent.X;
    PDI->SetHitProxy(new HBoxComponentVisProxy(Component, EBoxSide::FRONT));
    PDI->DrawPoint(
        pointLocation,
        FColor::Red,
        5.f, 
        SDPG_World);
    PDI->SetHitProxy(NULL);

        
    pointLocation = comp->GetComponentLocation() - comp->GetForwardVector() * boxExtent.X;
    PDI->SetHitProxy(new HBoxComponentVisProxy(Component, EBoxSide::BACK));
    PDI->DrawPoint(
        pointLocation,
        FColor::Red,
        5.f, 
        SDPG_World);
    PDI->SetHitProxy(NULL);

    pointLocation = comp->GetComponentLocation() + comp->GetRightVector() * boxExtent.Y;
    PDI->SetHitProxy(new HBoxComponentVisProxy(Component, EBoxSide::RIGHT));
    PDI->DrawPoint(
        pointLocation,
        FColor::Red,
        5.f, 
        SDPG_World);
    PDI->SetHitProxy(NULL);

    pointLocation = comp->GetComponentLocation() - comp->GetRightVector() * boxExtent.Y;
    PDI->SetHitProxy(new HBoxComponentVisProxy(Component, EBoxSide::LEFT));
    PDI->DrawPoint(
        pointLocation,
        FColor::Red,
        5.f, 
        SDPG_World);
    PDI->SetHitProxy(NULL);

    pointLocation = comp->GetComponentLocation() + comp->GetUpVector() * boxExtent.Z;
    PDI->SetHitProxy(new HBoxComponentVisProxy(Component, EBoxSide::TOP));
    PDI->DrawPoint(
        pointLocation,
        FColor::Red,
        5.f, 
        SDPG_World);
    PDI->SetHitProxy(NULL);

    pointLocation = comp->GetComponentLocation() - comp->GetUpVector() * boxExtent.Z;
    PDI->SetHitProxy(new HBoxComponentVisProxy(Component, EBoxSide::BOTTOM));
    PDI->DrawPoint(
        pointLocation,
        FColor::Red,
        5.f, 
        SDPG_World);
    PDI->SetHitProxy(NULL);
}

void FBoxComponentVisualizer::DrawVisualizationHUD(
    const UActorComponent* Component, 
    const FViewport* Viewport, 
    const FSceneView* View, 
    FCanvas* Canvas) 
{
    const UBoxComponent* comp = Cast<const UBoxComponent>(Component);
    if (comp == nullptr) { return; }

    UFont* LargeFont = GEngine->GetLargeFont();
	const float MaxLargeFontCharHeight = LargeFont->GetMaxCharHeight();

	// const FVector2f CurrentValuePosition = DrawParams.ClockPosition + FVector2f(GGameplayCamerasDebugClockPadding);
	// FCanvasTextItem TextItem(
	// 		FVector2D(CurrentValuePosition),
	// 		CurrentValueStr,
	// 		LargeFont,
	// 		DrawParams.ClockValueLineColor);
	// Canvas->DrawItem(TextItem);

    const FIntRect CanvasRect = Canvas->GetViewRect();
    const float DrawPositionX = CanvasRect.Min.X + (CanvasRect.Width() * 0.5f);
    const float DrawPositionY = CanvasRect.Max.Y - MaxLargeFontCharHeight;
    
    FCanvasTextItem TextItem(
        FVector2D(DrawPositionX, DrawPositionY), 
        FText::FromString(FString::Printf(
             TEXT("Selected Face: %s"), *UEnum::GetValueAsName(SelectedSide).ToString())), 
        LargeFont,
        FLinearColor::Black);
	Canvas->DrawItem(TextItem);

    // Canvas->DrawShadowedString(
    //     DrawPositionX, DrawPositionY,
    //     FString::Printf(
    //         TEXT("TEST DISPLAY THIS!: %s"), *UEnum::GetValueAsName(SelectedSide).ToString()),
    //     LargeFont,
    //     FLinearColor::Yellow
    // );
        
   
    // auto DisplaySnapToActorHelpText = [&](const FText& SnapHelpText)
    // {
    //     int32 XL;
    //     int32 YL;
    //     StringSize(GEngine->GetLargeFont(), XL, YL, *SnapHelpText.ToString());
    //     const float DrawPositionX = FMath::FloorToFloat(CanvasRect.Min.X + (CanvasRect.Width() - XL) * 0.5f);
    //     const float DrawPositionY = CanvasRect.Min.Y + 50.0f;
    //     Canvas->DrawShadowedString(DrawPositionX, DrawPositionY, *SnapHelpText.ToString(), GEngine->GetLargeFont(), FLinearColor::Yellow);
    // };
}


bool FBoxComponentVisualizer::VisProxyHandleClick(FEditorViewportClient *InViewportClient, HComponentVisProxy *VisProxy, const FViewportClick &Click)
{
    if (!VisProxy || !VisProxy->Component.IsValid())
    {
        return false;
    }

	bool bEditing = false;
	const UBoxComponent* comp = CastChecked<const UBoxComponent>(VisProxy->Component.Get());
    BoxPropertyPath = FComponentPropertyPath(comp);
	if (VisProxy->IsA(HBoxComponentVisProxy::StaticGetType()))
	{
        HBoxComponentVisProxy* Proxy = static_cast<HBoxComponentVisProxy*>(VisProxy);
        SelectedSide = Proxy->Side;

        bEditing = true;
    }
    else
    {
        SelectedSide = EBoxSide::NONE;
        BoxPropertyPath.Reset();
    }


    return bEditing;
}

bool FBoxComponentVisualizer::GetWidgetLocation(const FEditorViewportClient *ViewportClient, FVector &OutLocation) const
{
    const UBoxComponent* comp = Cast<UBoxComponent>(BoxPropertyPath.GetComponent());
    if (comp == nullptr) { return false; }
    if (SelectedSide == EBoxSide::NONE) { return false; }

    FVector boxExtent = comp->GetScaledBoxExtent();

    if (SelectedSide == EBoxSide::FRONT)
    {
        FVector pointLocation = comp->GetComponentLocation() + comp->GetForwardVector() * boxExtent.X;
        OutLocation = pointLocation;
    }
    else if (SelectedSide == EBoxSide::BACK)
    {
        FVector  pointLocation = comp->GetComponentLocation() - comp->GetForwardVector() * boxExtent.X;
        OutLocation = pointLocation;
    }
    else if (SelectedSide == EBoxSide::RIGHT)
    {
        FVector pointLocation = comp->GetComponentLocation() + comp->GetRightVector() * boxExtent.Y;
        OutLocation = pointLocation;
    }
    else if (SelectedSide == EBoxSide::LEFT)
    {
        FVector pointLocation = comp->GetComponentLocation() - comp->GetRightVector() * boxExtent.Y;
        OutLocation = pointLocation;
    }
    else if (SelectedSide == EBoxSide::TOP)
    {
        FVector pointLocation = comp->GetComponentLocation() + comp->GetUpVector() * boxExtent.Z;
        OutLocation = pointLocation;
    }
    else if (SelectedSide == EBoxSide::BOTTOM)
    {
        FVector pointLocation = comp->GetComponentLocation() - comp->GetUpVector() * boxExtent.Z;
        OutLocation = pointLocation;
    }

    return true;
}

bool FBoxComponentVisualizer::HandleInputDelta(FEditorViewportClient *ViewportClient, FViewport *Viewport, FVector &DeltaTranslate, FRotator &DeltaRotate, FVector &DeltaScale)
{
    UBoxComponent* comp = Cast<UBoxComponent>(BoxPropertyPath.GetComponent());
    if (comp == nullptr) { return false; }
    if (SelectedSide == EBoxSide::NONE) { return false; }

    FVector boxExtent = comp->GetScaledBoxExtent();
    FVector location = comp->GetComponentLocation();
    FVector moveDirection = FVector::ZeroVector;
    float deltaTranslate = 0.f;
    FVector TransformedVec = comp->GetComponentTransform().InverseTransformVector(DeltaTranslate);
    if (SelectedSide == EBoxSide::FRONT)
    {
        deltaTranslate = TransformedVec.X / 2.f;
        boxExtent.X += deltaTranslate;
        moveDirection = comp->GetForwardVector();
    }
    else if (SelectedSide == EBoxSide::BACK)
    {
        deltaTranslate = -TransformedVec.X / 2.f;
        boxExtent.X += deltaTranslate;
        moveDirection = -comp->GetForwardVector();
    }
    else if (SelectedSide == EBoxSide::RIGHT)
    {
        deltaTranslate = TransformedVec.Y / 2.f;
        boxExtent.Y += deltaTranslate;
        moveDirection = comp->GetRightVector();
    }
    else if (SelectedSide == EBoxSide::LEFT)
    {
        deltaTranslate = -TransformedVec.Y / 2.f;
        boxExtent.Y += deltaTranslate;
        moveDirection = -comp->GetRightVector();
    }
    else if (SelectedSide == EBoxSide::TOP)
    {
        deltaTranslate = TransformedVec.Z / 2.f;
        boxExtent.Z += deltaTranslate;
        moveDirection = comp->GetUpVector();
    }
    else if (SelectedSide == EBoxSide::BOTTOM)
    {
        deltaTranslate = -TransformedVec.Z / 2.f;
        boxExtent.Z += deltaTranslate;
        moveDirection = -comp->GetUpVector();
    }

    location += moveDirection * deltaTranslate;

    comp->SetWorldLocation(location);
    comp->SetBoxExtent(boxExtent, false);
    GEditor->RedrawLevelEditingViewports(true);
    return true;
}

void FBoxComponentVisualizer::EndEditing()
{
    SelectedSide = EBoxSide::NONE;
    BoxPropertyPath.Reset();
}