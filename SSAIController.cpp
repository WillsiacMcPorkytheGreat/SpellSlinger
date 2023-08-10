// Fill out your copyright notice in the Description page of Project Settings.


#include "SSAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SSCharacter.h"

void ASSAIController::BeginPlay() 
{
	Super::BeginPlay(); //super keyword for speaking to BPs
	
	if (AIBehaviour != nullptr)
	{
		RunBehaviorTree(AIBehaviour);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());

	}
}


void ASSAIController::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
	
}

bool ASSAIController::IsDead() const 
{
	ASSCharacter* ControlledCharacter = Cast<ASSCharacter>(GetPawn());
	if (ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead();
	}
	return true;
}

