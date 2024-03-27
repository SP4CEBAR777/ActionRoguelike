// Fill out your copyright notice in the Description page of Project Settings.

#include "BCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"

// Sets default values
ABCharacter::ABCharacter() {
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SpriteComp = CreateDefaultSubobject<UPaperFlipbookComponent>("SpriteComp");
  SpriteComp->SetupAttachment(RootComponent);
  SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
  CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
  SpringArmComp->SetupAttachment(RootComponent);
  CameraComp->SetupAttachment(SpringArmComp);

  bIsMovingX = false;
}

// Called when the game starts or when spawned
void ABCharacter::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ABCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// Called to bind functionality to input
void ABCharacter::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAxis("MoveForward", this,
                                 &ABCharacter::MoveForward);

  PlayerInputComponent->BindAxis("MoveRight", this, &ABCharacter::MoveRight);
}

void ABCharacter::MoveForward(float value) {
  if (!bIsMovingY && value != 0.0f) {
    bIsMovingX = true;
    FRotator ControlRot = GetControlRotation();
    ControlRot.Pitch = 0;
    ControlRot.Roll = 0;
    AddMovementInput(ControlRot.Vector(), value);
  } else {
    bIsMovingX = false;
  }
}
void ABCharacter::MoveRight(float value) {
  if (!bIsMovingX && value != 0.0f) {
    bIsMovingY = true;
    FRotator ControlRot = GetControlRotation();
    ControlRot.Pitch = 0;
    ControlRot.Roll = 0;
    FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
    AddMovementInput(RightVector, value);
  } else {
    bIsMovingY = false;
  }
}
