// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
  RootComponent = SphereComp;
  MovementComp =
      CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");

  EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
  EffectComp->SetupAttachment(SphereComp);

  Damage = 20.0f;
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay() {
  Super::BeginPlay();

  SphereComp->OnComponentBeginOverlap.AddDynamic(
      this, &ASMagicProjectile::OnActorOverlap);
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent *OverlappedComponent,
                                       AActor *OtherActor,
                                       UPrimitiveComponent *OtherComp,
                                       int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult &SweepResult) {
  if (OtherActor) {
    USAttributeComponent *AttributeComp = Cast<USAttributeComponent>(
        OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
    if (AttributeComp) {
      AttributeComp->ApplyHealthChange(-Damage);
      Destroy();
    }
  }
}