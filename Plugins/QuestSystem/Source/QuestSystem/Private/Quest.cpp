// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"
#include "Objective.h"


// Sets default values
AQuest::AQuest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AQuest::BeginPlay()
{
	Super::BeginPlay();

	AActor* ParentActor = GetAttachParentActor();
	if (ParentActor)
	{
		TakeQuest(ParentActor);
	}
}

// Called every frame
void AQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuest::UpdateLocation()
{
	AActor* ParentActor = GetAttachParentActor();
	if (ParentActor)
	{
		SetActorLocation(ParentActor->GetActorLocation());
	}
	
}

void AQuest::TakeQuest(AActor* Character)
{
	if (bIsTaken)
	{
		return;
	}

	for (int32 i = 0; i < Objectives.Num(); i++)
	{
		auto* Objective = Objectives[i];
		if (Objective)
		{
			Objective->ActivateObjective(Character);
			Objective->bCanCompleted = i == 0 || !bKeepObjectivesOrder;
			Objective->OnObjectiveCompleted.AddUObject(this, &ThisClass::OnObjectiveCompleted);
		}
	}
	bIsTaken = true;
}

void AQuest::OnObjectiveCompleted(UObjective* Objective)
{
	if (bKeepObjectivesOrder)
	{
		int32 Index;
		if (Objectives.Find(Objective, Index) && Objectives.IsValidIndex(Index+1))
		{
			Objectives[Index + 1]->bCanCompleted = true;
		}
	}
}

#if WITH_EDITOR

void AQuest::AddInteractObjective()
{
	Objectives.Add(NewObject<UInteractionObjective>(this));
}

void AQuest::AddLocationObjective()
{
	Objectives.Add(NewObject<ULocationObjective>(this));
}

void AQuest::AddKillObjective()
{
	Objectives.Add(NewObject<UKillObjective>(this));
}

void AQuest::AddCollectObjective()
{
	Objectives.Add(NewObject<UCollectObjective>(this));
}

#endif

