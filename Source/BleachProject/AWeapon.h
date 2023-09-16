// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AWeapon.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
UCLASS()
class BLEACHPROJECT_API AAWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private :
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	UStaticMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	UBoxComponent* WeaponBox;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
