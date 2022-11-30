// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Objective.generated.h"

UENUM()
enum class EObjectiveType : uint8
{
	Ot_Location,
	Ot_Interact,
	Ot_Collect,
	Ot_Kill,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, class UObjective*);

UCLASS(Abstract)
class QUESTSYSTEM_API UObjective : public UObject
{
	GENERATED_BODY()

public:

	virtual void ActivateObjective(AActor* Instigator) {};

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	EObjectiveType Type;

	FOnObjectiveCompleted OnObjectiveCompleted;

	UPROPERTY(VisibleAnywhere)
	bool bIsCompleted;

	UPROPERTY(VisibleAnywhere)
	bool bCanCompleted;
	
};

UCLASS()
class QUESTSYSTEM_API UInteractionObjective : public UObjective
{
	GENERATED_BODY()

public:

	UInteractionObjective();

	virtual void ActivateObjective(AActor* Instigator) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "InteractableObject"))
	AActor* Target;
	
};

UCLASS()
class QUESTSYSTEM_API ULocationObjective : public UObjective
{
	GENERATED_BODY()

public:

	ULocationObjective();

	virtual void ActivateObjective(AActor* Instigator) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "LocationMarker"))
	AActor* Marker;
};

UCLASS()
class UCollectObjective : public UObjective
{
	GENERATED_BODY()

public:

	UCollectObjective();

	virtual void ActivateObjective(AActor* Instigator) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "Collectable"))
	AActor* CollectableActor;
	
};

UCLASS()
class UKillObjective : public UObjective
{
	GENERATED_BODY()

public:

	UKillObjective();

	virtual void ActivateObjective(AActor* Instigator) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "ToKillInterface"))
	AActor* KilledActor;
	
};
