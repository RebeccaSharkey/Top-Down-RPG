// Copyright Spxcebxr Games


#include "Character/TopDownCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/TopDownCharacterController.h"

ATopDownCharacter::ATopDownCharacter()
{	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bReplicates = true;
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void ATopDownCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!CharacterController)
	{
		CharacterController = Cast<ATopDownCharacterController>(GetController());		
	}
}

void ATopDownCharacter::MoveTo(FVector Location)
{	
	if(!CharacterController)
	{
		return;
	}
	
	CharacterController->MoveToLocation(Location);
}

