// Copyright Spxcebxr Games


#include "Character/Player/TopDownPlayerController.h"
#include "Character/Player/TopDownPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"


ATopDownPlayerController::ATopDownPlayerController()
{
	bReplicates = true;
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	CameraSpeed = 1000.0f;
	CameraZoomSpeed = 20.0f;
	CameraZoomPerUnit = 300.0f;

	MinCameraDistance = 500.0f;
	MaxCameraDistance = 2500.0f;

	CameraPanThreshold = 20.0f;

	TargetCameraZoomDistance = 1500.0f;
}

void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(CameraMappingContext);
	
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(CameraMappingContext, 0);
	}
	
	bShowMouseCursor = true; 
	bEnableClickEvents = true; 
	bEnableMouseOverEvents = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	
}

void ATopDownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::MoveCamera);
		EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::MoveCamera);
		EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Started, this, &ATopDownPlayerController::RotateCameraPressed);
		EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::RotateCameraReleased);
		EnhancedInputComponent->BindAction(ZoomCameraAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::ZoomCamera);		
	}	
}

void ATopDownPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);	

	if(!TopDownPlayer && GetPawn())
	{
		TopDownPlayer = Cast<ATopDownPlayer>(GetPawn());
		if(TopDownPlayer)
		{
			TopDownPlayer->CameraSpringArm->TargetArmLength = TargetCameraZoomDistance;
		}
	}	

	if(TopDownPlayer)
	{
		CameraMovement(DeltaSeconds);
	}
}

ATopDownPlayer* ATopDownPlayerController::GetTopDownPlayer() const
{
	return TopDownPlayer;
}

void ATopDownPlayerController::MoveCamera(const FInputActionValue& Value)
{
	if(!Value.Get<FVector2D>().IsNearlyZero() || !Value.Get<FVector2D>().IsZero())
	{		
		CameraAxisValue = Value.Get<FVector2D>();
	}
}

void ATopDownPlayerController::RotateCameraPressed(const FInputActionValue& Value)
{
	bShouldRotateCamera = true;
	
	GetMousePosition(RotateCameraMouseStart.X, RotateCameraMouseStart.Y);
}

void ATopDownPlayerController::RotateCameraReleased(const FInputActionValue& Value)
{
	bShouldRotateCamera = false;
}

void ATopDownPlayerController::ZoomCamera(const FInputActionValue& Value)
{
	if (FMath::IsNearlyZero(Value.Get<float>()))
	{
		return;
	}
	
	float ZoomAmount;

	if (Value.Get<float>() > 0) {
		ZoomAmount = CameraZoomPerUnit;
	}
	else {
		ZoomAmount = -CameraZoomPerUnit;
	}
	
	TargetCameraZoomDistance = FMath::Clamp(TopDownPlayer->CameraSpringArm->TargetArmLength + ZoomAmount, MinCameraDistance, MaxCameraDistance);
}

void ATopDownPlayerController::CameraMovement(float DeltaTime)
{	
	
	int ViewportSizeX;
	int ViewportSizeY;

	GetViewportSize(ViewportSizeX,ViewportSizeY);
	
	const float ScrollBorderRight = ViewportSizeX - CameraPanThreshold;
	const float ScrollBorderTop = ViewportSizeY - CameraPanThreshold;
	
	float MouseX;
	float MouseY;
	
	if (GetMousePosition(MouseX, MouseY) && !bShouldRotateCamera)
	{
		if (MouseX <= CameraPanThreshold)
		{
			CameraAxisValue.X -= 1 - (MouseX / CameraPanThreshold);
		}
		else if (MouseX >= ScrollBorderRight)
		{
			CameraAxisValue.X += (MouseX - ScrollBorderRight) / CameraPanThreshold;
		}
		if (MouseY <= CameraPanThreshold)
		{
			CameraAxisValue.Y += 1 - (MouseY / CameraPanThreshold);
		}
		else if (MouseY >= ScrollBorderTop)
		{
			CameraAxisValue.Y -= (MouseY - ScrollBorderTop) / CameraPanThreshold;
		}
	}
	
	CameraAxisValue.X = FMath::Clamp(CameraAxisValue.X, -1.0, 1.0);
	CameraAxisValue.Y = FMath::Clamp(CameraAxisValue.Y, -1.0, 1.0);
	
	FVector Location = TopDownPlayer->GetActorLocation();

	Location += TopDownPlayer->CameraSpringArm->GetRightVector() * CameraSpeed * CameraAxisValue.X * DeltaTime;
	
	Location += FRotationMatrix(FRotator(0, TopDownPlayer->CameraSpringArm->GetRelativeRotation().Yaw, 0)).GetScaledAxis(EAxis::X) * CameraSpeed * CameraAxisValue.Y * DeltaTime;

	TopDownPlayer->SetActorLocation(Location);
	
	if (!FMath::IsNearlyEqual(TopDownPlayer->CameraSpringArm->TargetArmLength, TargetCameraZoomDistance, 0.5f))
	{
		TopDownPlayer->CameraSpringArm->TargetArmLength = FMath::FInterpTo(TopDownPlayer->CameraSpringArm->TargetArmLength, TargetCameraZoomDistance, DeltaTime, CameraZoomSpeed);
	}

	if (bShouldRotateCamera)
	{
		FVector2D MouseLocation;
		GetMousePosition(MouseLocation.X, MouseLocation.Y);

		const float XPercent = (MouseLocation.X - RotateCameraMouseStart.X) / ViewportSizeX;
		const float YPercent = (RotateCameraMouseStart.Y - MouseLocation.Y) / ViewportSizeY;

		const FRotator CurrentRot = TopDownPlayer->CameraSpringArm->GetRelativeRotation().GetNormalized();

		TopDownPlayer->CameraSpringArm->SetRelativeRotation(FRotator(FMath::Clamp<float>(CurrentRot.Pitch + (YPercent * 180), -88, 88), CurrentRot.Yaw + (XPercent * 180), 0));

		RotateCameraMouseStart = MouseLocation;
	}

	CameraAxisValue = FVector2D(0.f, 0.f);
}
