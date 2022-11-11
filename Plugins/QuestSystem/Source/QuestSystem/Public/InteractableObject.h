// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableObject : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteractionFinished, AActor* , AActor*);
class QUESTSYSTEM_API IInteractableObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(AActor* InteractInstigator);

	UFUNCTION()
	virtual void Interact_Implementation(AActor* InteractInstigator) {};

	virtual void NotifyInteractionFinished(AActor* InteractObject, AActor* InteractInstigator)
	{
			OnInteractionFinished.Broadcast(InteractObject, InteractInstigator);
	}

	FOnInteractionFinished OnInteractionFinished;
};
