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
    UBoxComponent* BoxComponent = Cast<UBoxComponent>(BoxPropertyPath.GetComponent());
    if (BoxComponent == nullptr) { return false; }
    if (SelectedSide == EBoxSide::NONE) { return false; }

    FVector BoxExtent = BoxComponent->GetScaledBoxExtent();
    FVector Location = BoxComponent->GetComponentLocation();

    FVector TransformedVec = BoxComponent->GetComponentTransform().InverseTransformVector(DeltaTranslate);
    if (SelectedSide == EBoxSide::FRONT)
    {
        BoxExtent.X += TransformedVec.X / 2.f;
        Location += BoxComponent->GetForwardVector() * (TransformedVec.X / 2.f);
    }
    else if (SelectedSide == EBoxSide::BACK)
    {
        BoxExtent.X -= TransformedVec.X / 2.f;
        Location += BoxComponent->GetForwardVector() * (TransformedVec.X / 2.f);
    }
    else if (SelectedSide == EBoxSide::RIGHT)
    {
        BoxExtent.Y += TransformedVec.Y / 2.f;
        Location += BoxComponent->GetRightVector() * (TransformedVec.Y / 2.f);
    }
    else if (SelectedSide == EBoxSide::LEFT)
    {
        BoxExtent.Y -= TransformedVec.Y / 2.f;
        Location += BoxComponent->GetRightVector() * (TransformedVec.Y / 2.f);
    }
    else if (SelectedSide == EBoxSide::TOP)
    {
        BoxExtent.Z += TransformedVec.Z / 2.f;
        Location += BoxComponent->GetUpVector() * (TransformedVec.Z / 2.f);
    }
    else if (SelectedSide == EBoxSide::BOTTOM)
    {
        BoxExtent.Z -= TransformedVec.Z / 2.f;
        Location += BoxComponent->GetUpVector() * (TransformedVec.Z / 2.f);
    }

    BoxComponent->SetWorldLocation(Location);
    BoxComponent->SetBoxExtent(BoxExtent, false);
    GEditor->RedrawLevelEditingViewports(true);
    return true;
}

void FBoxComponentVisualizer::EndEditing()
{
    SelectedSide = EBoxSide::NONE;
    BoxPropertyPath.Reset();
}
