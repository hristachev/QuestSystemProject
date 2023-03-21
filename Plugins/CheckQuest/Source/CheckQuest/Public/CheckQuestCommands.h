// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CheckQuestStyle.h"

class FCheckQuestCommands : public TCommands<FCheckQuestCommands>
{
public:

	FCheckQuestCommands()
		: TCommands<FCheckQuestCommands>(TEXT("CheckQuest"), NSLOCTEXT("Contexts", "CheckQuest", "CheckQuest Plugin"), NAME_None, FCheckQuestStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};