#pragma once

#include "ComponentVisualizer.h"

UENUM()
enum class EBoxSide : uint8{
    NONE,
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

class FBoxComponentVisualizer : public FComponentVisualizer
{
public:

    // Begin FComponentVisualizer interface
    virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
    
   	virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;

    virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
    virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
	virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override;
    virtual void EndEditing() override;

    // End FComponentVisualizer interface

protected:

	/** Property path from the parent actor to the component */
	FComponentPropertyPath BoxPropertyPath;

    EBoxSide SelectedSide = EBoxSide::NONE;
};