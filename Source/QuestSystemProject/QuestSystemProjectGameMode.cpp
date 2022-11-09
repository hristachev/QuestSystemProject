// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestSystemProjectGameMode.h"
#include "QuestSystemProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AQuestSystemProjectGameMode::AQuestSystemProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
