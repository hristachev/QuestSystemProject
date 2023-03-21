// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestDialog.generated.h"

class UQuestDescription;
class UButton;
class AQuest;
/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestDialog : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void Init(AQuest* Quest);
	
	FSimpleDelegate OnQuestAccepted;

	FSimpleDelegate OnDialogExit;

protected:

	UPROPERTY(meta=(BindWidget))
	UQuestDescription* Description;
	
	UPROPERTY(meta=(BindWidget))
	UButton* AcceptButton;

	UPROPERTY(meta=(BindWidget))
	UButton* RejectButton;

	UFUNCTION()
	void OnAccepted();

	UFUNCTION()
	void OnRejected();

	void HideDialog();
};
