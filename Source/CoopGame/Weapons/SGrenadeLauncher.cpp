// Fill out your copyright notice in the Description page of Project Settings.


#include "SGrenadeLauncher.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleSystemComponent.h"

#include "DrawDebugHelpers.h"

ASGrenadeLauncher::ASGrenadeLauncher()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxTraceDistance = 4000;
	MuzzleSocketName = "Muzzle";
}

void ASGrenadeLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGrenadeLauncher::BeginPlay()
{
	Super::BeginPlay();
}

void ASGrenadeLauncher::Fire()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner != nullptr)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * MaxTraceDistance);

		// Set the projectile trace default end 
		FVector TraceLength = TraceEnd;

		ensure(MuzzelEffect != nullptr);
		if (MuzzelEffect != nullptr)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzelEffect, SkeletalMeshComp, MuzzleSocketName);
		}


		// From FPS Game ------------
			// try and fire a projectile
		if (GrendateProjectileClass != nullptr)
		{
			FVector MuzzleLocation = LauncherMeshComponent->GetSocketLocation(MuzzleSocketName);
			FRotator MuzzleRotation = LauncherMeshComponent->GetSocketRotation(MuzzleSocketName);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Instigator = this;

			// spawn the projectile at the muzzle
			GetWorld()->SpawnActor<AGrenadeProjectile>(GrendateProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		}

		/*
		// try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		//DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Green, false, 10.f, 0, 1.f);
		//UGameplayStatics::SuggestProjectileVelocity(GetWorld(), 

		// try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
			}
		}
		
		
		*/

	}
}

