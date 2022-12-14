// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InteractionComponent.generated.h"

class UUserWidget;
class UPrimitiveComponent;

UCLASS(ClassGroup=(Collision), meta=(BlueprintSpawnableComponent, DisplayName = "InteractionCollision"))
class QUESTSYSTEM_API UInteractionComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InteractMessageWidget;

	UPROPERTY()
	UUserWidget* InteractMessage;

	UPROPERTY()
	AActor* ActorToInteract;

	UPROPERTY()
	AActor* ActorToCollect;

	UPROPERTY()
	AActor* ActorToKill;

	UFUNCTION()
	void OnComponentBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable)
	void Kill();

	UFUNCTION(BlueprintCallable)
	void Collect();
};
