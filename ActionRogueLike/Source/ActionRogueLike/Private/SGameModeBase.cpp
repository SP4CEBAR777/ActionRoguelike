// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "AI/SAICharacter.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SAttributeComponent.h"

ASGameModeBase::ASGameModeBase() { SpawnTimerInterval = 2.0f; }

void ASGameModeBase::StartPlay() {
  Super::StartPlay();

  GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this,
                                  &ASGameModeBase::SpawnBotTimerElapsed,
                                  SpawnTimerInterval, true);
}

void ASGameModeBase::SpawnBotTimerElapsed() {
  int32 NumOfAliveBots = 0;
  for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
    ASAICharacter *Bot = *It;

    USAttributeComponent *AttributeComp =
        USAttributeComponent::GetAttributes(Bot);
    if (ensure(AttributeComp) && AttributeComp->IsAlive()) {
      ++NumOfAliveBots;
    }
  }

  UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NumOfAliveBots);

  float MaxBotCount = 10.0f;

  if (DifficultyCurve) {
    MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
  }

  if (NumOfAliveBots >= MaxBotCount) {
    UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
    return;
  }

  UEnvQueryInstanceBlueprintWrapper *QueryInstance =
      UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this,
                                    EEnvQueryRunMode::RandomBest5Pct, nullptr);
  if (ensure(QueryInstance)) {
    QueryInstance->GetOnQueryFinishedEvent().AddDynamic(
        this, &ASGameModeBase::OnQueryCompleted);
  }
}

void ASGameModeBase::OnQueryCompleted(
    UEnvQueryInstanceBlueprintWrapper *QueryInstance,
    EEnvQueryStatus::Type QueryStatus) {
  if (QueryStatus != EEnvQueryStatus::Success) {
    UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
    return;
  }

  TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

  if (Locations.IsValidIndex(0)) {

    GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0],
                                   FRotator::ZeroRotator);

    // Track all the used spawn locations
    DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false,
                    60.0f);
  }
}