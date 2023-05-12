// Copyright Spxcebxr Games


#include "Character/TopDownPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/TopDownCharacter.h"
#include "Character/TopDownPlayerController.h"
#include "Net/UnrealNetwork.h"

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
}

void ATopDownPlayer::BeginPlay()
{
	Super::BeginPlay();	

	/* Only Spawn the Player's Character on the Server as technically it is AI */
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

	if(!PlayerController)
	{
		PlayerController = Cast<ATopDownPlayerController>(GetController());
		if(!PlayerController)
		{
			return;
		}		
	}	
		
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PointClickMappingContext, 0);
	}	
}

void ATopDownPlayer::Click(const FInputActionValue& Value)
{	
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

