// Copyright Spxcebxr Games


#include "Character/Player/TopDownPlayer.h"
#include "Character/Player/TopDownPlayerController.h"
#include "Character/PlayerCharacter/TopDownCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "Interaction/TopDownTargetInterface.h"
#include "Net/UnrealNetwork.h"

FPathingVariables::FPathingVariables()
{
	TargetLocation = FVector(0.f, 0.f, 0.f);
	EndPoint = FVector(0.f, 0.f, 0.f);
}

ATopDownPlayer::ATopDownPlayer()
{
 	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;	

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->CameraLagSpeed = 10.0;
	CameraSpringArm->TargetArmLength = 100.f;
	CameraSpringArm->bInheritPitch = false;
	CameraSpringArm->bInheritRoll = false;
	CameraSpringArm->bInheritYaw = false;
	CameraSpringArm->SetRelativeRotation(FRotator(295.f, 0.f, 0.f));	
	CameraSpringArm->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	CameraComponent->PostProcessSettings.bOverride_MotionBlurAmount = true;
	CameraComponent->PostProcessSettings.MotionBlurAmount = 0;
	CameraComponent->PostProcessSettings.MotionBlurMax = 0;
	CameraComponent->bUsePawnControlRotation = false;
}

void ATopDownPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ATopDownPlayer, TopDownCharacter, COND_None);
	DOREPLIFETIME_CONDITION(ATopDownPlayer, PathingVariables, COND_None);
}

void ATopDownPlayer::BeginPlay()
{
	Super::BeginPlay();	

	/* Only Spawn the Player's Character on the Server as technically it is AI and therefore not directly controlled by the player */
	if(HasAuthority())
	{
		if(!TopDownCharacterToSpawn)
		{		
			return;
		}

		FActorSpawnParameters CharacterSpawnParams;
		CharacterSpawnParams.Owner = this;
		CharacterSpawnParams.Instigator = this;

		TopDownCharacter = GetWorld()->SpawnActor<ATopDownCharacter>(TopDownCharacterToSpawn, RootComponent->GetComponentLocation(), RootComponent->GetComponentRotation(), CharacterSpawnParams);
		TopDownCharacter->CharacterOwner = this;
	}

	if(IsLocallyControlled())
	{
		/* Reset Camera Rotation */
		/* TODO: Rotate the Camera to face the same way as character. */
		RootComponent->SetWorldRotation(FRotator(0.f, 0.f, 0.f));

		/* Spawn The Mouse Position Decal */
		MousePositionDecal = GetWorld()->SpawnActor<ADecalActor>(GetActorLocation(), FRotator());
		MousePositionDecal->GetDecal()->DecalSize = FVector(32.0f, 64.0f, 64.0f);
		MousePositionDecal->SetDecalMaterial(AllowedPosition);
	}
	
}

void ATopDownPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{		
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &ATopDownPlayer::Click);
	}	
}

void ATopDownPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Check if the PlayerController has been properly set and set it if not. */
	if(!PlayerController && IsLocallyControlled())
	{
		PlayerController = Cast<ATopDownPlayerController>(GetController());

		/* Return if still not set as nothing else should be done. */
		if(!PlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("TOP DOWN PLAYER: Player Controller Not Set."));
			return;
		}
		
		/* Set the Mapping Context for Player Input. */
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PointClickMappingContext, 0);
		}
	}	
		
	CheckCurrentCursorPosition(DeltaTime);
}


void ATopDownPlayer::CheckCurrentCursorPosition(float DeltaTime)
{
	if(!PlayerController || !GetWorld() || !AllowedPosition || !NotAllowedPosition)
	{
		return;
	}
	
	/* Gets the position under the players mouse */
	FHitResult Hit;
	if(!PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, Hit) || Hit.Location.ContainsNaN())
	{
		return;
	}

	if(!MousePositionDecal)
	{
		return;
	}

	/* Checks to see if ATopDownCharacterBase was hit if so don't do path finding as can't move here. */
	ITopDownTargetInterface* CurrentActor = Cast<ITopDownTargetInterface>(Hit.GetActor());
	if(CurrentActor)
	{
		MousePositionDecal->GetDecal()->SetVisibility(false);
		bCanMoveToPosition = false;
		
		/* Nothing needs to be done here as we already have this target highlighted */
		if(TopDownTarget == CurrentActor)
		{
			return;
		}

		/* Unhighlights the Old Target if there was one. */
		if(TopDownTarget)
		{			
			TopDownTarget->UnHighlightActor();
		}

		/* Selects the new target and highlights it */
		TopDownTarget = CurrentActor;

		ATopDownCharacterBase* Character = Cast<ATopDownCharacterBase>(TopDownTarget);
		if(!Character)
		{
			TopDownTarget->HighlightActor(EIT_Item);
			return;
		}

		ATopDownCharacter* PlayerCharacter = Cast<ATopDownCharacter>(TopDownTarget);
		if(!PlayerCharacter)
		{
			TopDownTarget->HighlightActor(EIT_Enemy);
			return;
		}
		
		
		if(PlayerCharacter == TopDownCharacter)
		{			
			TopDownTarget->HighlightActor(EIT_Player);
		}
		else
		{
			TopDownTarget->HighlightActor(EIT_Enemy);
		}
		
		return;
	}
	
	MousePositionDecal->GetDecal()->SetVisibility(true);
		
	/* Unhighlights the Old Target if there was one. */
	if(TopDownTarget)
	{		
		TopDownTarget->UnHighlightActor();
		TopDownTarget = nullptr;
	}	
	
	/* Moves Decal to be used for player feedback */
	if(!MousePositionDecal->IsHidden())
	{		
		MousePositionDecal->SetActorLocation(PathingVariables.TargetLocation, false);
	}
	
	/* As the Character is not controlled directly by the player get the server to ask the Character to run path finding logic */
	Server_CheckPlayerCharacterPath(Hit.Location);

	/* Use the path variables the character found to determine if the path is allowed. */
	/* At the minute it only checks if the end point is not the same, this means there was an obstruction. */
	/* When checked if the path is allowed or not spawn a coloured decal under the mouse for player feedback */
	/* TODO: Set Up Player movement amount and check if the length of the path is allowed. */	
	if(PathingVariables.EndPoint.X != PathingVariables.TargetLocation.X || PathingVariables.EndPoint.Y != PathingVariables.TargetLocation.Y)
	{
		MousePositionDecal->SetDecalMaterial(NotAllowedPosition);		
		bCanMoveToPosition = false;
		return;
	}	
		
	MousePositionDecal->SetDecalMaterial(AllowedPosition);
	bCanMoveToPosition = true;
}

void ATopDownPlayer::Server_CheckPlayerCharacterPath_Implementation(FVector TargetLocation)
{
	if(!TopDownCharacter)
	{
		return;
	}
	
	TopDownCharacter->CheckPath(TargetLocation);
}

void ATopDownPlayer::SetPathingVariables(FVector TargetLocation, FNavPathSharedPtr PathSharedPtr)
{
	PathingVariables.TargetLocation = TargetLocation;
	PathingVariables.EndPoint = PathSharedPtr.Get()->GetEndLocation();
}

/* Movement */
void ATopDownPlayer::Click(const FInputActionValue& Value)
{
	if(!bCanMoveToPosition)
	{
		return;
	}

	Server_MovePlayerCharacter(PathingVariables.TargetLocation);	
}

void ATopDownPlayer::Server_MovePlayerCharacter_Implementation(FVector TargetLocation)
{
	if(!TopDownCharacter)
	{
		return;
	}
	
	TopDownCharacter->MoveTo(TargetLocation);
}

