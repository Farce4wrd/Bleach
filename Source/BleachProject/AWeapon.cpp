// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"

#include "Components/BoxComponent.h"

// Sets default values
AAWeapon::AAWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon HitBox"));
	WeaponBox -> SetupAttachment(GetRootComponent());
	WeaponBox -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

// Called when the game starts or when spawned
void AAWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

