// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class FTimerHandle;

UCLASS()
class COOPGAME_API AGrenadeProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AGrenadeProjectile();

	/** Methods */
public:
	virtual void Tick(float DeltaTime) override;

	/** Detection delegate for causing behavior when projectile hits blocking Actors */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Explosion delegate callback to perform actaul application of damage and destruction of projectile */
	UFUNCTION()
	void OnExplode();
protected:
	virtual void BeginPlay() override;

	/** Properties */
protected:
	/** The primary collision component to dictate behavior when colliding with objects */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* CollisionComp;

	/** The sphere size of the max explosion */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* ExplosionSphere;
	
	/** Handles the movements of the projectile when fired */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;

private:
	/** Timer handle to allow setting of delay from fire to explosion of grenade projectile */
	FTimerHandle ExplosionTimer_TimerHandle;
};
