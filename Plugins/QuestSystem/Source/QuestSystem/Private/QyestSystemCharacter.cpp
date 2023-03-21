// Fill out your copyright notice in the Description page of Project Settings.


#include "QyestSystemCharacter.h"
#include "Components/ArrowComponent.h"
#include "Quest.h"
#include "GameFramework/Actor.h"
#include "QuestDialog.h"
#include "QuestListComponent.h"


// Sets default values
AQyestSystemCharacter::AQyestSystemCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MarkerPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("QuestMarkerPoint"));
	MarkerPoint->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AQyestSystemCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!GetAcceptableQuest())
	{
		SpawnQuestMarker();
	}
}

// Called every frame
void AQyestSystemCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AQyestSystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AQyestSystemCharacter::Interact_Implementation(AActor* InteractInstigator)
{
	if (InteractInstigator == nullptr)
	{
		return;
	}

	UQuestListComponent* QuestList = Cast<UQuestListComponent>(InteractInstigator->GetComponentByClass(UQuestListComponent::StaticClass()));
	if (QuestList == nullptr)
	{
		return;
	}

	AQuest* Quest = GetAcceptableQuest();
	
	if (Quest && QuestDialogClass)
	{
		UQuestDialog* QuestDialog = CreateWidget<UQuestDialog>(GetWorld(), QuestDialogClass);
		QuestDialog->Init(Quest);

		QuestDialog->OnQuestAccepted.BindUObject(QuestList, &UQuestListComponent::AddQuest, Quest);
		QuestDialog->OnDialogExit.BindLambda([this, InteractInstigator, Quest]
		{
			if (Quest->bIsTaken)
			{
				Quest->OnQuestStatusUpdate.AddLambda([this](AQuest* Quest)
				{
					OnQuestUpdated.Broadcast(GetAcceptableQuest() != nullptr);
				});
			}
			NotifyInteractionFinished(this, InteractInstigator);
			OnQuestUpdated.Broadcast(GetAcceptableQuest() != nullptr);
		});
		QuestDialog->AddToViewport();
		return;
	}
	NotifyInteractionFinished(this, InteractInstigator);
}

AQuest* AQyestSystemCharacter::GetAcceptableQuest() const
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	for (auto& Actor : AttachedActors)
	{
		AQuest* Quest= Cast<AQuest>(Actor);
		if (Quest == nullptr) continue;

		if (Quest->bIsTaken || Quest->GetPreviousQuest())
		{
			continue;
		}
		return Quest;
	}

	return nullptr;
}

void AQyestSystemCharacter::SpawnQuestMarker()
{
	if (QuestMarker)
	{
		return;
	}
	
	QuestMarker = GetWorld()->SpawnActor(QuestMarkerClass);
	QuestMarker->AttachToComponent(MarkerPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AQyestSystemCharacter::DestroyQuestMarker(bool bHasQuest)
{
	
	if (QuestMarker)
	{
		QuestMarker->SetActorHiddenInGame(bHasQuest);
	}
	
}


