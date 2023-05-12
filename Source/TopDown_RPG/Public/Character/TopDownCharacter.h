// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/TopDownCharacterBase.h"
#include "TopDownCharacter.generated.h"

class ATopDownCharacterController;
class ATopDownPlayerController;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class TOPDOWN_RPG_API ATopDownCharacter : public ATopDownCharacterBase
{
	GENERATED_BODY()

public:
	friend class ATopDownPlayer;	
	ATopDownCharacter();

protected:	
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	
private:
	ATopDownCharacterController* CharacterController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	/* Movement*/
	void MoveTo(FVector Location);
	
};
