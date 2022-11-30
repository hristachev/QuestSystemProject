// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "UObject/Object.h"
#include "CollectableActor.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API ACollectableActor : public AActor , public ICollectable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void OnCollect(AActor* CollectInstigator)
	{
		NotifyCollectedActor(this, CollectInstigator);
	}
};
