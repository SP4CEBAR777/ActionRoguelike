// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USActionComponent : public UActionComponent {
  GENERATED_BODY()

public:
  USActionComponent();

  UFUNCTION(BlueprintCallable, Category = "Actions")
  void AddAction(TSubclassOf<USAction> ActionClass);

  UFUNCTION(BlueprintCallable, Category = "Actions")
  bool StartActionByName(AActor *Instigator, FName ActionName);

  UFUNCTION(BlueprintCallable, Category = "Actions")
  bool StopActionByName(AActor *Instigator, FName ActionName);

protected:
  virtual void BeginPlay() override;

  UPROPERTY()
  TArray<USAction *> Actions;

public:
  // Called every frame
  virtual void
  TickComponent(float DeltaTime, ELevelTick TickType,
                FActorComponentTickFunction *ThisTickFunction) override;
};
