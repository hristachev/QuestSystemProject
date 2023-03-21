// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "InteractableActor.h"
#include "GameFramework/Actor.h"
#include "CollectActor.generated.h"

UCLASS()
class QUESTSYSTEM_API ACollectActor : public AInteractableActor, public ICollectable
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void OnCollect(AActor* InteractInstigator)
	{
		NotifyCollectedActor(this, InteractInstigator);
	}
};
