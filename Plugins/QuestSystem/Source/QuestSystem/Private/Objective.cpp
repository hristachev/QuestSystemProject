// Fill out your copyright notice in the Description page of Project Settings.


#include "Objective.h"

#include "Collectable.h"
#include "InteractableObject.h"
#include "LocationMarker.h"
#include "ToKillInterface.h"

UInteractionObjective::UInteractionObjective()
{
	Type = EObjectiveType::Ot_Interact;
}

void UInteractionObjective::ActivateObjective(AActor* Instigator)
{
	if (auto* InteractTarget = Cast<IInteractableObject>(Target))
	{
		InteractTarget->OnInteractionFinished.AddLambda([this, Instigator](AActor* Object, AActor* InteractInstigator)
		{
			if (bCanCompleted && Instigator == InteractInstigator)
			{
				bIsCompleted = true;
				OnObjectiveCompleted.Broadcast(this);
			}
		});
	}
}

ULocationObjective::ULocationObjective()
{
	Type = EObjectiveType::Ot_Location;
}

void ULocationObjective::ActivateObjective(AActor* Instigator)
{
	if (auto* InteractTarget = Cast<ILocationMarker>(Marker))
	{
		InteractTarget->OnLocationReached.AddLambda([this, Instigator](AActor* Object, AActor* InteractInstigator)
		{
			if (bCanCompleted && Instigator == InteractInstigator)
			{
				bIsCompleted = true;
				OnObjectiveCompleted.Broadcast(this);
			}
		});
	}
}

UCollectObjective::UCollectObjective()
{
	Type = EObjectiveType::Ot_Collect;
}

void UCollectObjective::ActivateObjective(AActor* Instigator)
{
	if (auto* InteractTarget = Cast<ICollectable>(CollectableActor))
	{
		InteractTarget->OnCollect.AddLambda([this, Instigator](AActor* Object, AActor* InteractInstigator)
		{
			if (bCanCompleted && Instigator == InteractInstigator)
			{
				bIsCompleted = true;
				OnObjectiveCompleted.Broadcast(this);
			}
		});
	}
}

UKillObjective::UKillObjective()
{
	Type = EObjectiveType::Ot_Kill;
}

void UKillObjective::ActivateObjective(AActor* Instigator)
{
	if (auto* InteractTarget = Cast<IToKillInterface>(KilledActor))
	{
		InteractTarget->OnKilled.AddLambda([this, Instigator](AActor* Object, AActor* InteractInstigator)
		{
			if (bCanCompleted && Instigator == InteractInstigator)
			{
				bIsCompleted = true;
				OnObjectiveCompleted.Broadcast(this);
			}
		});
	}
}
