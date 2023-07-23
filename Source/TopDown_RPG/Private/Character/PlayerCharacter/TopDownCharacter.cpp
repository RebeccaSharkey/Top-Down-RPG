// Copyright Spxcebxr Games


#include "Character/PlayerCharacter/TopDownCharacter.h"
#include "Character/PlayerCharacter/TopDownCharacterController.h"
#include "Player/TopDownPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/TopDownAbilitySystemComponent.h"
#include "AbilitySystem/TopDownAttributeSet.h"
#include "Player/TopDownPlayerState.h"

ATopDownCharacter::ATopDownCharacter()
{	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	bReplicates = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;	

	AbilitySystemComponent = CreateDefaultSubobject<UTopDownAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UTopDownAttributeSet>("AttributeSet");
}

void ATopDownCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ATopDownCharacter, TopDownOwnerPlayer, COND_None);
	DOREPLIFETIME(ATopDownCharacter, TopDownOwnerPlayerState);
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void ATopDownCharacter::SetUpTopDownPlayerCharacter()
{
	if(!GetController())
	{
		UE_LOG(LogTemp, Warning, TEXT("TopDownCharacter - SetUpTopDownPlayerCharacter: No Controller Set."));
		return;
	}
	
	TopDownCharacterController = Cast<ATopDownCharacterController>(GetController());
	if(!TopDownCharacterController)
	{		
		UE_LOG(LogTemp, Warning, TEXT("TopDownCharacter - SetUpTopDownPlayerCharacter: Failed to set TopDownCharacterController."));
		return;
	}

	if(!TopDownOwnerPlayer)
	{		
		if(!GetOwner())
		{
			UE_LOG(LogTemp, Warning, TEXT("TopDownCharacter - SetUpTopDownPlayerCharacter: No Owner."));
			return;
		}

		TopDownOwnerPlayer = Cast<ATopDownPlayer>(GetOwner());
		if(!TopDownOwnerPlayer)
		{		
			UE_LOG(LogTemp, Warning, TEXT("TopDownCharacter - SetUpTopDownPlayerCharacter: Failed to set TopDownOwnerPlayer."));
			return;
		}
	}

	if(GetOwner() != TopDownOwnerPlayer)
	{
		SetOwner(TopDownOwnerPlayer);
	}

	if(!TopDownOwnerPlayer->GetPlayerState())
	{
		UE_LOG(LogTemp, Warning, TEXT("TopDownCharacter - SetUpTopDownPlayerCharacter: TopDownOwnerPlayer has no PlayerState"));
		return;
	}

	TopDownOwnerPlayerState = Cast<ATopDownPlayerState>(TopDownOwnerPlayer->GetPlayerState());
	if(!TopDownOwnerPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("TopDownCharacter - SetUpTopDownPlayerCharacter: Failed to set TopDownOwnerPlayerState."));
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(TopDownOwnerPlayerState, this);
}

void ATopDownCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!TopDownCharacterController)
	{
		TopDownCharacterController = Cast<ATopDownCharacterController>(GetController());		
	}

	if(!TopDownOwnerPlayer && GetOwner())
	{
		TopDownOwnerPlayer = Cast<ATopDownPlayer>(GetOwner());
	}
}

void ATopDownCharacter::CheckPath(FVector Location)
{
	if(!TopDownCharacterController && !TopDownOwnerPlayer)
	{
		return;
	}
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Location);
	FPathFindingQuery Query;
	Query.SetNavAgentProperties(TopDownCharacterController->GetNavAgentPropertiesRef());
	Query.SetAllowPartialPaths(false);

	TopDownCharacterController->BuildPathfindingQuery(MoveRequest, Query);

	TopDownCharacterController->FindPathForMoveRequest(MoveRequest, Query, PathSharedPtr);
	
	if(!PathSharedPtr.Get())
	{
		return;
	}

	Server_SetPlayerPathingVariables(Location);
}

void ATopDownCharacter::Server_SetPlayerPathingVariables_Implementation(FVector Location)
{
	if(TopDownOwnerPlayer)
	{		
		TopDownOwnerPlayer->SetPathingVariables(Location, PathSharedPtr);
	}
}

void ATopDownCharacter::MoveTo(FVector Location)
{	
	if(!TopDownCharacterController)
	{
		return;
	}

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Location);
	FPathFindingQuery Query;
	Query.SetNavAgentProperties(TopDownCharacterController->GetNavAgentPropertiesRef());
	Query.SetAllowPartialPaths(false);
	FNavPathSharedPtr MoveToPathSharedPtr;

	TopDownCharacterController->BuildPathfindingQuery(MoveRequest, Query);

	TopDownCharacterController->FindPathForMoveRequest(MoveRequest, Query, MoveToPathSharedPtr);
	
	if(!MoveToPathSharedPtr)
	{
		return;
	}

	if(MoveToPathSharedPtr.Get()->GetEndLocation().X != Location.X || MoveToPathSharedPtr.Get()->GetEndLocation().Y != Location.Y)
	{
		return;
	}
	
	TopDownCharacterController->MoveToLocation(Location, 1.f, false);
}

