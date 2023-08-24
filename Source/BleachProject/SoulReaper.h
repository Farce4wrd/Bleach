// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoulReaper.generated.h"

UCLASS()
class BLEACHPROJECT_API ASoulReaper : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoulReaper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Activate Flash step
	UFUNCTION(BlueprintCallable, Category="Mobility")
	bool BeginFlashStep();

	UFUNCTION(BlueprintCallable)
	float GetFlashStep();

	UFUNCTION(BlueprintCallable)
	void SetFlashStep();

	UFUNCTION(BlueprintCallable)
	void ReplenishFlashStep();

	UFUNCTION(BlueprintCallable)
	int GetSpiritual();

	UFUNCTION(BlueprintCallable)
	void SetSpiritual(int& cost);

	UFUNCTION(BlueprintPure)
	bool GetBlockState();

	UFUNCTION(BlueprintCallable)
	void StartBlocking();

	UFUNCTION(BlueprintCallable)
	void StopBlocking();

private:

	UPROPERTY(EditAnywhere)
	int SpiritualEnergy;

	UPROPERTY(EditAnywhere)
	int Health;

	UPROPERTY(EditAnywhere)
	float FlashStep;

	UPROPERTY(EditAnywhere)
	float MaxFlashStep;

	UPROPERTY(EditAnywhere)
	bool isBlocking;

	UPROPERTY(EditAnywhere)
	float FSRegen;

	UPROPERTY(EditAnywhere)
	bool isGrabbable;

	UPROPERTY(EditAnywhere)
	bool isCountered;

};
