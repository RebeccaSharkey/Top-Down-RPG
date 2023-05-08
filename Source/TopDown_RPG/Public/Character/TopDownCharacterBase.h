// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TopDownCharacterBase.generated.h"

UCLASS()
class TOPDOWN_RPG_API ATopDownCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ATopDownCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Cobat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
