// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "ToKillInterface.h"
#include "GameFramework/Actor.h"
#include "KillingActor.generated.h"

UCLASS()
class QUESTSYSTEM_API AKillingActor : public AInteractableActor, public IToKillInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void OnKilled(AActor* InteractInstigator)
	{
		NotifyKilled(this, InteractInstigator);
	}
};
