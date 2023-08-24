// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulReaper.h"

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

void ASoulReaper::StartBlocking(){
	isBlocking = true;
}

void ASoulReaper::StopBlocking(){
	isBlocking = false;
}