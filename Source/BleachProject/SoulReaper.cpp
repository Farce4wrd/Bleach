// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulReaper.h"
#include "AWeapon.h"
#include "Components/BoxComponent.h"

// Sets default values
ASoulReaper::ASoulReaper()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASoulReaper::BeginPlay()
{
	Super::BeginPlay();
	FlashStep = 3.0;
	MaxFlashStep = 3.0;
	FSRegen = 0.03;
	
	
}

// Called every frame
void ASoulReaper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ReplenishFlashStep();

}

// Called to bind functionality to input
void ASoulReaper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//Replenish Flash Step over time
void ASoulReaper::ReplenishFlashStep(){
	if(FlashStep < MaxFlashStep){
		FlashStep += FSRegen;
	UE_LOG(LogTemp, Display, TEXT("Current FS: %f"),GetFlashStep());
	}
	
}

//Called when soul reaper initiates a flash step
bool ASoulReaper::BeginFlashStep(){
	int stepsLeft = GetFlashStep();

	UWorld* World = GetWorld();

	if(stepsLeft > 0){
		//get player's original fvector location
		FVector CurrentLocation = GetActorLocation();
		FVector Forward = GetLastMovementInputVector().GetSafeNormal();
		float DeltaTime = GetWorld() -> GetDeltaSeconds();
		UE_LOG(LogTemp, Display, TEXT("DeltaTime: %f"), DeltaTime);
		FVector Velocity = (Forward*1000);
		UE_LOG(LogTemp, Display, TEXT("Forward: %s"), *Forward.ToString());
		UE_LOG(LogTemp, Display, TEXT("Original Distance: %s"), *CurrentLocation.ToString());
		UE_LOG(LogTemp, Display, TEXT("Velocity: %s"), *Velocity.ToString());
		
		LaunchCharacter(Velocity, false, false);
		if(World){
			DrawDebugLine(World, CurrentLocation, CurrentLocation+Velocity, FColor::Red, false, 4.0f,0,1.f);
		}
		
		FString myDistance = Forward.ToCompactString();
		
		UE_LOG(LogTemp, Display, TEXT("Distance: %s"), *myDistance);
		UE_LOG(LogTemp, Display, TEXT("FlashStep Activated!"));
		SetFlashStep();
		return true;
	}
	
	return false;

}

float ASoulReaper::GetFlashStep(){
	return FlashStep;
}

void ASoulReaper::SetFlashStep(){
	if(FlashStep != 0){
		FlashStep -= 1;
	}
	UE_LOG(LogTemp, Display, TEXT("FlashStep remaining: %d"), FlashStep);
}

int ASoulReaper::GetSpiritual(){
	return SpiritualEnergy;
}

void ASoulReaper::SetSpiritual(int& energy){
	if(SpiritualEnergy != 0){
		SpiritualEnergy -= energy;
	}
	
}

bool ASoulReaper::GetBlockState(){
	return isBlocking;
}

bool ASoulReaper::GetAirState()
{
	return isInAir;
}

void ASoulReaper::SetAirStateOnJump()
{
	isInAir = true;
}
void ASoulReaper::SetAirStateOnLand()
{
	isInAir = false;
}


void ASoulReaper::StartBlocking(){
	ActionState = EActionState::EAS_Blocking;
	isBlocking = true;
}

void ASoulReaper::StopBlocking(){
	ActionState =EActionState::EAS_Unoccupied;
	isBlocking = false;
}

void ASoulReaper::Attack()
{
	if(CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

bool ASoulReaper::CanAttack()
{
	if(ActionState == EActionState::EAS_Unoccupied && ActionState != EActionState::EAS_Blocking)
	{
		return true;
	}else
	{
		return false;
	}
}

void ASoulReaper::SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled)
{
	//UChildActorComponent* MyWeapon = GetComponentByClass<UChildActorComponent>();
	UChildActorComponent* HWeapon = GetComponentByClass<UChildActorComponent>();
	AWeapon* MyWeapon = Cast<AWeapon>(HWeapon -> GetChildActor());
	if(MyWeapon && MyWeapon ->GetWeaponBox())
	{
		MyWeapon -> GetWeaponBox() ->SetCollisionEnabled(CollisionEnabled);
		// FString name = MyWeapon -> GetName();
		// FString parentName = MyWeapon -> GetParentActor() ->GetName();
		// UE_LOG(LogTemp, Display, TEXT("Weapon name: %s"), *name);
		// UE_LOG(LogTemp, Display, TEXT("Parent name: %s"), *parentName);
	}
	
	
		
}

void ASoulReaper::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AttackMontage)
	{
		AnimInstance -> Montage_Play(AttackMontage);
	}
	ActionState = EActionState::EAS_Unoccupied;
}
