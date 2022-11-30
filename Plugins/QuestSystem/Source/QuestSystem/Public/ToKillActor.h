// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToKillInterface.h"
#include "GameFramework/Actor.h"
#include "ToKillActor.generated.h"

UCLASS()
class QUESTSYSTEM_API AToKillActor : public AActor , public IToKillInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void OnKilled(AActor* Killer)
	{
		NotifyKilled(this, Killer);
	}
};
