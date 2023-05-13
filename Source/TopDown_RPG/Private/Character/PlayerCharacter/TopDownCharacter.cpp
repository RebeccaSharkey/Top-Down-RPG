// Copyright Spxcebxr Games


#include "Character/PlayerCharacter/TopDownCharacter.h"
#include "Character/PlayerCharacter/TopDownCharacterController.h"
#include "Character/Player/TopDownPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

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

void ATopDownCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ATopDownCharacter, CharacterOwner, COND_None);
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

	if(!CharacterOwner && GetOwner())
	{
		CharacterOwner = Cast<ATopDownPlayer>(GetOwner());
	}
}

void ATopDownCharacter::CheckPath(FVector Location)
{
	if(!CharacterController && !CharacterOwner)
	{
		return;
	}
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Location);
	FPathFindingQuery Query;
	Query.SetNavAgentProperties(CharacterController->GetNavAgentPropertiesRef());
	Query.SetAllowPartialPaths(false);

	CharacterController->BuildPathfindingQuery(MoveRequest, Query);

	CharacterController->FindPathForMoveRequest(MoveRequest, Query, PathSharedPtr);
	
	if(!PathSharedPtr.Get())
	{
		return;
	}

	/*UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavSystem)
	{
		ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
		if (NavData)
		{
			PathSharedPtr.Get()->DebugDraw(NavData, FColor::Red, GetWorld()->GetCanvasForRenderingToTarget(), false);
		}
	}*/

	Server_SetPlayerPathingVariables(Location);
}

void ATopDownCharacter::Server_SetPlayerPathingVariables_Implementation(FVector Location)
{
	CharacterOwner->SetPathingVariables(Location, PathSharedPtr);
}

void ATopDownCharacter::MoveTo(FVector Location)
{	
	if(!CharacterController)
	{
		return;
	}

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Location);
	FPathFindingQuery Query;
	Query.SetNavAgentProperties(CharacterController->GetNavAgentPropertiesRef());
	Query.SetAllowPartialPaths(false);
	FNavPathSharedPtr MoveToPathSharedPtr;

	CharacterController->BuildPathfindingQuery(MoveRequest, Query);

	CharacterController->FindPathForMoveRequest(MoveRequest, Query, MoveToPathSharedPtr);
	
	if(!MoveToPathSharedPtr)
	{
		return;
	}

	if(MoveToPathSharedPtr.Get()->GetEndLocation().X != Location.X || MoveToPathSharedPtr.Get()->GetEndLocation().Y != Location.Y)
	{
		return;
	}
	
	CharacterController->MoveToLocation(Location);
		
}

