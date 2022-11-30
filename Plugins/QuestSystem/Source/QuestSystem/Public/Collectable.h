// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Collectable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCollectable : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCollect, AActor*, AActor*);

class QUESTSYSTEM_API ICollectable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Collect(AActor* CollectInstigator);

	virtual void NotifyCollectedActor(AActor* CollectableActor, AActor* CollectInstigator)
	{
		OnCollect.Broadcast(CollectableActor, CollectInstigator);
	}

	FOnCollect OnCollect;
};
