// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "AI/Navigation/NavigationTypes.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "TopDownPlayer.generated.h"

class ATopDownCharacter;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FPathingVariables
{
	GENERATED_BODY()

	
public:
	FPathingVariables();

	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere)
	FVector EndPoint;
};

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
	
	TObjectPtr<ATopDownPlayerController> PlayerController;

	UPROPERTY(EditAnywhere, Category = "Player")
	TSubclassOf<ATopDownCharacter> TopDownCharacterToSpawn;
	
	UPROPERTY(EditAnywhere, Replicated, Category = "Player")
	TObjectPtr<ATopDownCharacter> TopDownCharacter;
	
/*---------------------------------------------------------------------------------------------------------------------*
 *		Point and Click
 *---------------------------------------------------------------------------------------------------------------------*/
	/* Point and Click */
	/* TODO: Move the Input to the Player Controller. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputMappingContext> PointClickMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> ClickAction;

	/* Path Finding and Player Feedback */
private:
	void FindCurrentPath(float DeltaTime);
	
	UFUNCTION(Server, Reliable)
	void Server_CheckPlayerCharacterPath(FVector TargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Pathing", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMaterial> AllowedPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Pathing", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMaterial> NotAllowedPosition;	
	
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Pathing")
	FPathingVariables PathingVariables;
	
public:
	void SetPathingVariables(FVector TargetLocation, FNavPathSharedPtr PathSharedPtr);

	/* Movement */
private:
	bool bCanMoveToPosition;
	
	void Click(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void Server_MovePlayerCharacter(FVector TargetLocation);
};
