// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter {
  GENERATED_BODY()

public:
  ASAICharacter();

protected:
  UPROPERTY(VisibleAnywhere, Category = "Components")
  UPawnSensingComponent *PawnSensingComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  USAttributeComponent *AttributeComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  FName TargetActorKey;

  UPROPERTY(VisibleAnywhere, Category = "Effects")
  FName TimeToHitParamName;

  void SetTargetActor(AActor *NewTarget);

  virtual void PostInitializeComponents() override;

  UFUNCTION()
  void OnHealthChanged(AActor *InstigatorActor,
                       USAttributeComponent *OwningComp, float NewHealth,
                       float Delta);

  UFUNCTION()
  void OnPawnSeen(APawn *Pawn);
};
