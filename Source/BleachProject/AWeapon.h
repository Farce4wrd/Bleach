// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AWeapon.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
UCLASS()
class BLEACHPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	FORCEINLINE UBoxComponent* GetWeaponBox() const{ return WeaponBox;}
	TArray<AActor*> IgnoreActors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private :
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	UBoxComponent* WeaponBox;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
