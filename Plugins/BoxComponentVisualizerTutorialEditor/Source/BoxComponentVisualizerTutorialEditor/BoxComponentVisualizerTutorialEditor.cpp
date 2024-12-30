// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoxComponentVisualizerTutorialEditor.h"

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"

#define LOCTEXT_NAMESPACE "FBoxComponentVisualizerTutorialEditorModule"

void FBoxComponentVisualizerTutorialEditorModule::StartupModule()
{
}

void FBoxComponentVisualizerTutorialEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBoxComponentVisualizerTutorialEditorModule, BoxComponentVisualizerTutorialEditor)