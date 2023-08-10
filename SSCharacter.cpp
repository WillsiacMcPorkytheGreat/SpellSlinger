// Fill out your copyright notice in the Description page of Project Settings.


#include "SSCharacter.h"
#include "Gun.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterProjGameModeBase.h"

// Sets default values
ASSCharacter::ASSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASSCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	//hide original parented rifle

	Gun->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));

	Gun->SetOwner(this); //for parent

}

bool ASSCharacter::IsDead() const
{
	return Health <= 0;
}

float ASSCharacter::GetHealthPercent() const 
{
	return Health / MaxHealth;
}

// Called every frame
void ASSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, 
	&ASSCharacter::MoveForward);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this,
	&APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this,
	&ASSCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookRight"), this,
	&APawn::AddControllerYawInput);

	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this,
	&ASSCharacter::LookUpRate);

	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this,
	&ASSCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"),
	EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Shoot"),
	EInputEvent::IE_Pressed, this, &ASSCharacter::Shoot);
}

float ASSCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);

	if (IsDead())
	{
		ASimpleShooterProjGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterProjGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		
	
	}

	return DamageToApply;
}


void ASSCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ASSCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void ASSCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->
	GetDeltaSeconds()); //speed * deltaseconds for fps
}

void ASSCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->
	GetDeltaSeconds()); //speed * deltaseconds for fps
}

void ASSCharacter::Shoot() 
{
	Gun->PullTrigger();
}


