// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "CurrentObjectives.generated.h"

class UVerticalBox;
class UObjectiveWidget;
class AQuest;
/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UCurrentObjectives : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetCurrentObjectives(AQuest* Quest);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UObjectiveWidget> ObjectiveWidgetClass;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UVerticalBox* ObjectivesList;
};
