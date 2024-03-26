// Fill out your copyright notice in the Description page of Project Settings.

#include "BCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABCharacter::ABCharacter() {
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
  CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
  SpringArmComp->SetupAttachment(RootComponent);
  CameraComp->SetupAttachment(SpringArmComp);
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
  AddMovementInput(GetActorForwardVector(), value);
}
void ABCharacter::MoveRight(float value) {
  AddMovementInput(GetActorRightVector(), value);
}
