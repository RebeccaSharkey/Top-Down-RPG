// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TopDownCharacterAnimInstance.generated.h"

class ATopDownCharacterBase;
class UMovementComponent;
/**
 * 
 */
UCLASS()
class TOPDOWN_RPG_API UTopDownCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess="true"))
	ATopDownCharacterBase* TopDownCharacter;
	UPROPERTY(BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess="true"))
	UMovementComponent* MovementComponent;

protected:
	virtual void SetVariables(const float DeltaSeconds);

private:	
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving;
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;
};
