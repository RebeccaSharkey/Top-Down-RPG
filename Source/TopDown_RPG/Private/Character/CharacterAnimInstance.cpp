// Copyright Spxcebxr Games


#include "Character/CharacterAnimInstance.h"
#include "Character/TopDownCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	TopDownCharacter = Cast<ATopDownCharacterBase>(TryGetPawnOwner());
	if(!TopDownCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANIM INSTANCE: ---- TopDownCharacter Failed To Set."));
		return;
	}

	MovementComponent = Cast<UMovementComponent>(TopDownCharacter->GetCharacterMovement());
	if(!MovementComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANIM INSTANCE: ---- MovementComponent Failed To Set."));
		return;
	}
	
}

void UCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!TopDownCharacter)
	{
		TopDownCharacter = Cast<ATopDownCharacterBase>(TryGetPawnOwner());
		if(!TopDownCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("ANIM INSTANCE: ---- TopDownCharacter Failed To Set."));
			return;
		}
	}
	if(!MovementComponent)
	{
		MovementComponent = Cast<UMovementComponent>(TopDownCharacter->GetCharacterMovement());
		if(!MovementComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("ANIM INSTANCE: ---- MovementComponent Failed To Set."));
			return;
		}
	}

	SetVariables(DeltaSeconds);
	
}

void UCharacterAnimInstance::SetVariables(const float DeltaSeconds)
{
	FVector Velocity = MovementComponent->Velocity;
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
}
