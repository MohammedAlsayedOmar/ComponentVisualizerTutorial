// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoxComponentVisualizerEditor.h"

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"

#include "Visualizer/BoxComponentVisualizer.h"
#include "Components/BoxComponent.h"

#define LOCTEXT_NAMESPACE "FBoxComponentVisualizerEditorModule"

void FBoxComponentVisualizerEditorModule::StartupModule()
{
    if (GUnrealEd != NULL) 
    {
        TSharedPtr<FComponentVisualizer> BoxVisualizer = MakeShareable(new FBoxComponentVisualizer);
        GUnrealEd->RegisterComponentVisualizer(UBoxComponent::StaticClass()->GetFName(), BoxVisualizer);
    }
}

void FBoxComponentVisualizerEditorModule::ShutdownModule()
{
     if (GUnrealEd != NULL) 
    {
        GUnrealEd->UnregisterComponentVisualizer(UBoxComponent::StaticClass()->GetFName());
    }
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBoxComponentVisualizerEditorModule, BoxComponentVisualizerEditor)