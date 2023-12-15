// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeapon.h"
#include "Interfaces/HitInterface.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon HitBox"));
	WeaponBox -> SetupAttachment(GetRootComponent());
	WeaponBox -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox -> SetCollisionResponseToAllChannels(ECR_Overlap);
	WeaponBox -> SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart -> SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd -> SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox -> OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart -> GetComponentLocation();
	const FVector End = BoxTraceEnd -> GetComponentLocation();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f,5.f,5.f),
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		BoxHit,
		true
		);
	if(BoxHit.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if(HitInterface)
		{
			HitInterface ->GetHit(BoxHit.ImpactPoint);
		}
	}
}


