// Fill out your copyright notice in the Description page of Project Settings.

#include "GrenadeProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "SCharacter.h"

AGrenadeProjectile::AGrenadeProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	// Add dynamic implies that there needs to be a blueprint hookup to cause the hit behavior - Check this
	CollisionComp->OnComponentHit.AddDynamic(this, &AGrenadeProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ExplosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionSphere"));
	ExplosionSphere->InitSphereRadius(25.f);
	ExplosionSphere->SetCollisionProfileName("Projectile");
	// Again players can't walk on the explosion radius
	ExplosionSphere->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	ExplosionSphere->CanCharacterStepUpOn = ECB_No;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	//InitialLifeSpan = 3.0f;
}

void AGrenadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenadeProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	MakeNoise(1.f, Instigator);
}

void AGrenadeProjectile::OnExplode()
{
	TSet<AActor*> OverlappingActors;
	ExplosionSphere->GetOverlappingActors(OverlappingActors);
	for (auto Actor : OverlappingActors)
	{
		ASCharacter* ProjectileTarget = Cast<ASCharacter>(Actor);
		if (ProjectileTarget != nullptr)
		{
			// may not even need this now with the found method to apply damage below
			FVector FromExplosionDistance = ProjectileTarget->GetActorLocation() - GetActorLocation();
			
			// complete implementation of this method call to apply the damage
			//UGameplayStatics::ApplyRadialDamageWithFalloff()
		}
	}

	Destroy();
}

void AGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().ClearTimer(ExplosionTimer_TimerHandle);
	GetWorldTimerManager().SetTimer(ExplosionTimer_TimerHandle, this, &AGrenadeProjectile::OnExplode, 1.f);
}


