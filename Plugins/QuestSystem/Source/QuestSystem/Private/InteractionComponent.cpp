// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Collectable.h"
#include "InteractableObject.h"
#include "ToKillInterface.h"
#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlapFunc);
	OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnComponentEndOverlapFunc);
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionComponent::OnComponentBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UInteractableObject>() && !OtherActor->Implements<UToKillInterface>() && !OtherActor->Implements<UCollectable>())
	{
		return;
	}

	if(OtherActor->Implements<UInteractableObject>())
		ActorToInteract = OtherActor;
	if(OtherActor->Implements<UToKillInterface>())
		ActorToKill = OtherActor;
	if (OtherActor->Implements<UCollectable>())
		ActorToCollect = OtherActor;
//
	if (InteractMessage == nullptr && InteractMessageWidget)
	{
		InteractMessage = CreateWidget(GetWorld(), InteractMessageWidget);
	}

	if (InteractMessage && !InteractMessage->IsInViewport())
	{
		InteractMessage->AddToViewport();
	}
}

void UInteractionComponent::OnComponentEndOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ActorToInteract == OtherActor || ActorToKill == OtherActor || ActorToCollect == OtherActor)
	{
		ActorToInteract = nullptr;
		ActorToKill = nullptr;
		ActorToCollect = nullptr;

		if (InteractMessage)
		{
			InteractMessage->RemoveFromViewport();
		}
	}
}

void UInteractionComponent::Interact()
{
	if (ActorToInteract)
	{
		IInteractableObject::Execute_Interact(ActorToInteract, GetOwner());
	}
}

void UInteractionComponent::Kill()
{
	if (ActorToKill)
	{
		IToKillInterface::Execute_Kill(ActorToKill, GetOwner());
	}
}

void UInteractionComponent::Collect()
{
	if (ActorToCollect)
	{
		ICollectable::Execute_Collect(ActorToCollect, GetOwner());
	}
}

