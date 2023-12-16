// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulReaper.h"
#include "AWeapon.h"
#include "NavigationSystem.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"

// Sets default values
ASoulReaper::ASoulReaper()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh() -> SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh() -> SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	GetMesh() -> SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh() -> SetGenerateOverlapEvents(true);
	GetCapsuleComponent() ->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

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

void ASoulReaper::DirectionalHitReact(const FVector& ImpactPoint)
{
	//Forward * ToHit = |Forward| |ToHit| * cos(theta)
	//|Forward| = 1, |ToHit| = 1, so Forward *ToHit = cos(theta)
	const FVector Forward = GetActorForwardVector();
	//Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//Take the inverse cos to get theta - Angle of hit
	double Theta = FMath::Acos(CosTheta);
	//Convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);
	//If CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if(CrossProduct.Z <0)
	{
		Theta *= -1.f;
	}
	FName Section("FromBack");
	if(Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}else if(Theta >=-135 && Theta < -45)
	{
		Section = FName("FromLeft");
	}else if(Theta >= 45 && Theta < 135)
	{
		Section = FName("FromRight");
	}
	UKismetSystemLibrary::DrawDebugArrow(this,GetActorLocation(), GetActorLocation()+ CrossProduct*100.f,5.f, FColor::Blue,5.f);
	PlayHitReactMontage(Section);

	if(GEngine)
	{
		GEngine -> AddOnScreenDebugMessage(1,5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this,GetActorLocation(), GetActorLocation()+ Forward*60.f,5.f, FColor::Red,5.f);
	UKismetSystemLibrary::DrawDebugArrow(this,GetActorLocation(), GetActorLocation()+ ToHit*60.f,5.f, FColor::Green,5.f);
}

// Called when the character is hit
void ASoulReaper::GetHit(const FVector& ImpactPoint)
{
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
	DirectionalHitReact(ImpactPoint);
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
		MyWeapon ->IgnoreActors.Empty();
	}
	
	
		
}
//Plays Attack Montage
void ASoulReaper::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AttackMontage)
	{
		AnimInstance -> Montage_Play(AttackMontage);
	}
	ActionState = EActionState::EAS_Unoccupied;
}

//Plays Hit Montage
void ASoulReaper::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh() -> GetAnimInstance();
	if(AnimInstance && HitReactMontage)
	{
		AnimInstance -> Montage_Play(HitReactMontage);
		AnimInstance -> Montage_JumpToSection(SectionName,HitReactMontage);
	}
}

