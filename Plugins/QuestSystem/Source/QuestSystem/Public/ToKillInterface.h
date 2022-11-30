// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ToKillInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UToKillInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnKilled, AActor*, AActor*);

class QUESTSYSTEM_API IToKillInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Kill(AActor* KilledActor);

	virtual void NotifyKilled(AActor* KilledActor, AActor* Killer)
	{
		OnKilled.Broadcast(KilledActor, Killer);
	}
	
	FOnKilled OnKilled;
};
