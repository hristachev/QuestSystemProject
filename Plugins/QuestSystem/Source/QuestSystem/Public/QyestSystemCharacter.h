// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "GameFramework/Character.h"
#include "QyestSystemCharacter.generated.h"

class AQuest;
class UQuestListComponent;
class UQuestDialog;
class UArrowComponent;
class AActor;
UCLASS()
class QUESTSYSTEM_API AQyestSystemCharacter : public ACharacter, public IInteractableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AQyestSystemCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Interact_Implementation(AActor* InteractInstigator) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> QuestMarkerClass;

	UPROPERTY()
	AActor* QuestMarker;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UArrowComponent* MarkerPoint;
		
	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuestDialog> QuestDialogClass;

	AQuest* GetAcceptableQuest() const;

	UFUNCTION(BlueprintCallable)
	void SpawnQuestMarker();

	UFUNCTION(BlueprintCallable)
	void DestroyQuestMarker(bool bHasQuest);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdated, bool, bHasQuest);
	UPROPERTY(BlueprintAssignable)
	FOnQuestUpdated OnQuestUpdated;
	
};
