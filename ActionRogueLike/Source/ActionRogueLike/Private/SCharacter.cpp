// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"

// Sets default values
ASCharacter::ASCharacter() {
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
  SpringArmComp->SetupAttachment(RootComponent);

  CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
  CameraComp->SetupAttachment(SpringArmComp);

  InteractionComp =
      CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

  AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ASCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAxis("MoveForward", this,
                                 &ASCharacter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

  PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
  PlayerInputComponent->BindAxis("LookUp", this,
                                 &APawn::AddControllerPitchInput);

  PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this,
                                   &ASCharacter::PrimaryAttack);
  PlayerInputComponent->BindAction("Jump", IE_Pressed, this,
                                   &ASCharacter::Jump);
  PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this,
                                   &ASCharacter::PrimaryInteract);
}

void ASCharacter::MoveForward(float value) {
  FRotator ControlRot = GetControlRotation();
  ControlRot.Pitch = 0.0f;
  ControlRot.Roll = 0.0f;
  AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float value) {
  FRotator ControlRot = GetControlRotation();
  ControlRot.Pitch = 0.0f;
  ControlRot.Roll = 0.0f;

  FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
  AddMovementInput(RightVector, value);
}

void ASCharacter::PrimaryAttack() {
  PlayAnimMontage(AttackAnim);
  GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
                                  &ASCharacter::PrimaryAttack_TimeElapsed,
                                  0.18f);
}

void ASCharacter::PrimaryAttack_TimeElapsed() {
  if (ensure(ProjectileClass)) {
    FVector HandLoc = GetMesh()->GetSocketLocation("Muzzle_01");

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Instigator = this;

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    FHitResult Hit;

    FVector TraceStart = CameraComp->GetComponentLocation();

    FVector TraceEnd = TraceStart + GetControlRotation().Vector() * 5000.0f;

    if (GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd,
                                                ObjectQueryParams, Params)) {
      TraceEnd = Hit.ImpactPoint;
    }

    FRotator ProjRot = FRotationMatrix::MakeFromX(TraceEnd - HandLoc).Rotator();

    FTransform SpawnTM = FTransform(ProjRot, HandLoc);
    GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
  }
}

void ASCharacter::PrimaryInteract() { InteractionComp->PrimaryInteract(); }
