// Copyright Spxcebxr Games


#include "Character/TopDownCharacterBase.h"

#include "SWarningOrErrorBox.h"
#include "Components/CapsuleComponent.h"

ATopDownCharacterBase::ATopDownCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCapsuleRadius(50.f);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("Hand_WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ATopDownCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATopDownCharacterBase::HighlightActor(EInteractionType IteractionType)
{
	int32 HighlightColour = CUSTOM_DEPTH_WHITE;
	switch (IteractionType)
	{
	case EIT_Player:
		HighlightColour = CUSTOM_DEPTH_GREEN;
		break;
	case EIT_Enemy:
		HighlightColour = CUSTOM_DEPTH_RED;
		break;
	case EIT_Item:
		HighlightColour = CUSTOM_DEPTH_WHITE;
		break;
	case EIT_DEFAULT:
	case EIT_MAX:
		UE_LOG(LogTemp, Warning, TEXT("Invalid Interaction Type."));
		return;		
	}
	
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(HighlightColour);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(HighlightColour);
}

void ATopDownCharacterBase::UnHighlightActor()
{	
	GetMesh()->SetRenderCustomDepth(false);

	Weapon->SetRenderCustomDepth(false);
}



