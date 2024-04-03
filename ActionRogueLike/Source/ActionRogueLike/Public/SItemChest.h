// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor,
                                        public ISGameplayInterface {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ASItemChest();

  UPROPERTY(EditDefaultsOnly)
  float TargetPitch;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadonly)
  UStaticMeshComponent *LidMesh;
  UPROPERTY(VisibleAnywhere)
  UStaticMeshComponent *BaseMesh;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  void Interact_Implementation(APawn *InstigatorPawn);
};
