// Copyright Spxcebxr Games


#include "Character/TopDownPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/TopDownCharacter.h"
#include "Character/TopDownPlayerController.h"
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
	if(!PlayerController)
	{
		PlayerController = Cast<ATopDownPlayerController>(GetController());
		/* Return if still not set as nothing else should be done. */
		if(!PlayerController)
		{
			return;
		}
		
		/* Set the Mapping Context for Player Input. */
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PointClickMappingContext, 0);
		}
	}	
		
	FindCurrentPath();
}


void ATopDownPlayer::FindCurrentPath()
{
	/* Gets the position under the players mouse */
	FHitResult Hit;
	if(!PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, Hit) || Hit.Location.ContainsNaN())
	{
		return;
	}

	/* As the Character is not controlled directly by the player get the server to ask the Character to run path finding logic */
	Server_CheckPlayerCharacterPath(Hit.Location);

	/* Use the path variables the character found to determine if the path is allowed. */
	/* At the minute it only checks if the end point is not the same, this means there was an obstruction. */
	/* When checked if the path is allowed or not spawn a coloured decal under the mouse for player feedback */
	/* TODO: Spawn a Decal instead of a debug sphere. */
	/* TODO: Draw the path for the player. */
	/* TODO: Set Up Player movement amount and check if the length of the path is allowed. */
	if(PathingVariables.EndPoint.X != PathingVariables.TargetLocation.X || PathingVariables.EndPoint.Y != PathingVariables.TargetLocation.Y)
	{
		DrawDebugSphere(GetWorld(), PathingVariables.TargetLocation, 30.f, 12, FColor::Red, false, -1, 0, 2.f);
		return;
	}	
	
	DrawDebugSphere(GetWorld(), PathingVariables.TargetLocation, 30.f, 12, FColor::Green, false, -1, 0, 2.f);
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
	if(!PlayerController)
	{
		return;
	}
	
	FHitResult Hit;
	if(!PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, Hit) || Hit.Location.ContainsNaN())
	{
		return;
	}

	Server_MovePlayerCharacter(Hit.Location);	
}

void ATopDownPlayer::Server_MovePlayerCharacter_Implementation(FVector TargetLocation)
{
	if(!TopDownCharacter)
	{
		return;
	}
	
	TopDownCharacter->MoveTo(TargetLocation);
}

