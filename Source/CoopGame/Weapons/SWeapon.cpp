// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleSystemComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	RootComponent = SkeletalMeshComp;

	MuzzleSocketName = "MuzzleFlashSocket";
	TracertTargetName = "BeamEnd";
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASWeapon::Fire()
{
	// Trace the world, from pawn eyes to cross hair location
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

		//DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Green, false, 10.f, 0, 1.f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		FCollisionResponseParams ResponsParams = FCollisionResponseParams::DefaultResponseParam;
		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd,
												 ECollisionChannel::ECC_Visibility,
												 QueryParams, ResponsParams))
		{
			// Blocking hit process damage
			AActor* HitActor = HitResult.GetActor();
			if (HitActor != nullptr)
			{
				// Apply the damage to the struck HitActor
				UGameplayStatics::ApplyPointDamage(HitActor, 20.f, ShotDirection, HitResult, MyOwner->GetInstigatorController(),
												   this, DamageType);

				ensure(ImpactEffect != nullptr);
				if (ImpactEffect != nullptr)
				{
					// Spawn a decal on the Actor at the impact location
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint, 
															 HitResult.ImpactNormal.Rotation());
				}

				// override the default value since we have a blocking hit and need that impact point
				TraceLength = HitResult.ImpactPoint;
			}
		}

		ensure(TraceEffect != nullptr);
		if (TraceEffect != nullptr)
		{
			FVector MuzzelLocation = SkeletalMeshComp->GetSocketLocation(MuzzleSocketName);
			UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, MuzzelLocation);
			if (TracerComponent != nullptr)
			{
				TracerComponent->SetVectorParameter(TracertTargetName, TraceLength);
			}
		}

	}
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

