// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/SWeapon.h"
#include "SGrenadeLauncher.generated.h"

class UParticleSystem;
class AGrenadeProjectile;

/**
 * 
 */
UCLASS()
class COOPGAME_API ASGrenadeLauncher : public ASWeapon
{
	GENERATED_BODY()
	
public:
	ASGrenadeLauncher();

	/** Methods */
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/** Cause the weapon to fire at line traced location */
	virtual void Fire() override;

	/** Properties */
protected:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	USkeletalMeshComponent LauncherMeshComponent;

	/** the explosion particle to be displayed on projectile impact */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* ExplosionEffect;
	
	/** The velocity vector of the fired projectile */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FVector ProjectileVelocity = FVector(1000.f, 0.f, 0.f);


	AGrenadeProjectile GrendateProjectileClass;
};
