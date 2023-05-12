// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "TopDownPlayer.generated.h"

class ATopDownCharacter;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class TOPDOWN_RPG_API ATopDownPlayer : public APawn
{
	GENERATED_BODY()

public:
	friend class ATopDownPlayerController;
	
	ATopDownPlayer();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

/*---------------------------------------------------------------------------------------------------------------------*
 *		Camera Set Up
 *---------------------------------------------------------------------------------------------------------------------*/
	UPROPERTY(EditAnywhere, Category = "PlayerCamera")
	TObjectPtr<USceneComponent> SceneComponent;	
	UPROPERTY(EditAnywhere, Category = "PlayerCamera")
	TObjectPtr<USpringArmComponent> CameraSpringArm;	
	UPROPERTY(EditAnywhere, Category = "PlayerCamera")
	TObjectPtr<UCameraComponent> CameraComponent;

/*---------------------------------------------------------------------------------------------------------------------*
 *		Player Set Up
 *---------------------------------------------------------------------------------------------------------------------*/
	
	ATopDownPlayerController* PlayerController;

	UPROPERTY(EditAnywhere, Category = "Player")
	TSubclassOf<ATopDownCharacter> TopDownCharacterToSpawn;
	
	UPROPERTY(EditAnywhere, Replicated, Category = "Player")
	TObjectPtr<ATopDownCharacter> TopDownCharacter;
	
/*---------------------------------------------------------------------------------------------------------------------*
 *		Point and Click
 *---------------------------------------------------------------------------------------------------------------------*/

	/* Point and Click */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* PointClickMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* ClickAction;

private:	
	void Click(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void Server_MovePlayerCharacter(FVector TargetLocation);	
};
