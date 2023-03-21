// Copyright Epic Games, Inc. All Rights Reserved.

#include "CheckQuestCommands.h"

#define LOCTEXT_NAMESPACE "FCheckQuestModule"

void FCheckQuestCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "CheckQuest", "Bring up CheckQuest window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
