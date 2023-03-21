// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestDescription.generated.h"

class UObjectiveWidget;
class UScrollBox;
class UTextBlock;
class AQuest;
/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestDescription : public UUserWidget
{
	GENERATED_BODY()

public:

	void Init(AQuest* Quest);

protected:

	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta=(BindWidget))
	UScrollBox* ObjectiveList;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DescriptionText;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UObjectiveWidget> ObjectiveWidgetClass;
};
