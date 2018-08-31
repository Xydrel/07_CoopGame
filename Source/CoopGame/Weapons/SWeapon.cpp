// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	RootComponent = SkeletalMeshComp;

}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASWeapon::Fire()
{
	// Trace the world, from pawn eyes to crosshair location
	AActor* MyOwner = GetOwner();
	if (MyOwner != nullptr)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * MaxTraceDistance);

		// Just a debug trace draw to determine the fire action is occurring and the direction of the projectile
		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Green, false, 10.f, 0, 1.f);

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
				UGameplayStatics::ApplyPointDamage(HitActor, 20.f, ShotDirection, HitResult, MyOwner->GetInstigatorController(), this, DamageType);
			}
		}

	}
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

