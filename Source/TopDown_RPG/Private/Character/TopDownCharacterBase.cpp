// Copyright Spxcebxr Games


#include "Character/TopDownCharacterBase.h"

ATopDownCharacterBase::ATopDownCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("Hand_WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATopDownCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}



