// Copyright Spxcebxr Games


#include "Character/TopDownCharacterBase.h"
#include "Components/CapsuleComponent.h"

ATopDownCharacterBase::ATopDownCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

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

void ATopDownCharacterBase::HighlightActor()
{
	
}

void ATopDownCharacterBase::UnHighlightActor()
{
	
}



