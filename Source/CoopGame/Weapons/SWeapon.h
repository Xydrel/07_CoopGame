// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

	/** Methods */
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Cause the weapon to fire at line traced location */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();
	
	/** Properties */
protected:
	/** Weapon skeletal mesh comp */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp = nullptr;

	/** Max distance for weapon trace in centimeters */
	UPROPERTY(EditAnywhere, Category = "LineTrace")
	float MaxTraceDistance = 10000;

	/** The type of damage to be applied to the target */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;
	
};
