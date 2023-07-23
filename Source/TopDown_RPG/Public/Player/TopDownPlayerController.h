// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

class ATopDownPlayer;
class ATopDownCharacter;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class TOPDOWN_RPG_API ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopDownPlayerController();
	
protected:	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	TObjectPtr<ATopDownPlayer> TopDownPlayer;

public:
	ATopDownPlayer* GetTopDownPlayer() const;
	
/*---------------------------------------------------------------------------------------------------------------------*
 *		Input
 *---------------------------------------------------------------------------------------------------------------------*/
protected:
	/* Camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputMappingContext> CameraMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> MoveCameraAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> RotateCameraAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> ZoomCameraAction;

	
private:
	void MoveCamera(const FInputActionValue& Value);

	void RotateCameraPressed(const FInputActionValue& Value);
	void RotateCameraReleased(const FInputActionValue& Value);

	void ZoomCamera(const FInputActionValue& Value);

	
/*---------------------------------------------------------------------------------------------------------------------*
 *		Camera
 *---------------------------------------------------------------------------------------------------------------------*/	
protected:
	void CameraMovement(float DeltaTime);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerCamera", meta = (ClampMin = 0))
	float CameraSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerCamera")
	float CameraZoomSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerCamera")
	float CameraZoomPerUnit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerCamera", meta = (ClampMin = 0))
	float MinCameraDistance;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerCamera", meta = (ClampMin = 0))
	float MaxCameraDistance;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerCamera", meta = (ClampMin = 0))
	int32 CameraPanThreshold;

private:
	FVector2D CameraAxisValue;
	
	float CameraZoomAxisValue;
	float TargetCameraZoomDistance;

	bool bShouldRotateCamera;
	FVector2D RotateCameraMouseStart;
	
};
